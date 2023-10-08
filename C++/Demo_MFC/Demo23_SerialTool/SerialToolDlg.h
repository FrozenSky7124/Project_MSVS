
// SerialToolDlg.h : header file
//

#pragma once

#include <iostream>
#include "FSC_Serial.h"

// SerialToolDlg dialog
class SerialToolDlg : public CDialogEx
{
// Construction
public:
	SerialToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO23_SERIALTOOL_DIALOG };
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
	afx_msg void OnClicked_BtnOpen();
};
