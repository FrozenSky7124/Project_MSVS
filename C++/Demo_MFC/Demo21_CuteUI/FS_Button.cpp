// FS_Button.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo21_CuteUI.h"
#include "FS_Button.h"


// FS_Button

IMPLEMENT_DYNAMIC(FS_Button, CButton)

FS_Button::FS_Button()
{
	m_bMouseHover = FALSE;
}

FS_Button::~FS_Button()
{

}

void FS_Button::SetImagePath(CString strImgPath, CString strParentImgPath)
{
	m_strImgPath = strImgPath;
	m_strImgParentPath = strParentImgPath;
}

bool FS_Button::InitMyButton(int nX, int nY, int nW, int nH, bool bIsPng)
{
	HRESULT hr = 0;
	if (m_strImgPath.IsEmpty())	return false;
	hr = m_imgButton.Load(m_strImgPath);

	if (FAILED(hr))	return false;

	if (bIsPng) //如果是PNG带透明色的图片，需要对它进行特殊处理
	{
		if (m_imgButton.GetBPP() == 32)
		{
			int i = 0;
			int j = 0;
			for (i = 0; i < m_imgButton.GetWidth(); i++)
			{
				for (j = 0; j < m_imgButton.GetHeight(); j++)
				{
					byte * pbyte = (byte *)m_imgButton.GetPixelAddress(i, j);
					pbyte[0] = pbyte[0] * pbyte[3] / 255;
					pbyte[1] = pbyte[1] * pbyte[3] / 255;
					pbyte[2] = pbyte[2] * pbyte[3] / 255;
				}
			}
		}
	}

	MoveWindow(nX, nY, nW, nH);

	return true;
}

void FS_Button::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (!lpDrawItemStruct) return;
	HDC hMemDC;
	HBITMAP bmpMem;
	HGDIOBJ hOldObj;
	bmpMem = CreateCompatibleBitmap(lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top);
	if (!bmpMem) return;
	hMemDC = CreateCompatibleDC(lpDrawItemStruct->hDC);
	if (!hMemDC)
	{
		if (bmpMem)
		{
			::DeleteObject(bmpMem);
			bmpMem = NULL;
		}
		return;
	}

	hOldObj = ::SelectObject(hMemDC, bmpMem);

	RECT rectTmp = { 0 };
	rectTmp = lpDrawItemStruct->rcItem;
	MapWindowPoints(GetParent(), &rectTmp);
	int nW = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int nH = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		//按钮被选择
		m_imgButton.BitBlt(hMemDC, 0, 0, nW, nH, nW * 2, 0, SRCCOPY);
	}
	else if (/*lpDrawItemStruct->itemState & ODS_FOCUS || */m_bMouseHover)
	{
		//焦点状态 //不判断焦点状态只判断鼠标滑过
		m_imgButton.BitBlt(hMemDC, 0, 0, nW, nH, nW, 0, SRCCOPY);
	}
	else 
	{
		//默认状态
		CImage imgParent;
		imgParent.Create(nW, nH, 32);
		for (int i = 0; i < imgParent.GetWidth(); i++)
		{
			for (int j = 0; j < imgParent.GetHeight(); j++)
			{
				byte * pbyte = (byte *)imgParent.GetPixelAddress(i, j);
				pbyte[0] = pbyte[1] = pbyte[2] = 60;
				pbyte[3] = 255;
			}
		}
		//imgParent.Draw(hMemDC, 0, 0, nW, nH, rectTmp.left, rectTmp.top, nW, nH);
		imgParent.Draw(hMemDC, 0, 0, nW, nH, 0, 0, nW, nH);
		m_imgButton.AlphaBlend(hMemDC, 0, 0, nW, nH, 0, 0, nW, nH);
		imgParent.Destroy();
	}

	::BitBlt(lpDrawItemStruct->hDC, 0, 0, nW, nH, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldObj);

	if (bmpMem)
	{
		::DeleteObject(bmpMem);
		bmpMem = NULL;
	}

	if (hMemDC)
	{
		::DeleteDC(hMemDC);
		hMemDC = NULL;
	}
	return;
}

BEGIN_MESSAGE_MAP(FS_Button, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()



// FS_Button 消息处理程序




void FS_Button::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT  tme = { 0 };
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = 50;
	tme.hwndTrack = this->m_hWnd;
	if (TrackMouseEvent(&tme)) {}
	TRACE("OnMouseMove\n");
	CButton::OnMouseMove(nFlags, point);
}


void FS_Button::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bMouseHover = FALSE;
	RECT rcBtn;
	GetClientRect(&rcBtn);
	InvalidateRect(&rcBtn);
	TRACE("OnMouseLeave\n");
	CButton::OnMouseLeave();
}


void FS_Button::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bMouseHover = TRUE;
	RECT rcBtn;
	GetClientRect(&rcBtn);
	InvalidateRect(&rcBtn);
	TRACE("OnMouseHover\n");
	CButton::OnMouseHover(nFlags, point);
}
