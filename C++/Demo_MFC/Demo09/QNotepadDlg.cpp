
// QNotepadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo09.h"
#include "QNotepadDlg.h"
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
	EndDialog(ID_APP_EXIT);
}


void QNotepadDlg::OnAppExit()
{
	// TODO: �ڴ���������������
	EndDialog(ID_APP_EXIT);
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
	CDialogEx::OnDropFiles(hDropInfo);
}
