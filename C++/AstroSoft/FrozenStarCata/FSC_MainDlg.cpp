
// FSC_MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FrozenStarCata.h"
#include "FSC_MainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UINT WINAPI uiProcFuncMake(LPVOID lpParam)
{
	FSC_MainDlg* pMainDlg = static_cast<FSC_MainDlg*>(lpParam);
	CListCtrl* pMainList = static_cast<CListCtrl*>(&(pMainDlg->m_MainList));

	// Read Star Catalogue
	CString strFilePath;
	pMainDlg->GetDlgItemText(IDC_EditFilePath, strFilePath);
	if (!PathFileExists(strFilePath))
	{
		MessageBoxEx(pMainDlg->GetSafeHwnd(), _T("Star Catalogue file not found!"), _T("QAQ"), MB_ICONERROR, NULL);
		return -1;
	}
	CStdioFile fileStarCata;
	if (!fileStarCata.Open(strFilePath, CFile::modeRead))
	{
		MessageBoxEx(pMainDlg->GetSafeHwnd(), _T("Faild to open Star Catalogue file!"), _T("QAQ"), MB_ICONERROR, NULL);
		return -2;
	}
	CString strRead = _T("");

	pMainDlg->m_iCurMakeNo = 0;
	while (fileStarCata.ReadString(strRead) && pMainDlg->m_iCurMakeNo < 10)
	{
		CString strID, strmRA, strmDE, strpmRA, strpmDE, strVT;
		strID = strRead.Mid(0, 12);
		strmRA = strRead.Mid(15, 12);
		strmDE = strRead.Mid(28, 12);
		strpmRA = strRead.Mid(41, 7);
		strpmDE = strRead.Mid(49, 7);
		strVT = strRead.Mid(123, 6);

		pMainList->InsertItem(pMainDlg->m_iCurMakeNo, strID);
		pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 1, strmRA);
		pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 2, strmDE);
		pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 3, strpmRA);
		pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 4, strpmDE);
		pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 5, strVT);
		
		pMainDlg->m_iCurMakeNo++;
	}

	fileStarCata.Close();
	return 0;
}


// FSC_MainDlg 对话框

FSC_MainDlg::FSC_MainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MainDlg, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void FSC_MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_MainList);
	DDX_Control(pDX, IDC_ProgMain, m_MainProgBar);
}

BEGIN_MESSAGE_MAP(FSC_MainDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnCreateDB, &FSC_MainDlg::OnBnClickedBtnCreateDB)
	ON_BN_CLICKED(IDC_BtnSelectFile, &FSC_MainDlg::OnBnClickedBtnSelectFile)
	ON_BN_CLICKED(IDC_BtnMake, &FSC_MainDlg::OnBnClickedBtnMake)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// FSC_MainDlg 消息处理程序

BOOL FSC_MainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	InitUI();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void FSC_MainDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR FSC_MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void FSC_MainDlg::InitUI()
{
	// Font
	m_FontYaHeiConsolas_S.CreatePointFont(90, _T("YaHei Consolas Hybrid"));
	
	// UI
	m_MainList.SetFont(&m_FontYaHeiConsolas_S, FALSE);
	m_MainList.InsertColumn(0, _T("TYC恒星编号"), LVCFMT_LEFT, 110);
	m_MainList.InsertColumn(1, _T("平赤经 mRA"), LVCFMT_LEFT, 100);
	m_MainList.InsertColumn(2, _T("平赤纬 mDE"), LVCFMT_LEFT, 100);
	m_MainList.InsertColumn(3, _T("赤经自行 pmRA"), LVCFMT_LEFT, 110);
	m_MainList.InsertColumn(4, _T("赤纬自行 pmDE"), LVCFMT_LEFT, 110);
	m_MainList.InsertColumn(5, _T("星等 VT"), LVCFMT_LEFT, 60);
	m_MainList.SetBkColor(RGB(205, 226, 252));
	m_MainList.SetTextBkColor(RGB(205, 226, 252));
	m_MainList.SetExtendedStyle(m_MainList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_MainProgBar.SetRange(0, 10);

	// UI Proc Timer
	SetTimer(1, 300, NULL);
}


void FSC_MainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:
	switch (nIDEvent)
	{
	case 1: // UI Proc Timer
		m_MainProgBar.SetPos(m_iCurMakeNo);
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


void FSC_MainDlg::OnBnClickedBtnCreateDB()
{
	// TODO:
	bool fTest;
	CDbSQLite sqlite;
	// Open Sqlite database
	fTest = sqlite.Open(_T("theTestSqlite.db"));
	if (!fTest)
	{
		AfxMessageBox(_T("Error in sqlite.Open()"));
		return;
	}
	// Create TABLE
	fTest = sqlite.DirectStatement(_T("CREATE TABLE usersInfo(name varchar(30), password varchar(20))"));
	if (!fTest)
		AfxMessageBox(_T("Error in CREATE TABLE"));
	// INSERT
	CString szQuery;
	szQuery = _T("INSERT INTO usersInfo VALUES ('xiaoWang','123')");
	fTest = sqlite.DirectStatement(szQuery);
	if (!fTest)
		AfxMessageBox(_T("Error in INSERT"));
	szQuery = _T("INSERT INTO usersInfo VALUES ('daWang','322')");
	fTest = sqlite.DirectStatement(szQuery);
	if (!fTest)
		AfxMessageBox(_T("Error in INSERT"));
	AfxMessageBox(_T("Success!"));
}


void FSC_MainDlg::OnBnClickedBtnSelectFile()
{
	// TODO:
	CString strFilePath;
	CFileDialog selectFileDlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);
	if (IDOK == selectFileDlg.DoModal())
	{
		strFilePath = selectFileDlg.GetPathName();
		SetDlgItemText(IDC_EditFilePath, strFilePath);
	}
}


void FSC_MainDlg::OnBnClickedBtnMake()
{
	// Create Make Thread	
	m_hMakeThread = (HANDLE)_beginthreadex(NULL, 0, &uiProcFuncMake, this, 0, &m_uiMakeThreadID);
	ASSERT(m_uiMakeThreadID);
	SetThreadPriority(m_hMakeThread, THREAD_PRIORITY_HIGHEST);

	
}
