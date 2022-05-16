#pragma once
#include "afxwin.h"
 
class PNGButton : public CWnd
{
public:
	PNGButton(void);
	DECLARE_DYNCREATE(PNGButton)   
	~PNGButton(void);
	virtual BOOL Create(UINT x,UINT y, CWnd* pParentWnd, UINT nID,
		UINT PngID,Gdiplus::Image* hoverBg, CCreateContext* pContext = NULL);  
	virtual BOOL Create(UINT x,UINT y, CWnd* pParentWnd, UINT nID,
		UINT PngID,UINT PngID_hOver, CCreateContext* pContext = NULL);  
protected:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);   
	DECLARE_MESSAGE_MAP()  

	Gdiplus::Image* bg;
	Gdiplus::Image* hoverBg;
	bool m_is_mouse_over;
	bool m_is_tracked;
	bool DrawBorder;
	bool m_is_checked;
	UINT mWidth;
	UINT mHeight;

public:
	afx_msg void OnPaint();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClick(); 
	void Check(bool check);
	void PaintParent();  
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

