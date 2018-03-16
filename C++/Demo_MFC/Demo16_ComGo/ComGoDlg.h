
// ComGoDlg.h : 头文件
//

#pragma once
#define ROWLENGTH 43
#define ROWLENGTH_BYTE 14

// ComGoDlg 对话框
class ComGoDlg : public CDialog
{
// 构造
public:
	ComGoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO16_COMGO_DIALOG };
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
	virtual void OnOK();
	virtual void OnCancel();

public:
	CListCtrl* m_pCListData;	//列表控件
	CHAR* m_pData;				//存储文本数据
	BYTE* m_pByteData;			//存储字节数据
	HANDLE m_hComm;				//存储已打开的串口句柄
	DCB m_dcb;					//存储串口配置结构体
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonOpencom();
	afx_msg void OnBnClickedButtonSend();
};
