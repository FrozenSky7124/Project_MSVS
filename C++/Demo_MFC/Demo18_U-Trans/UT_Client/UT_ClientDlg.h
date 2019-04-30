
// UT_ClientDlg.h : 头文件
//

#pragma once

#define BLOCKSIZE 32768 //data size in UDP package

// UT_ClientDlg 对话框
class UT_ClientDlg : public CDialog
{
// 构造
public:
	UT_ClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UT_CLIENT_DIALOG };
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
	BOOL InitSocket();
	virtual void OnCancel();

	// UDP Socket
	WSADATA m_wsaData;
	SOCKET m_socket;
	sockaddr_in m_addrRecvin;

	// Send thread
	HANDLE m_hRecvThread; //store handle of udp receive thread
	UINT m_iRecvThread; //store id of udp receive thread
};
