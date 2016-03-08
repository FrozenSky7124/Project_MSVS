#pragma once

#include "sqlite3.h"

// MainDialog 对话框

class MainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MainDialog)

public:
	MainDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MainDialog();

	CFont m_editFont;
	CFont m_editFontH;
	virtual BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnChangeSearchEdit();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	bool OpenDB();
	sqlite3 *db;
	bool firstCall;
	bool confirmAddr;
	int idAddr;
	int preID;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
