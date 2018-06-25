
// GDI_MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo17_GDI.h"
#include "GDI_MainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// GDI_MainDlg 对话框



GDI_MainDlg::GDI_MainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO17_GDI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void GDI_MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(GDI_MainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnTest, &GDI_MainDlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// GDI_MainDlg 消息处理程序

BOOL GDI_MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hBmpBackground = NULL;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void GDI_MainDlg::OnPaint()
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
		HWND hMainWnd = GetSafeHwnd();
		HDC hMainDC = ::GetDC(hMainWnd);
		// 背景图片
		HDC hMemDC = ::CreateCompatibleDC(hMainDC);
		m_hBmpBackground = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BmpYYS));
		HBITMAP hOldBmp = static_cast<HBITMAP>(::SelectObject(hMemDC, m_hBmpBackground));
		BITMAP bmp;
		::GetObject(m_hBmpBackground, sizeof(BITMAP), &bmp);
		RECT rcClient;
		::GetClientRect(hMainWnd, &rcClient);
		int iWidth = rcClient.right - rcClient.left;
		int iHeight = rcClient.bottom - rcClient.top;
		//::BitBlt(hMainDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
		StretchBlt(hMainDC, 0, 0, iWidth, iHeight, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		::SelectObject(hMemDC, hOldBmp);
		::DeleteDC(hMemDC);
		// 文本绘制
		LOGFONT logFont;
		memset(&logFont, 0, sizeof(LOGFONT));
		logFont.lfWidth = 20;
		logFont.lfHeight = 40;
		logFont.lfCharSet = GB2312_CHARSET;
		_tcscpy_s(logFont.lfFaceName, _T("YaHei Consolas Hybrid"));
		HFONT hFont = CreateFontIndirect(&logFont);
		HFONT hOldFont = (HFONT)::SelectObject(hMainDC, hFont);
		COLORREF clrOldText = ::SetTextColor(hMainDC, RGB(0, 0, 128));
		::SetBkMode(hMainDC, TRANSPARENT);
		CString csTitle = _T("FrozenSky7124.GitHub.io");
		DrawText(hMainDC, csTitle, csTitle.GetLength(), &rcClient, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
		::SetTextColor(hMainDC, clrOldText);
		::SelectObject(hMainDC, hOldFont);
		::DeleteObject(hFont);

		//画线
		HPEN hPen = CreatePen(PS_DOT, 1, RGB(42, 178, 203));
		HPEN hOldPen = (HPEN)::SelectObject(hMainDC, hPen);
		MoveToEx(hMainDC, 50, 50, NULL);
		LineTo(hMainDC, 200, 300);
		::SelectObject(hMainDC, hOldPen);
		::DeleteObject(hPen);
		
		::ReleaseDC(hMainWnd, hMainDC);
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR GDI_MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void GDI_MainDlg::OnBnClickedBtnTest()
{
	HWND hMainWnd = GetSafeHwnd();
	//HDC hMainDC = ::GetDC(hMainWnd);
	//// 其他绘图操作代码
	//// ......
	//::ReleaseDC(hMainWnd, hMainDC);
	
	//PAINTSTRUCT ps;
	//HDC hMainDC = ::BeginPaint(hMainWnd, &ps);
	//// 其他绘图操作代码
	//// ......
	//::EndPaint(hMainWnd, &ps);

	//HDC hMemDC = ::CreateCompatibleDC(hDest);
	//// 其他绘图操作代码
	//// ......
	//::BitBlt(hDest, x, y, cx, cy, hMemDC, 0, 0, SRCCOPY); //将内存DC拷贝到目标DC
	//::DeleteDC(hMemDC);

	HDC hMainDC = ::GetDC(hMainWnd);
	LOGFONT lf = { 0 };
	lf.lfWidth = 16;
	lf.lfHeight = 40;
	lf.lfCharSet = GB2312_CHARSET;
	HFONT hFont = ::CreateFontIndirect(&lf);
	HFONT hOldFont = static_cast<HFONT>(::SelectObject(hMainDC, (HGDIOBJ)hFont));
	RECT rect;
	::GetClientRect(hMainWnd, &rect);
	DrawText(hMainDC, _T("ABC"), 3, &rect, DT_LEFT);

	::SelectObject(hMainDC, hOldFont);
	::DeleteObject(hFont);
	::ReleaseDC(hMainWnd, hMainDC);
}
