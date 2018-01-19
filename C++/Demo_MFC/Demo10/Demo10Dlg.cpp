
// Demo10Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo10.h"
#include "Demo10Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemo10Dlg 对话框



CDemo10Dlg::CDemo10Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO10_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 全局函数AfxGetApp取出全局变量theApp的地址
	CWinApp *winApp = AfxGetApp();
	// 全局函数AfxGetInstanceHandle取出资源句柄
	HINSTANCE hInstance = AfxGetInstanceHandle();
	HINSTANCE hInstance1 = winApp->m_hInstance;
	// 全局函数AfxGetAppName取出AppName
	LPCTSTR appName = AfxGetAppName();
	LPCTSTR appName1 = winApp->m_pszAppName;
	// 全局函数AfxGetMainWnd取出主窗口句柄
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


// CDemo10Dlg 消息处理程序

BOOL CDemo10Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDemo10Dlg::OnPaint()
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
HCURSOR CDemo10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemo10Dlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}


void CDemo10Dlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CDemo10Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	EndDialog(IDCANCEL);
	CDialogEx::OnClose();
}


void CDemo10Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_modelDlg.m_hWnd)
	{
		m_modelDlg.Create(IDD_DIALOG1, CWnd::GetDesktopWindow());
		m_modelDlg.ShowWindow(SW_SHOW);
	}
	m_modelDlg.ShowWindow(SW_SHOW);
}
