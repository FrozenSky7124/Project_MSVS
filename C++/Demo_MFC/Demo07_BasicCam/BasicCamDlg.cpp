
// BasicCamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo07_BasicCam.h"
#include "BasicCamDlg.h"
#include "afxdialogex.h"
#include "malloc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*图像抓取线程，主动调用SDK接口函数获取图像*/
UINT WINAPI uiDisplayThread(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	BasicCamDlg*	pThis = (BasicCamDlg*)lpParam;
	BYTE*			pRgbBuffer;
	CString         fileName;
	UINT			fileCount = 0;

	//先清0
	memset(&sFrameInfo, 0, sizeof(tSdkFrameHead));

	while (!pThis->m_bExit)
	{
		if ((pRgbBuffer = CameraGetImageBufferPriorityEx(pThis->m_hCamera, &sFrameInfo.iWidth, &sFrameInfo.iHeight, 1000,
			CAMERA_GET_IMAGE_PRIORITY_NEWEST)) != 0)
		{
			sFrameInfo.uiMediaType = CAMERA_MEDIA_TYPE_BGR8;

			//分辨率改变了，则刷新背景
			if (pThis->m_sFrInfo.iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo.iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo.iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo.iHeight = sFrameInfo.iHeight;
				pThis->InvalidateRect(NULL);
			}

			// 处理截图请求
			//pThis->ProcessSnapRequest(pRgbBuffer, &sFrameInfo);

			//调用SDK封装好的显示接口来显示图像
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

// BasicCamDlg 对话框

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


// BasicCamDlg 消息处理程序

BOOL BasicCamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 调整图像预览控件的大小和位置
	CRect viewRect;
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&viewRect);
	viewRect.SetRect(10, 10, 10 + 1024, 10 + 768);
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(viewRect);
	// 控件状态初始化
	((CButton *)GetDlgItem(IDC_RADIO_MODECAPT))->SetCheck(1);
	
	do
	{
		if (CameraSdkInit(1) != 0)
			TRACE(_T("初始化SDK失败！\n"));
		else
			TRACE(_T("初始化SDK成功！\n"));
		if (!InitCamera())
		{
			break;
		}
		return TRUE;

	} while (0);

	// 没有找到相机或初始化失败，退出程序
	EndDialog(0);

	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL BasicCamDlg::InitCamera()
{
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	tSdkCameraCapbility sCameraInfo;

	//枚举设备，获得设备列表
	//Enumerate camera
	iCameraNums = CameraEnumerateDeviceEx();

	if (iCameraNums <= 0)
	{
		MessageBox("无法找到相机！");
		return FALSE;
	}

	//该示例中，我们只假设连接了一个相机。因此，只初始化第一个相机。(-1,-1)表示加载上次退出前保存的参数，如果是第一次使用该相机，则加载默认参数.
	if ((status = CameraInitEx(0, -1, -1, &m_hCamera)) != CAMERA_STATUS_SUCCESS)
	{
		CString msg;
		msg.Format("Failed to init the camera! Error code is %d", status);
		MessageBox(msg + "，原因是" + CameraGetErrorString(status));
		return FALSE;
	}

	//获得该相机的特性描述
	if (CameraGetCapability(m_hCamera, &sCameraInfo) != 0) TRACE("获得该相机的特性描述时出错！\n");

	//使用SDK封装好的显示接口
	if (CameraDisplayInit(m_hCamera, m_cPreview.GetSafeHwnd()) != 0) TRACE("初始化显示接口出错！\n");

	m_cPreview.GetClientRect(&rect);
	//Set display window size
	if (CameraSetDisplaySize(m_hCamera, rect.right - rect.left, rect.bottom - rect.top) != 0) TRACE("设置显示窗口大小出错！\n");

	//通知SDK内部建该相机的属性页面。窗口名称为该相机的名称。您也可以根据SDK的接口来自己实现相机的属性配置页面，
	//但是我们建议您使用SDK内部自动创建的方式，来省去您在界面开发上的大量时间。
	//Create the settings window for the camera
	CameraCreateSettingPageEx(m_hCamera);

	m_hDispThread = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread, this, 0, &m_threadID);
	ASSERT(m_hDispThread);
	SetThreadPriority(m_hDispThread, THREAD_PRIORITY_HIGHEST);

	//使相机进入工作模式，并且SDK开始接收相机图像
	//if (CameraPlay(m_hCamera) != 0) TRACE("进入工作模式失败！\n");
	return TRUE;
}

void BasicCamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR BasicCamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void BasicCamDlg::OnBnClickedCaptStart()
{
	int errorCode;
	if ((errorCode = CameraPlay(m_hCamera)) != CAMERA_STATUS_SUCCESS) //使相机进入工作模式，并且SDK开始接收相机图像
	{
		AfxMessageBox(_T("开始采集失败！ 错误代码：%d ！"), errorCode);
		return;
	}
	m_bPause = FALSE;
	GetDlgItem(IDC_BTN_CAPTSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CAPTSTOP)->EnableWindow(TRUE);
}

void BasicCamDlg::OnBnClickedBtnCaptStop()
{
	int errorCode;
	if ((errorCode = CameraPause(m_hCamera)) != CAMERA_STATUS_SUCCESS) //使相机进入暂停模式
	{
		AfxMessageBox(_T("停止采集失败！ 错误代码：%d ！"), errorCode);
		return;
	}
	m_bPause = TRUE;
	GetDlgItem(IDC_BTN_CAPTSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CAPTSTOP)->EnableWindow(FALSE);
}

void BasicCamDlg::OnCancel()
{
	AfxMessageBox(_T("请点击[退出程序]按钮关闭！"));
	//OnClose();
	//CDialogEx::OnCancel();
}

void BasicCamDlg::OnClose()
{
	AfxMessageBox(_T("请点击[退出程序]按钮关闭！"));
	//OnClose();
	//CDialogEx::OnCancel();
}

void BasicCamDlg::OnBnClickedBtnQuit()
{
	if (m_hCamera > 0)
	{
		if (NULL != m_hDispThread)
		{
			//等待采集线程结束
			m_bExit = TRUE;
			::WaitForSingleObject(m_hDispThread, INFINITE);
			TRACE(_T("采集线程已终止\n"));
			CloseHandle(m_hDispThread);
			m_hDispThread = NULL;
		}

		//反初始化相机。
		TRACE(_T("相机卸载\n"));
		CameraUnInit(m_hCamera);
		m_hCamera = 0;
	}

	if (m_pFrameBuffer)
	{
		TRACE(_T("采集缓存清空\n"));
		CameraAlignFree(m_pFrameBuffer);
		m_pFrameBuffer = NULL;
	}
	CDialogEx::OnCancel();
}

void BasicCamDlg::OnBnClickedRadioModeCapt()
{
	if (m_iMode == 0)
	{
		AfxMessageBox(_T("已经处于“采集模式”！"));
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
		AfxMessageBox(_T("已经处于“回放模式”！"));
		return;
	}
	if (m_bPause == FALSE)
	{
		((CButton *)GetDlgItem(IDC_RADIO_MODECAPT))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_MODEREVIEW))->SetCheck(0);
		AfxMessageBox(_T("正在采集中，请先[停止采集]！"));
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
