
// QNotepadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo09.h"
#include "QNotepadDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// QNotepadDlg �Ի���



QNotepadDlg::QNotepadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO09_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void QNotepadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(QNotepadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &QNotepadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &QNotepadDlg::OnBnClickedCancel)
	ON_COMMAND(ID_APP_EXIT, &QNotepadDlg::OnAppExit)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_HELPMENU_ABOUT, &QNotepadDlg::OnHelpmenuAbout)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_EDITMENU_SELECTALL, &QNotepadDlg::OnEditmenuSelectall)
	ON_COMMAND(ID_EDITMENU_DATETIME, &QNotepadDlg::OnEditmenuDatetime)
	ON_COMMAND(ID_FORMATMENU_FONT, &QNotepadDlg::OnFormatmenuFont)
END_MESSAGE_MAP()


// QNotepadDlg ��Ϣ�������

BOOL QNotepadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect rect = NULL;
	CWnd *mainEdit = GetDlgItem(IDC_EDIT_MAIN);
	GetClientRect(rect);
	mainEdit->MoveWindow(rect);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void QNotepadDlg::OnPaint()
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
HCURSOR QNotepadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void QNotepadDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void QNotepadDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}


void QNotepadDlg::OnAppExit()
{
	// TODO: �ڴ���������������
	EndDialog(ID_APP_EXIT);
}

// ANSI�����ȡ����
void QNotepadDlg::FileRead_ANSI(CFile& file)
{
	file.Seek(0, CFile::begin);
	UINT fileLen = (UINT)file.GetLength();
	CHAR *fileBuff = new CHAR[fileLen + 1];
	memset(fileBuff, 0, sizeof(CHAR)*(fileLen + 1));
	file.Read(fileBuff, fileLen);
	fileBuff[fileLen] = '\0';
	CString tempStr(fileBuff);
	SetDlgItemText(IDC_EDIT_MAIN, tempStr);
	delete[]fileBuff;
}

// UTF-8�����ȡ����
void QNotepadDlg::FileRead_UTF8(CFile& file)
{
	file.Seek(3, CFile::begin);
	UINT fileLen = (UINT)file.GetLength();
	CHAR *fileBuff = new CHAR[fileLen + 1];
	memset(fileBuff, 0, sizeof(CHAR)*(fileLen + 1));
	file.Read(fileBuff, fileLen);
	fileBuff[fileLen] = '\0';
	TCHAR *tFileBuff = new TCHAR[fileLen / 2 + 2];
	MultiByteToWideChar(CP_UTF8, NULL, fileBuff, -1, tFileBuff, fileLen / 2 + 2);
	SetDlgItemText(IDC_EDIT_MAIN, tFileBuff);
	delete[]tFileBuff;
	delete[]fileBuff;
}

// UTF-16�����ȡ����
void QNotepadDlg::FileRead_UTF16(CFile& file)
{
	file.Seek(2, CFile::begin);
	UINT fileLen = (UINT)file.GetLength();
	TCHAR *tFileBuff = new TCHAR[fileLen / 2 + 1];
	fileLen = file.Read(tFileBuff, fileLen);
	tFileBuff[fileLen / 2] = _T('\0');
	SetDlgItemText(IDC_EDIT_MAIN, tFileBuff);
	delete[]tFileBuff;
}

// �Ϸ��ļ���Ϣ������
void QNotepadDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	TRACE(_T("DragFileCount: %d\n"), nFileCount);
	/*
	TCHAR filePath[256];
	int i = 0;
	while (i < nFileCount)
	{
		DragQueryFile(hDropInfo, i, filePath, _countof(filePath));
		TRACE(_T("File %d: %s\n"), i, filePath);
		i++;
	}
	*/

	// ��ȡ������ļ�·��
	TCHAR filePath[256];
	DragQueryFile(hDropInfo, 0, filePath, _countof(filePath));
	TRACE(_T("File %d: %s\n"), 0, filePath);
	// ���ļ�
	CFile file;
	if (!file.Open(filePath, CFile::modeRead))
	{
		CString tip;
		tip.Format(_T("�޷����ļ�%s !"), filePath);
		AfxMessageBox(tip);
		return;
	}
	// �ж��ļ����룬���ö�Ӧ���ļ���ȡ���������ļ���ʾ���ı����С�
	CHAR fileMod[3];
	file.Read(&fileMod, 3);
	if ((fileMod[0] == (CHAR)0xEF) && (fileMod[1] == (CHAR)0xBB) && (fileMod[2] == (CHAR)0xBF))
	{
		// UTF-8
		TRACE(_T("UTF-8 Procedure.\n"));
		FileRead_UTF8(file);
	}
	else if ((fileMod[0] == (CHAR)0xFE) && (fileMod[1] == (CHAR)0xFF))
	{
		// UTF-16
		TRACE(_T("UTF-16 Procedure.\n"));
		FileRead_UTF16(file);
	}
	else
	{
		// ANSI
		TRACE(_T("ANSI Procedure.\n"));
		FileRead_ANSI(file);
	}
	file.Close();
	CDialogEx::OnDropFiles(hDropInfo);
}

// ����-���� �˵���Ӧ����
void QNotepadDlg::OnHelpmenuAbout()
{
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// �ı䴰�ڴ�Сʱ����Ϣ������
void QNotepadDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rect = NULL;
	CWnd *mainEdit = GetDlgItem(IDC_EDIT_MAIN);
	if (mainEdit)
	{
		GetClientRect(rect);
		mainEdit->MoveWindow(rect);
	}
}


void QNotepadDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	EndDialog(IDCANCEL);
	CDialogEx::OnClose();
}


void QNotepadDlg::OnEditmenuSelectall()
{
	// TODO: 
	CEdit *mainEdit = (CEdit *)GetDlgItem(IDC_EDIT_MAIN);
	mainEdit->SetSel(0, -1);
}


void QNotepadDlg::OnEditmenuDatetime()
{
	// TODO: 
	CEdit *mainEdit = (CEdit *)GetDlgItem(IDC_EDIT_MAIN);
	COleDateTime dateTime = COleDateTime::GetCurrentTime();
	CString tempStr;
	tempStr = dateTime.Format(_T("%Y��%m��%d�� %A %H:%M:%S"));
	mainEdit->ReplaceSel(tempStr);
}


void QNotepadDlg::OnFormatmenuFont()
{
	// TODO:
	LOGFONT lf;
	CFont *pTempFont = NULL;
	if (m_cFont.GetSafeHandle())
		pTempFont = &m_cFont;
	else
		pTempFont = CWnd::GetFont();
	pTempFont->GetLogFont(&lf);
	CFontDialog fontDlg(&lf);
	if (fontDlg.DoModal() == IDCANCEL)
		return;
	fontDlg.GetCurrentFont(&lf);
	this->m_cFont.DeleteObject();
	this->m_cFont.CreateFontIndirect(&lf);
	GetDlgItem(IDC_EDIT_MAIN)->SetFont(&this->m_cFont);
}
