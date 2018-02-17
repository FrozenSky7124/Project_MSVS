
// CameraAlphaDlg.cpp : 实现文件
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


/*图像抓取线程，主动调用SDK接口函数获取图像*/
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

	//先清0
	memset(&sFrameInfo, 0, sizeof(tSdkFrameHead));

	while (!pThis->m_bExit)
	{
		if (CameraGetImageBuffer(pThis->m_hCamera, &sFrameInfo, &pbyBuffer, 1000) == CAMERA_STATUS_SUCCESS)
		{
			//保存RAW数据，无需调用CameraImageProcess
			//CameraSaveImage(pThis->m_hCamera, fileName.GetBuffer(1), pbyBuffer, &sFrameInfo, FILE_RAW, 100);
			
			//将获得的原始数据转换成RGB格式的数据，同时经过ISP模块，对图像进行降噪，边沿提升，颜色校正等处理。
			//我公司大部分型号的相机，原始数据都是Bayer格式的
			statusCode = CameraImageProcess(pThis->m_hCamera, pbyBuffer, pThis->m_pFrameBuffer, &sFrameInfo);//连续模式

			//分辨率改变了，则刷新背景
			if (pThis->m_sFrInfo.iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo.iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo.iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo.iHeight = sFrameInfo.iHeight;
				pThis->InvalidateRect(NULL);
			}

			// 处理截图请求
			//pThis->ProcessSnapRequest(pRgbBuffer, &sFrameInfo);

			if (statusCode == CAMERA_STATUS_SUCCESS)
			{
				//调用SDK封装好的显示接口来显示图像,您也可以将m_pFrameBuffer中的RGB数据通过其他方式显示，比如directX,OpengGL,等方式。
				//CameraImageOverlay(pThis->m_hCamera, pThis->m_pFrameBuffer, &sFrameInfo);
				//CameraDisplayRGB24(pThis->m_hCamera, pThis->m_pFrameBuffer, &sFrameInfo);

				//调用CDib类载入缓存数据并显示图像
				
				cDibImage.LoadFromBuffer(pThis->m_pFrameBuffer, sFrameInfo.iWidth, sFrameInfo.iHeight, 8);
				cDibImage.Draw(pThis->m_pCDC, CPoint(0, 0), CSize(tRect.Width(), tRect.Height()));

				if (pThis->m_bSaveFile == TRUE)
				{
					curTime = CTime::GetCurrentTime();
					strTime.Format(_T("_%d%02d%02d%02d%02d%02d_"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
					fileName.Format(_T("%05d.BMP"), iSaveCounts++);
					fileName = pThis->m_csSaveFolder + _T("Image") + strTime + fileName;
					// 保存为24位BMP文件
					//CameraSaveImage(pThis->m_hCamera, fileName.GetBuffer(1), pThis->m_pFrameBuffer, &sFrameInfo, FILE_BMP, 100);
					// 保存为8位BMP文件
					CameraSaveImage(pThis->m_hCamera, fileName.GetBuffer(1), pThis->m_pFrameBuffer, &sFrameInfo, FILE_BMP_8BIT, 100);
					// CDib类 保存图像
					//cDibImage.SaveToFile(fileName.GetBuffer(1));
					fileName.ReleaseBuffer();
				}
			}
			pThis->m_iDispFrameNum++;

			//在成功调用CameraGetImageBuffer后，必须调用CameraReleaseImageBuffer来释放获得的buffer。
			//否则再次调用CameraGetImageBuffer时，程序将被挂起，知道其他线程中调用CameraReleaseImageBuffer来释放了buffer
			CameraReleaseImageBuffer(pThis->m_hCamera, pbyBuffer);

			memcpy(&pThis->m_sFrInfo, &sFrameInfo, sizeof(tSdkFrameHead));
		}
	}
	_endthreadex(0);
	TRACE(_T("图像采集线程退出!"));
	return 0;
}

// 回放线程
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
		bFound = imageFind.FindNextFile(); //首次执行选择搜索到的第一个文件，以后执行将选择下一个文件
		if (imageFind.IsDots()) continue; //过滤.与..文件
		if (imageFind.IsDirectory()) continue; //过滤文件夹
		
		imagePath = imageFind.GetFilePath();
		//显示图像
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
	TRACE(_T("图像回放线程退出!"));
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
		AfxMessageBox(_T("创建配置文件异常！"));
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
		AfxMessageBox(_T("已载入默认配置！"));
		return;
	}
	fin.Read(&tempConfig, sizeof(SConfig));
	fin.Close();
	this->m_bSaveFile = tempConfig.bSaveFile;
	this->m_csSaveFolder = tempConfig.sSaveFolder;
	if (m_bSaveFile == TRUE) ((CComboBox *)GetDlgItem(IDC_COMBO_SAVEFILE))->SetCurSel(0);
	SetDlgItemTextA(IDC_STATIC_SAVEPATH, m_csSaveFolder);
}

// CameraAlphaDlg 对话框

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


// CameraAlphaDlg 消息处理程序

BOOL CameraAlphaDlg::OnInitDialog()
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

	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();
	
	// 控件状态初始化
	HICON m_IconBtn = AfxGetApp()->LoadIcon(IDI_ICON_CONFIG);
	m_BtnConfig.SetIcon(m_IconBtn);

	((CButton *)GetDlgItem(IDC_RADIO_MODECAPT))->SetCheck(1);

	((CComboBox *)GetDlgItem(IDC_COMBO_SAVEFILE))->AddString(_T("保存数据"));
	((CComboBox *)GetDlgItem(IDC_COMBO_SAVEFILE))->AddString(_T("不保存数据"));
	((CComboBox *)GetDlgItem(IDC_COMBO_SAVEFILE))->SetCurSel(1);

	GetDlgItem(IDC_STATIC_SAVEPATH)->SetWindowTextA(m_csSaveFolder);

	//Test

	ConfigRead();

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

BOOL CameraAlphaDlg::InitCamera()
{
	tSdkCameraDevInfo sCameraList[10];
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	tSdkCameraCapbility sCameraInfo;

	//枚举设备，获得设备列表
	iCameraNums = CameraEnumerateDeviceEx();

	iCameraNums = 10;//调用CameraEnumerateDevice前，先设置iCameraNums = 10，表示最多只读取10个设备，如果需要枚举更多的设备，请更改sCameraList数组的大小和iCameraNums的值
	if (CameraEnumerateDevice(sCameraList, &iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
	{
		MessageBox("未找到相机！");
		return FALSE;
	}

	//该示例中，我们只假设连接了一个相机。因此，只初始化第一个相机。(-1,-1)表示加载上次退出前保存的参数，如果是第一次使用该相机，则加载默认参数.
	if ((status = CameraInit(&sCameraList[0], -1, -1, &m_hCamera)) != CAMERA_STATUS_SUCCESS)
	{
		CString msg;
		msg.Format("Failed to init the camera! Error code is %d", status);
		MessageBox(msg + "，原因是" + CameraGetErrorString(status));
		return FALSE;
	}

	//获得该相机的特性描述
	if (CameraGetCapability(m_hCamera, &sCameraInfo) != 0) TRACE("获得该相机的特性描述时出错！\n");

	m_pFrameBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax * 4, 16);
	ASSERT(m_pFrameBuffer);

	//设置为8bit灰度格式，黑白相机可设置为8bit的灰度图像输出，以节约内存存储空间，同时可提高图像算法处理的速度。例如halcon和labview以及opencv都支持8bit的灰度位图直接处理。
	CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8);

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

void CameraAlphaDlg::OnPaint()
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
HCURSOR CameraAlphaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CameraAlphaDlg::OnBnClickedCaptStart()
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

void CameraAlphaDlg::OnBnClickedBtnCaptStop()
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

void CameraAlphaDlg::OnBnClickedRadioModeCapt()
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

void CameraAlphaDlg::OnBnClickedRadioModeReview()
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


void CameraAlphaDlg::OnBnClickedBtnBrowse()
{
	//typedef struct _browseinfo {
	//	HWND hwndOwner;            // 父窗口句柄  
	//	LPCITEMIDLIST pidlRoot;    // 要显示的文件目录对话框的根(Root)  
	//	LPTSTR pszDisplayName;     // 保存被选取的文件夹路径的缓冲区  
	//	LPCTSTR lpszTitle;         // 显示位于对话框左上部的标题  
	//	UINT ulFlags;              // 指定对话框的外观和功能的标志  
	//	BFFCALLBACK lpfn;          // 处理事件的回调函数  
	//	LPARAM lParam;             // 应用程序传给回调函数的参数  
	//	int iImage;                // 文件夹对话框的图片索引  
	//} BROWSEINFO, *PBROWSEINFO, *LPBROWSEINFO
	TCHAR acDir[MAX_PATH] = { 0 };
	BROWSEINFO browInfo;
	LPITEMIDLIST lpidlBrowse;

	memset(&browInfo, 0, sizeof(BROWSEINFO));

	browInfo.hwndOwner = this->m_hWnd;
	browInfo.pidlRoot = NULL;
	browInfo.lpszTitle = _T("请选择图像存储的文件夹：");
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
	AfxMessageBox(_T("请点击[退出程序]按钮关闭！"));
	//OnClose();
	//CDialogEx::OnCancel();
}

void CameraAlphaDlg::OnClose()
{
	AfxMessageBox(_T("请点击[退出程序]按钮关闭！"));
	//OnClose();
	//CDialogEx::OnCancel();
}

void CameraAlphaDlg::OnBnClickedBtnQuit()
{
	ConfigSave(); //保存配置
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

void CameraAlphaDlg::OnBnClickedBtnOpenfile()
{
	TCHAR acDir[MAX_PATH] = { 0 };
	BROWSEINFO browInfo;
	LPITEMIDLIST lpidlBrowse;

	memset(&browInfo, 0, sizeof(BROWSEINFO));

	browInfo.hwndOwner = this->m_hWnd;
	browInfo.pidlRoot = NULL;
	browInfo.lpszTitle = _T("请选择回放图像的文件夹：");
	browInfo.pszDisplayName = acDir;
	browInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	browInfo.lpfn = NULL;
	browInfo.lParam = 0;
	browInfo.iImage = 0;

	lpidlBrowse = ::SHBrowseForFolder(&browInfo);
	if (lpidlBrowse == NULL) return;
	if (!::SHGetPathFromIDList(lpidlBrowse, acDir)) return;

	m_csReviewFolder = acDir;

	// 遍历回放目录，存储回放文件的文件名信息
	CFileFind tFileFind;
	BOOL bFound;
	UINT iFileCount = 0;

	bFound = tFileFind.FindFile(m_csReviewFolder + _T("\\*.BMP"));
	while (bFound)
	{
		bFound = tFileFind.FindNextFile(); //首次执行选择搜索到的第一个文件，以后执行将选择下一个文件
		if (tFileFind.IsDots()) continue; //过滤.与..文件
		if (tFileFind.IsDirectory()) continue; //过滤文件夹
		iFileCount++;
	}

	TRACE(_T("Find: %d files.\n"), iFileCount);
}


void CameraAlphaDlg::OnBnClickedBtnReviewStart()
{
	if (m_csReviewFolder == CString(_T("")))
	{
		AfxMessageBox(_T("未设置回放目录！"));
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
