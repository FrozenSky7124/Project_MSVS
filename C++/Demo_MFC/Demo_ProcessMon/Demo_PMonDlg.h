
// Demo_PMonDlg.h : 头文件
//

#pragma once

#define LISTENPORT 17241 //定义接收端口
#define UDPPKGSIZE 32    //定义UDP数据包大小

// CDemo_PMonDlg 对话框
class CDemo_PMonDlg : public CDialogEx
{
// 构造
public:
	CDemo_PMonDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_PROCESSMON_DIALOG };
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
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnIDCancel();
	void InitMonitor();
	BOOL InitSocket();
	void SearchProcess();

	// UDP Socket
	WSADATA m_wsaData;
	SOCKET m_socket;
	sockaddr_in m_addrSendto;
};
