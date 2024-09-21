
// PowerAlertUI.h : header file
//

#pragma once

#include "mmsystem.h"
#include "Digitalv.h"
#include "afxwin.h"


// PowerAlertUI dialog
class PowerAlertUI : public CDialogEx
{
// Construction
public:
	PowerAlertUI(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POWERALERT_DIALOG };
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
	CEdit m_Edit1;
	CEdit m_EditInterval;
	CEdit m_EditVolume;

	UINT m_MonitorStatus; // 0: off   1: on
	UINT m_AlertStatus;   // 0: not ringing   1: ringing
	UINT m_Interval;
	UINT m_Volume;
	MCIDEVICEID m_nDeviceID;

	void InitUI();
	void Monitor();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
