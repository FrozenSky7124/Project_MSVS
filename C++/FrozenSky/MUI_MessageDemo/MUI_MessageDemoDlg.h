
// MUI_MessageDemoDlg.h : header file
//

#pragma once
#include "MUI_Message.h"

// CMUI_MessageDemoDlg dialog
class CMUI_MessageDemoDlg : public CDialogEx
{
// Construction
public:
	CMUI_MessageDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUI_MESSAGEDEMO_DIALOG };
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
	afx_msg void OnBnClickedOk();
	MUI_Message MMsg;
};
