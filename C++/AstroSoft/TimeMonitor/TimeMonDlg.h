
// TimeMonDlg.h : 头文件
//

#pragma once
#include <vector>
#include <algorithm>
#include "FSC_FitsX.h"
#include "afxcmn.h"


// TimeMonDlg 对话框
class TimeMonDlg : public CDialogEx
{
// 构造
public:
	TimeMonDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIMEMONITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnstart();

	void Proc_CheckTime(CString csPath);

	std::vector<CString> m_vFitsName;
	CString m_csFitsDir;
	int m_iTimeErrCount;
	int m_iFitsCount; //Record fits file count of OBJECT directory
	int m_iImgIndex;

	int m_iThreadStatus; //Thread status. 0 Stop, 1 Running, 2 Pause.
	HANDLE m_hThread_Proc;

	FSC_FitsX m_FSCFitsX;
	CProgressCtrl m_Progress;
};
