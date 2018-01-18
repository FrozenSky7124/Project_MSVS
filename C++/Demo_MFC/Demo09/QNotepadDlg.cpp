
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

// ANSI��ʽ��ȡ����
void QNotepadDlg::FileRead_ANSI(CFile& file)
{
	UINT fileLen = file.GetLength();
	CHAR *fileBuff = new CHAR[fileLen + 1];
	memset(fileBuff, 0, sizeof(CHAR)*(fileLen + 1));
	file.Read(fileBuff, fileLen);
	fileBuff[fileLen] = '\0';
	CString tempStr(fileBuff);
	SetDlgItemText(IDC_EDIT_MAIN, tempStr);
	delete []fileBuff;
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
	// �����ļ���ȡ���������ļ���ʾ���ı�����
	FileRead_ANSI(file);
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
