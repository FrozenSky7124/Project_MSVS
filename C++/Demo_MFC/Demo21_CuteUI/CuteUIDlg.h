
// CuteUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "FS_Caption.h"
#include "FS_Button.h"


// CuteUIDlg dialog
class CuteUIDlg : public CDialog
{
// Construction
public:
	CuteUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO21_CUTEUI_DIALOG };
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
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	FS_Caption m_UICaption;
	CBrush m_bkBrush;
	FS_Button m_btnMin;
	FS_Button m_btnClose;
};
