
// CuteDlg01.cpp : implementation file
//

#include "stdafx.h"
#include "Demo20_CuteDlg01.h"
#include "CuteDlg01.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CuteDlg01 dialog



CuteDlg01::CuteDlg01(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO20_CUTEDLG01_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CuteDlg01::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CuteDlg01, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CuteDlg01 message handlers

BOOL CuteDlg01::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_Blend.BlendOp = 0;
	m_Blend.BlendFlags = 0;
	m_Blend.AlphaFormat = 1;
	m_Blend.SourceConstantAlpha = 255;

	m_pImgBk = Gdiplus::Image::FromFile(L".\\res\\kqsleep.png");
	m_iBkWidth = m_pImgBk->GetWidth();
	m_iBkHeight = m_pImgBk->GetHeight();

	//MoveWindow(0, 0, m_iBkWidth, m_iBkHeight);

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, m_iBkWidth, m_iBkHeight, SWP_NOSIZE | SWP_NOMOVE);

	//COLORREF maskColor = RGB(0, 0, 0);
	//SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE | WS_EX_LAYERED));
	//::SetLayeredWindowAttributes(this->GetSafeHwnd(), maskColor, 0, LWA_ALPHA);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CuteDlg01::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		HDC hdcTmp = GetDC()->m_hDC;
		m_hdcMem = CreateCompatibleDC(hdcTmp);
		HBITMAP hBitMap = CreateCompatibleBitmap(hdcTmp, m_iBkWidth, m_iBkHeight);
		SelectObject(m_hdcMem, hBitMap);

		HDC hdcScreen = ::GetDC(m_hWnd);
		RECT rct;
		GetWindowRect(&rct);
		POINT ptWinPos = { rct.left, rct.top };
		Gdiplus::Graphics graph(m_hdcMem);
		Gdiplus::Point points[] = { Gdiplus::Point(0, 0), Gdiplus::Point(0 + m_iBkWidth, 0), Gdiplus::Point(0, 0 + m_iBkHeight) };
		static bool bFly = false;
		graph.DrawImage(m_pImgBk, points, 3);

		SIZE sizeWindow = { m_iBkWidth, m_iBkHeight };
		POINT ptSrc = { 0, 0 };
		DWORD dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		if ((dwExStyle & 0x80000) != 0x80000)
			SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ 0x80000);
		::UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos, &sizeWindow, m_hdcMem, &ptSrc, 0, &m_Blend, 2);
		graph.ReleaseHDC(m_hdcMem);
		::ReleaseDC(m_hWnd, hdcScreen);
		hdcScreen = NULL;
		::ReleaseDC(m_hWnd, hdcTmp);
		hdcTmp = NULL;
		DeleteObject(hBitMap);
		DeleteDC(m_hdcMem);
		m_hdcMem = NULL;

		/*
		CPaintDC dc(this);
		CString csImgPath = _T(".\\res\\sh.png");
		CImage cImgBk;
		if (cImgBk.IsNull())
		{
			cImgBk.Load(csImgPath);
		}

		if (cImgBk.GetBPP() == 32)
		{
			int i = 0;
			int j = 0;
			for (i = 0; i < cImgBk.GetWidth(); i++)
			{
				for (j = 0; j < cImgBk.GetHeight(); j++)
				{
					byte * pbyte = (byte *)cImgBk.GetPixelAddress(i, j);
					pbyte[0] = pbyte[0] * pbyte[3] / 255;
					pbyte[1] = pbyte[1] * pbyte[3] / 255;
					pbyte[2] = pbyte[2] * pbyte[3] / 255;
				}
			}
		}
		// 设置窗体属性为 LayeredWindow （无边框窗体）
		//LONG para = GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
		//para |= WS_EX_LAYERED;
		//SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, para);
		// 设置窗体透明色，透明度255
		//SetLayeredWindowAttributes(RGB(0, 0, 255), 255, LWA_COLORKEY | LWA_ALPHA);
		// 绘制窗体背景色
		//RECT rect;
		//GetClientRect(&rect);
		//dc.FillSolidRect(&rect, RGB(0, 0, 255));
		// 绘制图片
		cImgBk.Draw(dc.m_hDC, 0, 0, cImgBk.GetWidth(), cImgBk.GetHeight());
		*/
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CuteDlg01::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CuteDlg01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd == this)
	{
		//return m_bkBrush;
	}

	return hbr;
}


void CuteDlg01::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO
	delete m_pImgBk;
	m_pImgBk = NULL;
}


void CuteDlg01::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 
	//禁止显示移动矩形窗体框
	::SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, 0);
	//非标题栏移动整个窗口
	SendMessage(WM_SYSCOMMAND, 0xF012, 0);

	CDialogEx::OnLButtonDown(nFlags, point);
}
