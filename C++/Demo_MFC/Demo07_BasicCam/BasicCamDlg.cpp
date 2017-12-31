
// BasicCamDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo07_BasicCam.h"
#include "BasicCamDlg.h"
#include "afxdialogex.h"
#include "malloc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*ͼ��ץȡ�̣߳���������SDK�ӿں�����ȡͼ��*/
UINT WINAPI uiDisplayThread(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	BasicCamDlg*	pThis = (BasicCamDlg*)lpParam;
	BYTE*			pRgbBuffer;

	//����0
	memset(&sFrameInfo, 0, sizeof(tSdkFrameHead));

	while (!pThis->m_bExit)
	{
		if ((pRgbBuffer = CameraGetImageBufferPriorityEx(pThis->m_hCamera, &sFrameInfo.iWidth, &sFrameInfo.iHeight, 1000,
			CAMERA_GET_IMAGE_PRIORITY_NEWEST)) != 0)
		{
			sFrameInfo.uiMediaType = CAMERA_MEDIA_TYPE_BGR8;

			//�ֱ��ʸı��ˣ���ˢ�±���
			if (pThis->m_sFrInfo.iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo.iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo.iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo.iHeight = sFrameInfo.iHeight;
				pThis->InvalidateRect(NULL);
			}

			// �����ͼ����
			//pThis->ProcessSnapRequest(pRgbBuffer, &sFrameInfo);

			//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��
			CameraImageOverlay(pThis->m_hCamera, pRgbBuffer, &sFrameInfo);
			CameraDisplayRGB24(pThis->m_hCamera, pRgbBuffer, &sFrameInfo);
			pThis->m_iDispFrameNum++;
		}
	}
	_endthreadex(0);
	return 0;
}

// BasicCamDlg �Ի���

BasicCamDlg::BasicCamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hCamera = -1;
	m_bPause = TRUE;
	m_iDispFrameNum = 0;
	m_bExit = FALSE;
	m_hDispThread = NULL;
	m_pFrameBuffer = NULL;
}

void BasicCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_cPreview);
}

BEGIN_MESSAGE_MAP(BasicCamDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &BasicCamDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// BasicCamDlg ��Ϣ�������

BOOL BasicCamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	do
	{
		if (CameraSdkInit(1) != 0)
			TRACE(_T("��ʼ��SDKʧ�ܣ�\n"));
		else
			TRACE(_T("��ʼ��SDK�ɹ���\n"));
		if (!InitCamera())
		{
			break;
		}
		return TRUE;
		
	} while (0);
	
	// û���ҵ�������ʼ��ʧ�ܣ��˳�����
	EndDialog(0);

	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//  �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

BOOL BasicCamDlg::InitCamera()
{
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	tSdkCameraCapbility sCameraInfo;

	//ö���豸������豸�б�
	//Enumerate camera
	iCameraNums = CameraEnumerateDeviceEx();

	if (iCameraNums <= 0)
	{
		MessageBox("�޷��ҵ������");
		return FALSE;
	}

	//��ʾ���У�����ֻ����������һ���������ˣ�ֻ��ʼ����һ�������(-1,-1)��ʾ�����ϴ��˳�ǰ����Ĳ���������ǵ�һ��ʹ�ø�����������Ĭ�ϲ���.
	if ((status = CameraInitEx(0, -1, -1, &m_hCamera)) != CAMERA_STATUS_SUCCESS)
	{
		CString msg;
		msg.Format("Failed to init the camera! Error code is %d", status);
		MessageBox(msg + "��ԭ����" + CameraGetErrorString(status));
		return FALSE;
	}

	//��ø��������������
	if (CameraGetCapability(m_hCamera, &sCameraInfo) != 0) TRACE("��ø��������������ʱ����\n");

	//ʹ��SDK��װ�õ���ʾ�ӿ�
	if (CameraDisplayInit(m_hCamera, m_cPreview.GetSafeHwnd()) != 0) TRACE("��ʼ����ʾ�ӿڳ���\n");

	m_cPreview.GetClientRect(&rect);
	//Set display window size
	if (CameraSetDisplaySize(m_hCamera, rect.right - rect.left, rect.bottom - rect.top) != 0) TRACE("������ʾ���ڴ�С����\n");

	//֪ͨSDK�ڲ��������������ҳ�档��������Ϊ����������ơ���Ҳ���Ը���SDK�Ľӿ����Լ�ʵ���������������ҳ�棬
	//�������ǽ�����ʹ��SDK�ڲ��Զ������ķ�ʽ����ʡȥ���ڽ��濪���ϵĴ���ʱ�䡣
	//Create the settings window for the camera
	CameraCreateSettingPageEx(m_hCamera);

	m_hDispThread = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread, this, 0, &m_threadID);
	ASSERT(m_hDispThread);
	SetThreadPriority(m_hDispThread, THREAD_PRIORITY_HIGHEST);

	//ʹ������빤��ģʽ������SDK��ʼ�������ͼ��
	if (CameraPlay(m_hCamera) != 0) TRACE("���빤��ģʽʧ�ܣ�\n");
	m_bPause = FALSE;
	//GetDlgItem(IDC_BUTTON_PREVIEW)->SetWindowText(gLanguage ? "��ͣ" : "Pause");
	return TRUE;
}

void BasicCamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR BasicCamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void BasicCamDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TRACE(_T("����ʼ�����\n"));
	if (m_hCamera > 0)
	{
		if (NULL != m_hDispThread)
		{
			//�ȴ��ɼ��߳̽���
			m_bExit = TRUE;
			::WaitForSingleObject(m_hDispThread, INFINITE);
			CloseHandle(m_hDispThread);
			m_hDispThread = NULL;
		}

		//����ʼ�������
		CameraUnInit(m_hCamera);
		m_hCamera = 0;
	}

	if (m_pFrameBuffer)
	{
		CameraAlignFree(m_pFrameBuffer);
		m_pFrameBuffer = NULL;
	}
	CDialogEx::OnClose();
}


void BasicCamDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TRACE(_T("����ʼ�����\n"));
	if (m_hCamera > 0)
	{
		if (NULL != m_hDispThread)
		{
			//�ȴ��ɼ��߳̽���
			m_bExit = TRUE;
			::WaitForSingleObject(m_hDispThread, INFINITE);
			CloseHandle(m_hDispThread);
			m_hDispThread = NULL;
		}

		//����ʼ�������
		CameraUnInit(m_hCamera);
		m_hCamera = 0;
	}

	if (m_pFrameBuffer)
	{
		CameraAlignFree(m_pFrameBuffer);
		m_pFrameBuffer = NULL;
	}
	CDialogEx::OnCancel();
}
