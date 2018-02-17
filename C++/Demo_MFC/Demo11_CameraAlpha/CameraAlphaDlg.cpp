
// CameraAlphaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo11_CameraAlpha.h"
#include "CameraAlphaDlg.h"
#include "afxdialogex.h"
#include "malloc.h"
#include "Dib_Ex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*ͼ��ץȡ�̣߳���������SDK�ӿں�����ȡͼ��*/
UINT WINAPI uiDisplayThread(LPVOID lpParam)
{
	tSdkFrameHead		sFrameInfo;
	CameraAlphaDlg*	pThis = (CameraAlphaDlg*)lpParam;
	BYTE*			pbyBuffer;
	CString			fileName;
	int				iSaveCounts = 0;
	int				statusCode;
	CString			strTime;
	CTime			curTime;
	CRect			tRect;
	CDib			cDibImage;
	
	pThis->GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&tRect);
	cDibImage.MakeRgbQuadMem(8);

	//����0
	memset(&sFrameInfo, 0, sizeof(tSdkFrameHead));

	while (!pThis->m_bExit)
	{
		if (CameraGetImageBuffer(pThis->m_hCamera, &sFrameInfo, &pbyBuffer, 1000) == CAMERA_STATUS_SUCCESS)
		{
			//����RAW���ݣ��������CameraImageProcess
			//CameraSaveImage(pThis->m_hCamera, fileName.GetBuffer(1), pbyBuffer, &sFrameInfo, FILE_RAW, 100);
			
			//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
			//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
			statusCode = CameraImageProcess(pThis->m_hCamera, pbyBuffer, pThis->m_pFrameBuffer, &sFrameInfo);//����ģʽ

			//�ֱ��ʸı��ˣ���ˢ�±���
			if (pThis->m_sFrInfo.iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo.iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo.iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo.iHeight = sFrameInfo.iHeight;
				pThis->InvalidateRect(NULL);
			}

			// �����ͼ����
			//pThis->ProcessSnapRequest(pRgbBuffer, &sFrameInfo);

			if (statusCode == CAMERA_STATUS_SUCCESS)
			{
				//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
				//CameraImageOverlay(pThis->m_hCamera, pThis->m_pFrameBuffer, &sFrameInfo);
				//CameraDisplayRGB24(pThis->m_hCamera, pThis->m_pFrameBuffer, &sFrameInfo);

				//����CDib�����뻺�����ݲ���ʾͼ��
				
				cDibImage.LoadFromBuffer(pThis->m_pFrameBuffer, sFrameInfo.iWidth, sFrameInfo.iHeight, 8);
				cDibImage.Draw(pThis->m_pCDC, CPoint(0, 0), CSize(tRect.Width(), tRect.Height()));

				if (pThis->m_bSaveFile == TRUE)
				{
					curTime = CTime::GetCurrentTime();
					strTime.Format(_T("_%d%02d%02d%02d%02d%02d_"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
					fileName.Format(_T("%05d.BMP"), iSaveCounts++);
					fileName = pThis->m_csSaveFolder + _T("Image") + strTime + fileName;
					// ����Ϊ24λBMP�ļ�
					//CameraSaveImage(pThis->m_hCamera, fileName.GetBuffer(1), pThis->m_pFrameBuffer, &sFrameInfo, FILE_BMP, 100);
					// ����Ϊ8λBMP�ļ�
					CameraSaveImage(pThis->m_hCamera, fileName.GetBuffer(1), pThis->m_pFrameBuffer, &sFrameInfo, FILE_BMP_8BIT, 100);
					// CDib�� ����ͼ��
					//cDibImage.SaveToFile(fileName.GetBuffer(1));
					fileName.ReleaseBuffer();
				}
			}
			pThis->m_iDispFrameNum++;

			//�ڳɹ�����CameraGetImageBuffer�󣬱������CameraReleaseImageBuffer���ͷŻ�õ�buffer��
			//�����ٴε���CameraGetImageBufferʱ�����򽫱�����֪�������߳��е���CameraReleaseImageBuffer���ͷ���buffer
			CameraReleaseImageBuffer(pThis->m_hCamera, pbyBuffer);

			memcpy(&pThis->m_sFrInfo, &sFrameInfo, sizeof(tSdkFrameHead));
		}
	}
	_endthreadex(0);
	TRACE(_T("ͼ��ɼ��߳��˳�!"));
	return 0;
}

// �ط��߳�
UINT WINAPI uiReviewThread(LPVOID lpParam)
{
	CameraAlphaDlg*	pThis = (CameraAlphaDlg*)lpParam;
	CFileFind		imageFind;
	BOOL			bFound;
	CString			imagePath;
	CWnd*			pWnd = pThis->GetDlgItem(IDC_STATIC_VIEW);
	CDC*			pDC = pWnd->GetDC();
	HDC				hDC = pDC->m_hDC;
	CRect			rectPicCtrl;
	CImage			tempImage;

	pWnd->GetClientRect(&rectPicCtrl);
	bFound = imageFind.FindFile(pThis->m_csReviewFolder + _T("\\*.BMP"));
	while (bFound && !pThis->m_bReviewEnd)
	{
		bFound = imageFind.FindNextFile(); //�״�ִ��ѡ���������ĵ�һ���ļ����Ժ�ִ�н�ѡ����һ���ļ�
		if (imageFind.IsDots()) continue; //����.��..�ļ�
		if (imageFind.IsDirectory()) continue; //�����ļ���
		
		imagePath = imageFind.GetFilePath();
		//��ʾͼ��
		TRACE(imagePath + _T("\n"));
		tempImage.Load(imagePath);
		::SetStretchBltMode(hDC, HALFTONE);
		::SetBrushOrgEx(hDC, 0, 0, NULL);
		tempImage.Draw(hDC, rectPicCtrl);
		tempImage.Destroy();
		Sleep(40);
	}
	ReleaseDC(*pWnd, hDC);
	imageFind.Close();
	pThis->m_bReviewEnd = TRUE;
	pThis->GetDlgItem(IDC_BTN_REVIEWSTART)->EnableWindow(TRUE);
	pThis->GetDlgItem(IDC_BTN_REVIEWSTOP)->EnableWindow(FALSE);
	TRACE(_T("ͼ��ط��߳��˳�!"));
	_endthreadex(0);	
	return 0;
}

// Config Save
void CameraAlphaDlg::ConfigSave()
{
	CFile fout;
	SConfig tempConfig;
	
	::ZeroMemory(&tempConfig, sizeof(SConfig));
	tempConfig.bSaveFile = this->m_bSaveFile;
	memcpy(&tempConfig.sSaveFolder, this->m_csSaveFolder.GetString(), sizeof(TCHAR)*MAX_PATH);
	if (!fout.Open(_T("BasicCamConfig.DAT"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		AfxMessageBox(_T("���������ļ��쳣��"));
		return;
	}
	fout.Write(&tempConfig, sizeof(SConfig));
	fout.Close();
}

// Config Read
void CameraAlphaDlg::ConfigRead()
{
	CFile fin;
	SConfig tempConfig;

	::ZeroMemory(&tempConfig, sizeof(SConfig));
	if (!fin.Open(_T("BasicCamConfig.DAT"), CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("������Ĭ�����ã�"));
		return;
	}
	fin.Read(&tempConfig, sizeof(SConfig));
	fin.Close();
	this->m_bSaveFile = tempConfig.bSaveFile;
	this->m_csSaveFolder = tempConfig.sSaveFolder;
	if (m_bSaveFile == TRUE) ((CComboBox *)GetDlgItem(IDC_COMBO_SAVEFILE))->SetCurSel(0);
	SetDlgItemTextA(IDC_STATIC_SAVEPATH, m_csSaveFolder);
}

// CameraAlphaDlg �Ի���

CameraAlphaDlg::CameraAlphaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{
	m_hIcon				=	AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hCamera			=	-1;
	m_bPause			=	TRUE;
	m_iDispFrameNum		=	0;
	m_bExit				=	FALSE;
	m_hDispThread		=	NULL;
	m_pFrameBuffer		=	NULL;
	m_iMode				=	0;
	m_bSaveFile			=	FALSE;
	m_csSaveFolder		=	_T("CameraImages\\");
	m_csReviewFolder	=	_T("");
	m_tIDReviewThread	=	NULL;
	m_hReviewThread		=	NULL;
	m_bReviewEnd		=	TRUE;
}

void CameraAlphaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_cPreview);
	DDX_Control(pDX, IDC_BTN_CONFIG, m_BtnConfig);
}

BEGIN_MESSAGE_MAP(CameraAlphaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CAPTSTART, &CameraAlphaDlg::OnBnClickedCaptStart)
	ON_BN_CLICKED(IDC_BTN_CAPTSTOP, &CameraAlphaDlg::OnBnClickedBtnCaptStop)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CameraAlphaDlg::OnBnClickedBtnQuit)
	ON_BN_CLICKED(IDC_RADIO_MODECAPT, &CameraAlphaDlg::OnBnClickedRadioModeCapt)
	ON_BN_CLICKED(IDC_RADIO_MODEREVIEW, &CameraAlphaDlg::OnBnClickedRadioModeReview)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CameraAlphaDlg::OnBnClickedBtnBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_SAVEFILE, &CameraAlphaDlg::OnSelchangeComboSavefile)
	ON_BN_CLICKED(IDC_BTN_CONFIG, &CameraAlphaDlg::OnBnClickedBtnConfig)
	ON_STN_CLICKED(IDC_STATIC_SAVEPATH, &CameraAlphaDlg::OnStnClickedStaticSavepath)
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CameraAlphaDlg::OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_BTN_REVIEWSTART, &CameraAlphaDlg::OnBnClickedBtnReviewStart)
	ON_BN_CLICKED(IDC_BTN_REVIEWSTOP, &CameraAlphaDlg::OnBnClickedBtnReviewstop)
END_MESSAGE_MAP()


// CameraAlphaDlg ��Ϣ�������

BOOL CameraAlphaDlg::OnInitDialog()
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

	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();
	
	// �ؼ�״̬��ʼ��
	HICON m_IconBtn = AfxGetApp()->LoadIcon(IDI_ICON_CONFIG);
	m_BtnConfig.SetIcon(m_IconBtn);

	((CButton *)GetDlgItem(IDC_RADIO_MODECAPT))->SetCheck(1);

	((CComboBox *)GetDlgItem(IDC_COMBO_SAVEFILE))->AddString(_T("��������"));
	((CComboBox *)GetDlgItem(IDC_COMBO_SAVEFILE))->AddString(_T("����������"));
	((CComboBox *)GetDlgItem(IDC_COMBO_SAVEFILE))->SetCurSel(1);

	GetDlgItem(IDC_STATIC_SAVEPATH)->SetWindowTextA(m_csSaveFolder);

	//Test

	ConfigRead();

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

BOOL CameraAlphaDlg::InitCamera()
{
	tSdkCameraDevInfo sCameraList[10];
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	tSdkCameraCapbility sCameraInfo;

	//ö���豸������豸�б�
	iCameraNums = CameraEnumerateDeviceEx();

	iCameraNums = 10;//����CameraEnumerateDeviceǰ��������iCameraNums = 10����ʾ���ֻ��ȡ10���豸�������Ҫö�ٸ�����豸�������sCameraList����Ĵ�С��iCameraNums��ֵ
	if (CameraEnumerateDevice(sCameraList, &iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
	{
		MessageBox("δ�ҵ������");
		return FALSE;
	}

	//��ʾ���У�����ֻ����������һ���������ˣ�ֻ��ʼ����һ�������(-1,-1)��ʾ�����ϴ��˳�ǰ����Ĳ���������ǵ�һ��ʹ�ø�����������Ĭ�ϲ���.
	if ((status = CameraInit(&sCameraList[0], -1, -1, &m_hCamera)) != CAMERA_STATUS_SUCCESS)
	{
		CString msg;
		msg.Format("Failed to init the camera! Error code is %d", status);
		MessageBox(msg + "��ԭ����" + CameraGetErrorString(status));
		return FALSE;
	}

	//��ø��������������
	if (CameraGetCapability(m_hCamera, &sCameraInfo) != 0) TRACE("��ø��������������ʱ����\n");

	m_pFrameBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax * 4, 16);
	ASSERT(m_pFrameBuffer);

	//����Ϊ8bit�Ҷȸ�ʽ���ڰ����������Ϊ8bit�ĻҶ�ͼ��������Խ�Լ�ڴ�洢�ռ䣬ͬʱ�����ͼ���㷨������ٶȡ�����halcon��labview�Լ�opencv��֧��8bit�ĻҶ�λͼֱ�Ӵ���
	CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8);

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

void CameraAlphaDlg::OnPaint()
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
HCURSOR CameraAlphaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CameraAlphaDlg::OnBnClickedCaptStart()
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

void CameraAlphaDlg::OnBnClickedBtnCaptStop()
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

void CameraAlphaDlg::OnBnClickedRadioModeCapt()
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

void CameraAlphaDlg::OnBnClickedRadioModeReview()
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


void CameraAlphaDlg::OnBnClickedBtnBrowse()
{
	//typedef struct _browseinfo {
	//	HWND hwndOwner;            // �����ھ��  
	//	LPCITEMIDLIST pidlRoot;    // Ҫ��ʾ���ļ�Ŀ¼�Ի���ĸ�(Root)  
	//	LPTSTR pszDisplayName;     // ���汻ѡȡ���ļ���·���Ļ�����  
	//	LPCTSTR lpszTitle;         // ��ʾλ�ڶԻ������ϲ��ı���  
	//	UINT ulFlags;              // ָ���Ի������ۺ͹��ܵı�־  
	//	BFFCALLBACK lpfn;          // �����¼��Ļص�����  
	//	LPARAM lParam;             // Ӧ�ó��򴫸��ص������Ĳ���  
	//	int iImage;                // �ļ��жԻ����ͼƬ����  
	//} BROWSEINFO, *PBROWSEINFO, *LPBROWSEINFO
	TCHAR acDir[MAX_PATH] = { 0 };
	BROWSEINFO browInfo;
	LPITEMIDLIST lpidlBrowse;

	memset(&browInfo, 0, sizeof(BROWSEINFO));

	browInfo.hwndOwner = this->m_hWnd;
	browInfo.pidlRoot = NULL;
	browInfo.lpszTitle = _T("��ѡ��ͼ��洢���ļ��У�");
	browInfo.pszDisplayName = acDir;
	browInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	browInfo.lpfn = NULL;
	browInfo.lParam = 0;
	browInfo.iImage = 0;

	lpidlBrowse = ::SHBrowseForFolder(&browInfo);
	if (lpidlBrowse == NULL) return;
	if (!::SHGetPathFromIDList(lpidlBrowse, acDir)) return;

	m_csSaveFolder = acDir;
	m_csSaveFolder += "\\";
	SetDlgItemTextA(IDC_STATIC_SAVEPATH, m_csSaveFolder);
}

void CameraAlphaDlg::OnStnClickedStaticSavepath()
{
	if (m_csSaveFolder == CString(_T("CameraImages\\")))
	{
		TCHAR tcPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, tcPath);
		_tcscat_s(tcPath, _T("\\CameraImages\\"));
		::ShellExecute(NULL, "explore", tcPath, NULL, NULL, SW_SHOWDEFAULT);
	}
	else
	{
		::ShellExecute(NULL, "explore", m_csSaveFolder, NULL, NULL, SW_SHOWDEFAULT);
	}
}

void CameraAlphaDlg::OnSelchangeComboSavefile()
{
	int i = ((CComboBox *)GetDlgItem(IDC_COMBO_SAVEFILE))->GetCurSel();
	if (0 == i)
		this->m_bSaveFile = TRUE;
	else
		this->m_bSaveFile = FALSE;
}

void CameraAlphaDlg::OnBnClickedBtnConfig()
{
	CameraShowSettingPage(m_hCamera, TRUE);
}

void CameraAlphaDlg::OnCancel()
{
	AfxMessageBox(_T("����[�˳�����]��ť�رգ�"));
	//OnClose();
	//CDialogEx::OnCancel();
}

void CameraAlphaDlg::OnClose()
{
	AfxMessageBox(_T("����[�˳�����]��ť�رգ�"));
	//OnClose();
	//CDialogEx::OnCancel();
}

void CameraAlphaDlg::OnBnClickedBtnQuit()
{
	ConfigSave(); //��������
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

void CameraAlphaDlg::OnBnClickedBtnOpenfile()
{
	TCHAR acDir[MAX_PATH] = { 0 };
	BROWSEINFO browInfo;
	LPITEMIDLIST lpidlBrowse;

	memset(&browInfo, 0, sizeof(BROWSEINFO));

	browInfo.hwndOwner = this->m_hWnd;
	browInfo.pidlRoot = NULL;
	browInfo.lpszTitle = _T("��ѡ��ط�ͼ����ļ��У�");
	browInfo.pszDisplayName = acDir;
	browInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	browInfo.lpfn = NULL;
	browInfo.lParam = 0;
	browInfo.iImage = 0;

	lpidlBrowse = ::SHBrowseForFolder(&browInfo);
	if (lpidlBrowse == NULL) return;
	if (!::SHGetPathFromIDList(lpidlBrowse, acDir)) return;

	m_csReviewFolder = acDir;

	// �����ط�Ŀ¼���洢�ط��ļ����ļ�����Ϣ
	CFileFind tFileFind;
	BOOL bFound;
	UINT iFileCount = 0;

	bFound = tFileFind.FindFile(m_csReviewFolder + _T("\\*.BMP"));
	while (bFound)
	{
		bFound = tFileFind.FindNextFile(); //�״�ִ��ѡ���������ĵ�һ���ļ����Ժ�ִ�н�ѡ����һ���ļ�
		if (tFileFind.IsDots()) continue; //����.��..�ļ�
		if (tFileFind.IsDirectory()) continue; //�����ļ���
		iFileCount++;
	}

	TRACE(_T("Find: %d files.\n"), iFileCount);
}


void CameraAlphaDlg::OnBnClickedBtnReviewStart()
{
	if (m_csReviewFolder == CString(_T("")))
	{
		AfxMessageBox(_T("δ���ûط�Ŀ¼��"));
		return;
	}
	m_bReviewEnd = FALSE;
	m_hReviewThread = (HANDLE)_beginthreadex(NULL, 0, &uiReviewThread, this, 0, &m_tIDReviewThread);
	ASSERT(m_hReviewThread);
	SetThreadPriority(m_hReviewThread, THREAD_PRIORITY_HIGHEST);
	GetDlgItem(IDC_BTN_REVIEWSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_REVIEWSTOP)->EnableWindow(TRUE);
}


void CameraAlphaDlg::OnBnClickedBtnReviewstop()
{
	m_bReviewEnd = TRUE;
}
