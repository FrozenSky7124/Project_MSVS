
// TimeMonDlg.cpp : ʵ���ļ�
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


// TimeMonDlg �Ի���



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


// TimeMonDlg ��Ϣ�������

BOOL TimeMonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_iThreadStatus = 0;
	m_iTimeErrCount = 0;
	m_iFitsCount    = 0;
	m_iImgIndex     = 0;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void TimeMonDlg::OnPaint()
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
HCURSOR TimeMonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void TimeMonDlg::OnBnClickedBtnstart()
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
