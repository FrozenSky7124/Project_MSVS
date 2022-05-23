
// CuteDlg01.h : header file
//

#pragma once
#include "PNGButton.h"
#include "KCBusyProgressCtrl.h"
#include "afxwin.h"

// CuteDlg01 dialog
class CuteDlg01 : public CDialogEx
{
// Construction
public:
	CuteDlg01(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO20_CUTEDLG01_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// WM_CTLCOLOR
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	CBrush m_bkBrush; //±³¾°»­Ë¢
	Gdiplus::Image * m_pImgBk;
	HDC m_hdcMem;
	BLENDFUNCTION m_Blend;
	int m_iBkWidth;
	int m_iBkHeight;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	PNGButton m_PNGBtn;
	afx_msg void OnBnClickedButtonKeqing();
	PNGButton m_BtnSwd;
	CKCBusyProgressCtrl m_ctlBProgress;
};
