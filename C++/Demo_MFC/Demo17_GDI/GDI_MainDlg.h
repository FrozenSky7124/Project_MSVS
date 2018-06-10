
// GDI_MainDlg.h : 头文件
//

#pragma once


// GDI_MainDlg 对话框
class GDI_MainDlg : public CDialogEx
{
// 构造
public:
	GDI_MainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO17_GDI_DIALOG };
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

	afx_msg void OnBnClickedBtnTest();

	DECLARE_MESSAGE_MAP()

public:
	HBITMAP m_hBmpBackground;
};
