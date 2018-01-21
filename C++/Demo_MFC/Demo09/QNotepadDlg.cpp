
// QNotepadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo09.h"
#include "QNotepadDlg.h"
#include "AboutDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// QNotepadDlg 对话框



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


// QNotepadDlg 消息处理程序

BOOL QNotepadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect rect = NULL;
	CWnd *mainEdit = GetDlgItem(IDC_EDIT_MAIN);
	GetClientRect(rect);
	mainEdit->MoveWindow(rect);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void QNotepadDlg::OnPaint()
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
HCURSOR QNotepadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void QNotepadDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void QNotepadDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void QNotepadDlg::OnAppExit()
{
	// TODO: 在此添加命令处理程序代码
	EndDialog(ID_APP_EXIT);
}

// ANSI编码读取函数
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

// UTF-8编码读取函数
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

// UTF-16编码读取函数
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

// 拖放文件消息处理函数
void QNotepadDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	// 获取拖入的文件路径
	TCHAR filePath[256];
	DragQueryFile(hDropInfo, 0, filePath, _countof(filePath));
	TRACE(_T("File %d: %s\n"), 0, filePath);
	// 打开文件
	CFile file;
	if (!file.Open(filePath, CFile::modeRead))
	{
		CString tip;
		tip.Format(_T("无法打开文件%s !"), filePath);
		AfxMessageBox(tip);
		return;
	}
	// 判断文件编码，调用对应的文件读取函数，将文件显示在文本框中。
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

// 帮助-关于 菜单响应函数
void QNotepadDlg::OnHelpmenuAbout()
{
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// 改变窗口大小时的消息处理函数
void QNotepadDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	tempStr = dateTime.Format(_T("%Y年%m月%d日 %A %H:%M:%S"));
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
