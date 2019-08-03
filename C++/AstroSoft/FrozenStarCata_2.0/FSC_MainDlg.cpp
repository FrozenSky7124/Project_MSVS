
// FSC_MainDlg.cpp : ʵ���ļ�
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
	//CStdioFile fileStarCata;
	//if (!fileStarCata.Open(strFilePath, CFile::modeRead))
	//{
	//	MessageBoxEx(pMainDlg->GetSafeHwnd(), _T("Faild to open Star Catalogue file!"), _T("QAQ"), MB_ICONERROR, NULL);
	//	return -2;
	//}

	CFile fileStarCata;
	if (!fileStarCata.Open(strFilePath, CFile::modeRead))
	{
		MessageBoxEx(pMainDlg->GetSafeHwnd(), _T("Faild to open Star Catalogue file!"), _T("QAQ"), MB_ICONERROR, NULL);
		return -2;
	}
	int fileLength = (int)fileStarCata.GetLength();
	int iCount = fileLength / 208;
	pMainDlg->m_MainProgBar.SetRange32(0, iCount);
	char* pData = new char[fileLength];
	fileStarCata.Read(pData, fileLength);
	fileStarCata.Close();

	// Open Database
	sqlite3 *Sqlite3_dbCoon;
	char *zErrMsg = 0;
	char *sql;
	int rc;

	rc = sqlite3_open("Tycho2_VT12_DEC-50+_Fix.db", &Sqlite3_dbCoon);

	if (rc)
	{
		MessageBoxEx(pMainDlg->GetSafeHwnd(), _T("Can't open Tycho2 database."), _T("QAQ"), MB_OK | MB_ICONERROR, 0);
		return -1;
	}
	else
	{
		TRACE(_T("Open database successfully.\n"));
	}

	// Create TABLE
	//sql = "CREATE TABLE Tycho2_all(StarID varchar(15), RA numeric(12,8), DEC numeric(12,8), VT numeric(6,3))";
	sql = "CREATE TABLE Tycho2_all(StarID varchar(15), RA numeric(12,8), DEC numeric(12,8), pmRA numeric(7,1), pmDE numeric(7,1), VT numeric(6,3))";
	rc = sqlite3_exec(Sqlite3_dbCoon, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		TRACE(_T("SQL_EXEC Error: %s\n"), zErrMsg);
		sqlite3_free(zErrMsg);
	}

	// Transaction BEGIN
	sqlite3_exec(Sqlite3_dbCoon, "BEGIN;", 0, 0, 0);

	CString strRead = _T("");
	for (int i = 0; i < iCount; i++)
	{
		char lineBuffer[208];
		memcpy_s(&lineBuffer, 208, pData + 208 * i, 208);
		strRead.ReleaseBuffer();
		strRead = lineBuffer;

		CString strID, strmRA, strmDE, strpmRA, strpmDE, strVT;

		strID = strRead.Mid(0, 12);
		strmRA = strRead.Mid(15, 12);
		strmDE = strRead.Mid(28, 12);
		strpmRA = strRead.Mid(41, 7);
		strpmDE = strRead.Mid(49, 7);
		strVT = strRead.Mid(123, 6);
		
		pMainDlg->m_iCurMakeNo = i;
		
		// Filter DEC: Stars above China
		if (atof(strmDE) < -50.) continue;
		// Filter VT: 1.905 ~ 9.0
		if (-1 == strVT.Find(_T("."))) continue;
		double dVT = atof(strVT);
		//if (dVT < 1.90 || dVT > 12.0) continue;
		// Filter mRA and mDE exclude empty value
		if (-1 == strmRA.Find(_T("."))) continue;
		if (-1 == strmDE.Find(_T("."))) continue;
		// Filter pmRA and pmDE exclude empty value
		if (-1 == strpmRA.Find(_T("."))) continue;
		if (-1 == strpmDE.Find(_T("."))) continue;
		// Filter pmRA and pmDE exclude large value
		double dpmRA = atof(strpmRA);
		double dpmDE = atof(strpmDE);
		if (dpmRA > 10.0 || dpmDE > 10.0 || dpmRA < -10.0 || dpmDE < -10.0) continue;

		// Insert into database
		CString szQuery;
		szQuery.Format(_T("INSERT INTO Tycho2_all VALUES ('%s', %.8f, %.8f, %.1f, %.1f, %.3f)"), strID, atof(strmRA), atof(strmDE), dpmRA, dpmDE, dVT);
		rc = sqlite3_exec(Sqlite3_dbCoon, szQuery, NULL, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			TRACE(_T("SQL_EXEC Error: %s\n"), zErrMsg);
			sqlite3_free(zErrMsg);
		}
	}
	// Transaction COMMIT
	sqlite3_exec(Sqlite3_dbCoon, "COMMIT;", 0, 0, 0);
	// Close Database
	sqlite3_close(Sqlite3_dbCoon);

	//pMainList->InsertItem(pMainDlg->m_iCurMakeNo, strID);
	//pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 1, strmRA);
	//pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 2, strmDE);
	//pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 3, strpmRA);
	//pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 4, strpmDE);
	//pMainList->SetItemText(pMainDlg->m_iCurMakeNo, 5, strVT);
	
	delete[] pData;
	MessageBoxEx(pMainDlg->GetSafeHwnd(), _T("Star Catalogue Making Success!"), _T("QwQ"), MB_ICONINFORMATION, NULL);
	return 0;
}


// FSC_MainDlg �Ի���

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


// FSC_MainDlg ��Ϣ�������

BOOL FSC_MainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	InitUI();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void FSC_MainDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	m_MainList.InsertColumn(0, _T("TYC���Ǳ��"), LVCFMT_LEFT, 110);
	m_MainList.InsertColumn(1, _T("ƽ�ྭ mRA"), LVCFMT_LEFT, 100);
	m_MainList.InsertColumn(2, _T("ƽ��γ mDE"), LVCFMT_LEFT, 100);
	m_MainList.InsertColumn(3, _T("�ྭ���� pmRA"), LVCFMT_LEFT, 110);
	m_MainList.InsertColumn(4, _T("��γ���� pmDE"), LVCFMT_LEFT, 110);
	m_MainList.InsertColumn(5, _T("�ǵ� VT"), LVCFMT_LEFT, 60);
	m_MainList.SetBkColor(RGB(205, 226, 252));
	m_MainList.SetTextBkColor(RGB(205, 226, 252));
	m_MainList.SetExtendedStyle(m_MainList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_MainProgBar.SetRange32(0, 2539913);

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
	sqlite3 *Sqlite3_dbCoon;
	char *zErrMsg = 0;
	char *sql;
	int rc;

	rc = sqlite3_open("Tycho2_VT9_DEC-50+.db", &Sqlite3_dbCoon);

	if (rc)
	{
		MessageBoxEx(GetSafeHwnd(), _T("Can't open Tycho2 database."), _T("QAQ"), MB_OK | MB_ICONERROR, 0);
		return;
	}
	else
	{
		TRACE(_T("Open database successfully.\n"));
	}

	// Create TABLE
	sql = "CREATE TABLE Tycho2_all(StarID varchar(15), RA numeric(12,8), DEC numeric(12,8), VT numeric(6,3))";
	rc = sqlite3_exec(Sqlite3_dbCoon, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		TRACE(_T("SQL_EXEC Error: %s\n"), zErrMsg);
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(Sqlite3_dbCoon);
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
