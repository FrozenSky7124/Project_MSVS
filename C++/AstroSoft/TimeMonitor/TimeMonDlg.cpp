
// TimeMonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TimeMonitor.h"
#include "TimeMonDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Global

bool CompareFitsTime(CString & cs1, CString & cs2)
{
	CString sub1 = cs1.Right(21);
	CString sub2 = cs2.Right(21);
	long long time1 = atoll(sub1);
	long long time2 = atoll(sub2);
	if (time1 > time2)
		return false;
	else
		return true;
}


UINT WINAPI uiFunc_Proc(LPVOID lpParam)
{
	TimeMonDlg* pIPLDLG = (TimeMonDlg*)lpParam;
	std::vector<CString>* lpvFitsName = &(pIPLDLG->m_vFitsName);
	int & iImgIndex = pIPLDLG->m_iImgIndex;

	for (; iImgIndex < lpvFitsName->size(); iImgIndex++)
	{
		pIPLDLG->m_Progress.SetRange32(0, lpvFitsName->size());
		pIPLDLG->m_Progress.SetPos(iImgIndex);

		// Load Fits
		CString csFitsPath = pIPLDLG->m_csFitsDir + _T("\\") + lpvFitsName->at(iImgIndex);
		pIPLDLG->Proc_CheckTime(csFitsPath);

		// Stop and Release when ThreadStatus is 0;
		if (pIPLDLG->m_iThreadStatus == 0) break;
	}
	// Proc End.
	if (iImgIndex == lpvFitsName->size())
	{
		pIPLDLG->m_iThreadStatus = 0;
		//Set BtnSTStart to START
		//pIPLDLG->m_BtnSTStart.SetIcon(IDI_ICON_START, 48, 48);
		MessageBoxEx(pIPLDLG->GetSafeHwnd(), _T("Auto Process Over!"), _T("QwQ"), MB_OK | MB_ICONINFORMATION, 0);
	}
	return 0;
}


// TimeMonDlg 对话框



TimeMonDlg::TimeMonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TIMEMONITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void TimeMonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}

BEGIN_MESSAGE_MAP(TimeMonDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnStart, &TimeMonDlg::OnBnClickedBtnstart)
END_MESSAGE_MAP()


// TimeMonDlg 消息处理程序

BOOL TimeMonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_iThreadStatus = 0;
	m_iTimeErrCount = 0;
	m_iFitsCount    = 0;
	m_iImgIndex     = 0;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void TimeMonDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR TimeMonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void TimeMonDlg::OnBnClickedBtnstart()
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

	m_vFitsName.clear();

	// If Process be Stop, init m_csFitsDir, init MarkPos, reset ProcErr count.
	if (m_iThreadStatus == 0)
	{
		// Init BROWSEINFO
		TCHAR acDir[MAX_PATH] = { 0 };
		BROWSEINFO browInfo;
		LPITEMIDLIST lpidlBrowse;

		memset(&browInfo, 0, sizeof(BROWSEINFO));
		browInfo.hwndOwner = this->m_hWnd;
		browInfo.pidlRoot = NULL;
		browInfo.lpszTitle = _T("Please choose folder:");
		browInfo.pszDisplayName = acDir;
		browInfo.ulFlags = BIF_RETURNONLYFSDIRS;
		browInfo.lpfn = NULL;
		browInfo.lParam = 0;
		browInfo.iImage = 0;

		lpidlBrowse = ::SHBrowseForFolder(&browInfo);
		if (lpidlBrowse == NULL) return;
		if (!::SHGetPathFromIDList(lpidlBrowse, acDir)) return;
		// Get fits folder path
		m_csFitsDir = acDir;
		// Reset ProcErr count
		m_iTimeErrCount = 0;
	}
	// Create Output file directory
	CString csOutputPath = m_csFitsDir;

	// If Process be Pause, reload fits directory.

	// Get fits file count
	CFileFind fileFinder;
	bool bFound;
	int iFileCount = 0;

	bFound = fileFinder.FindFile(m_csFitsDir + _T("\\*.fit"));
	while (bFound)
	{
		bFound = fileFinder.FindNextFile();
		if (fileFinder.IsDots()) continue; //filter . and ..
		if (fileFinder.IsDirectory()) continue; //filter directory
		if (fileFinder.IsSystem()) continue; //filter system file
		if (fileFinder.IsHidden()) continue; //filter hidden file
		m_vFitsName.push_back(fileFinder.GetFileName());
		iFileCount++;
	}
	fileFinder.Close();

	m_iFitsCount = iFileCount;
	if (m_iFitsCount == 0) //Find no file
	{
		MessageBoxEx(GetSafeHwnd(), _T("Can not find Fits files. Terminated."), _T("QAQ"), MB_OK | MB_ICONERROR, 0);
		return;
	}
	// Sort fits file by OBS-TIME
	std::sort(m_vFitsName.begin(), m_vFitsName.end(), CompareFitsTime);

	TRACE(_T("m_csFitsDir = %s\n"), m_csFitsDir);
	TRACE(_T("m_iFitsCount = %d\n"), m_iFitsCount);
	CString csTmp;
	csTmp.Format(_T("FITS Directory: %s"), m_csFitsDir);

	// Start processing thread
	UINT uiThread_Proc;
	m_hThread_Proc = (HANDLE)_beginthreadex(NULL, 0, &uiFunc_Proc, this, 0, &uiThread_Proc);
	if (m_hThread_Proc != NULL)
	{
		m_iThreadStatus = 1; //Set thread status Running
		//m_BtnSTStart.SetIcon(IDI_ICON_PAUSE, 48, 48); //Set BtnSTStart to PAUSE
		//PrintLog(_T("Auto Process Start."));
	}
	ASSERT(m_hThread_Proc);
	SetThreadPriority(m_hThread_Proc, THREAD_PRIORITY_HIGHEST);
}


void TimeMonDlg::Proc_CheckTime(CString csPath)
{
	bool bTimeErr;
	CString csLog;
	bTimeErr = m_FSCFitsX.OpenFitsFileToCheckTime(csPath, csLog);
	TRACE(_T("%s"), csLog);
	if (bTimeErr == false)
		MessageBoxEx(GetSafeHwnd(), _T("Time Error!"), _T("QAQ"), MB_OK | MB_ICONERROR, 0);
}
