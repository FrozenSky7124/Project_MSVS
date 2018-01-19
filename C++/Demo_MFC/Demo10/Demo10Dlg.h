
// Demo10Dlg.h : 头文件
//

#pragma once

#include "ModelDlg.h"

// CDemo10Dlg 对话框
class CDemo10Dlg : public CDialogEx
{
// 构造
public:
	CDemo10Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO10_DIALOG };
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
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();

// FrozenSky
protected:
	ModelDlg m_modelDlg;
};
