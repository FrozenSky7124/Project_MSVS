
// Demo10Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo10.h"
#include "Demo10Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemo10Dlg �Ի���



CDemo10Dlg::CDemo10Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO10_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// ȫ�ֺ���AfxGetAppȡ��ȫ�ֱ���theApp�ĵ�ַ
	CWinApp *winApp = AfxGetApp();
	// ȫ�ֺ���AfxGetInstanceHandleȡ����Դ���
	HINSTANCE hInstance = AfxGetInstanceHandle();
	HINSTANCE hInstance1 = winApp->m_hInstance;
	// ȫ�ֺ���AfxGetAppNameȡ��AppName
	LPCTSTR appName = AfxGetAppName();
	LPCTSTR appName1 = winApp->m_pszAppName;
	// ȫ�ֺ���AfxGetMainWndȡ�������ھ��
	CWnd *mainWnd = AfxGetMainWnd();
	CWnd *mainWnd1 = winApp->m_pMainWnd;
}

void CDemo10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDemo10Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDemo10Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDemo10Dlg ��Ϣ�������

BOOL CDemo10Dlg::OnInitDialog()
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

void CDemo10Dlg::OnPaint()
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
HCURSOR CDemo10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemo10Dlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnCancel();
}


void CDemo10Dlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


void CDemo10Dlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	EndDialog(IDCANCEL);
	CDialogEx::OnClose();
}


void CDemo10Dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_modelDlg.m_hWnd)
	{
		m_modelDlg.Create(IDD_DIALOG1, CWnd::GetDesktopWindow());
		m_modelDlg.ShowWindow(SW_SHOW);
	}
	m_modelDlg.ShowWindow(SW_SHOW);
}
