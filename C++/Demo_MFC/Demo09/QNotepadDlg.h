
// QNotepadDlg.h : 头文件
//

#pragma once


// QNotepadDlg 对话框
class QNotepadDlg : public CDialogEx
{
// 构造
public:
	QNotepadDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO09_DIALOG };
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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnAppExit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnHelpmenuAbout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnEditmenuSelectall();
	afx_msg void OnEditmenuDatetime();

protected:
	void FileRead_ANSI(CFile& file);
	void FileRead_UTF8(CFile& file);
	void FileRead_UTF16(CFile& file);
};
