// MUI_Message.cpp : 实现文件
//

#include "stdafx.h"
#include "MUI_Message.h"
#include "afxdialogex.h"
#include "MUI_MessageDemo.h"


// MUI_Message 对话框

IMPLEMENT_DYNAMIC(MUI_Message, CDialogEx)

MUI_Message::MUI_Message(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

MUI_Message::~MUI_Message()
{

}

void MUI_Message::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MUI_Message, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// MUI_Message 消息处理程序

void MUI_Message::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: 在此处添加消息处理程序代码
	/* 绘制透明PNG图片窗体 */
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
	graph.DrawImage(m_pImgBk, points, Gdiplus::UnitPoint);

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

	// 不为绘图消息调用 CDialogEx::OnPaint)
	CDialogEx::OnPaint();
}


BOOL MUI_Message::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Blend.BlendOp = 0;
	m_Blend.BlendFlags = 0;
	m_Blend.AlphaFormat = 1;
	m_Blend.SourceConstantAlpha = 255;

	m_pImgBk = Gdiplus::Image::FromFile(L".\\res\\imgBk.png");
	m_iBkWidth = m_pImgBk->GetWidth();
	m_iBkHeight = m_pImgBk->GetHeight();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}