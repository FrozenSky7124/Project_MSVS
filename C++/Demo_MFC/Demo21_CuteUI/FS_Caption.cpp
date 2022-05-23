// FS_Caption.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo21_CuteUI.h"
#include "FS_Caption.h"


// FS_Caption

IMPLEMENT_DYNAMIC(FS_Caption, CStatic)

FS_Caption::FS_Caption()
{

}

FS_Caption::~FS_Caption()
{

}

void FS_Caption::Init(int x, int y, int W, int H)
{
	MoveWindow(x, y, W, H);
}

HBRUSH FS_Caption::CtlColor(CDC * pDC, UINT)
{
	if (!pDC) return NULL;
	// 创建一个空画刷，返回这个画刷可以设置静态控件的背景透明
	//HBRUSH hr = (HBRUSH)GetStockObject(NULL_BRUSH);
	// 创建一个单色画刷，返回这个画刷可以设置静态控件的背景颜色
	HBRUSH hr = CreateSolidBrush(RGB(60, 60, 60));
	// 使文字背景透明/背景颜色
	pDC->SetBkMode(TRANSPARENT); // 文字背景透明
	//pDC->SetBkColor(RGB(31, 31, 31)); // 文字背景颜色
	// 设置文字颜色
	pDC->SetTextColor(RGB(102, 204, 255));
	// 设置字体
	CFont font;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 20; // 设置逻辑字体的高度
	lf.lfWeight = FW_BOLD; // 设置逻辑字体为粗体
	::lstrcpy(lf.lfFaceName, _T("Bitsumishi")); // 设置字体
	font.CreateFontIndirect(&lf); // 使用逻辑字体 lf 创建一个 CFont 对象
	pDC->SelectObject(&font); // 应用字体

	return hr;
	//return HBRUSH();
}


BEGIN_MESSAGE_MAP(FS_Caption, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// FS_Caption 消息处理程序


