
// CDibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo12_CDib.h"
#include "CDibDlg.h"
#include "afxdialogex.h"
#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDibDlg 对话框



CDibDlg::CDibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO12_CDIB_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDibDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CDibDlg::OnBnClickedButtonShow)
END_MESSAGE_MAP()


// CDibDlg 消息处理程序

BOOL CDibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDibDlg::OnPaint()
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
HCURSOR CDibDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDibDlg::OnBnClickedButtonShow()
{
	CDib tDib;
	CRect tRect;
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&tRect);
	tDib.LoadFromFile(_T("F:\\玉雪·幻境云图\\Wallpaper\\TEMP.bmp"));
	tDib.Draw(m_pCDC, CPoint(0, 0), CSize(tRect.Width(), tRect.Height()));
	return;
}
