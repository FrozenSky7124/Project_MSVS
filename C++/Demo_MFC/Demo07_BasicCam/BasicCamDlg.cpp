
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
	CString         fileName;
	UINT			fileCount = 0;

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
			/*
			CameraReleaseImageBuffer(pThis->m_hCamera, pRgbBuffer);
			memcpy(&pThis->m_sFrInfo, &sFrameInfo, sizeof(tSdkFrameHead));

			fileName.Format("X:\\Images\\ImageSnap-%3d.BMP", fileCount++);
			CameraSaveImage(pThis->m_hCamera, fileName.GetBuffer(1), pThis->m_pFrameBuffer, &sFrameInfo, FILE_BMP, 100);
			fileName.ReleaseBuffer();*/
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
	m_iMode = 0;
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
	ON_BN_CLICKED(IDC_BTN_CAPTSTART, &BasicCamDlg::OnBnClickedCaptStart)
	ON_BN_CLICKED(IDC_BTN_CAPTSTOP, &BasicCamDlg::OnBnClickedBtnCaptStop)
	ON_BN_CLICKED(IDC_BTN_QUIT, &BasicCamDlg::OnBnClickedBtnQuit)
	ON_BN_CLICKED(IDC_RADIO_MODECAPT, &BasicCamDlg::OnBnClickedRadioModeCapt)
	ON_BN_CLICKED(IDC_RADIO_MODEREVIEW, &BasicCamDlg::OnBnClickedRadioModeReview)
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
	// ����ͼ��Ԥ���ؼ��Ĵ�С��λ��
	CRect viewRect;
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&viewRect);
	viewRect.SetRect(10, 10, 10 + 1024, 10 + 768);
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(viewRect);
	// �ؼ�״̬��ʼ��
	((CButton *)GetDlgItem(IDC_RADIO_MODECAPT))->SetCheck(1);
	
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
	//if (CameraPlay(m_hCamera) != 0) TRACE("���빤��ģʽʧ�ܣ�\n");
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR BasicCamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void BasicCamDlg::OnBnClickedCaptStart()
{
	int errorCode;
	if ((errorCode = CameraPlay(m_hCamera)) != CAMERA_STATUS_SUCCESS) //ʹ������빤��ģʽ������SDK��ʼ�������ͼ��
	{
		AfxMessageBox(_T("��ʼ�ɼ�ʧ�ܣ� ������룺%d ��"), errorCode);
		return;
	}
	m_bPause = FALSE;
	GetDlgItem(IDC_BTN_CAPTSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CAPTSTOP)->EnableWindow(TRUE);
}

void BasicCamDlg::OnBnClickedBtnCaptStop()
{
	int errorCode;
	if ((errorCode = CameraPause(m_hCamera)) != CAMERA_STATUS_SUCCESS) //ʹ���������ͣģʽ
	{
		AfxMessageBox(_T("ֹͣ�ɼ�ʧ�ܣ� ������룺%d ��"), errorCode);
		return;
	}
	m_bPause = TRUE;
	GetDlgItem(IDC_BTN_CAPTSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CAPTSTOP)->EnableWindow(FALSE);
}

void BasicCamDlg::OnCancel()
{
	AfxMessageBox(_T("����[�˳�����]��ť�رգ�"));
	//OnClose();
	//CDialogEx::OnCancel();
}

void BasicCamDlg::OnClose()
{
	AfxMessageBox(_T("����[�˳�����]��ť�رգ�"));
	//OnClose();
	//CDialogEx::OnCancel();
}

void BasicCamDlg::OnBnClickedBtnQuit()
{
	if (m_hCamera > 0)
	{
		if (NULL != m_hDispThread)
		{
			//�ȴ��ɼ��߳̽���
			m_bExit = TRUE;
			::WaitForSingleObject(m_hDispThread, INFINITE);
			TRACE(_T("�ɼ��߳�����ֹ\n"));
			CloseHandle(m_hDispThread);
			m_hDispThread = NULL;
		}

		//����ʼ�������
		TRACE(_T("���ж��\n"));
		CameraUnInit(m_hCamera);
		m_hCamera = 0;
	}

	if (m_pFrameBuffer)
	{
		TRACE(_T("�ɼ��������\n"));
		CameraAlignFree(m_pFrameBuffer);
		m_pFrameBuffer = NULL;
	}
	CDialogEx::OnCancel();
}

void BasicCamDlg::OnBnClickedRadioModeCapt()
{
	if (m_iMode == 0)
	{
		AfxMessageBox(_T("�Ѿ����ڡ��ɼ�ģʽ����"));
		return;
	}
	m_iMode = 0;
	((CButton *)GetDlgItem(IDC_RADIO_MODECAPT))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_MODEREVIEW))->SetCheck(0);
	GetDlgItem(IDC_BTN_REVIEWSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_REVIEWSTOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_OPENFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CAPTSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CAPTSTOP)->EnableWindow(FALSE);
}

void BasicCamDlg::OnBnClickedRadioModeReview()
{
	if (m_iMode == 1)
	{
		AfxMessageBox(_T("�Ѿ����ڡ��ط�ģʽ����"));
		return;
	}
	if (m_bPause == FALSE)
	{
		((CButton *)GetDlgItem(IDC_RADIO_MODECAPT))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_MODEREVIEW))->SetCheck(0);
		AfxMessageBox(_T("���ڲɼ��У�����[ֹͣ�ɼ�]��"));
		return;
	}
	m_iMode = 1;
	((CButton *)GetDlgItem(IDC_RADIO_MODECAPT))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO_MODEREVIEW))->SetCheck(1);
	GetDlgItem(IDC_BTN_CAPTSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CAPTSTOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_REVIEWSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_REVIEWSTOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_OPENFILE)->EnableWindow(TRUE);
}
