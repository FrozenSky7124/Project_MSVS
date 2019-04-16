
// TTransDlg.h : 头文件
//

#pragma once


// TTransDlg 对话框
class TTransDlg : public CDialog
{
// 构造
public:
	TTransDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO19_TTRANS_DIALOG };
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
	int InitTCPSocket();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	BOOL LoadFile(CString csPath);
	virtual void OnCancel();

	// File info
	BYTE * m_pFileData; //store ptr for file data in bytes
	ULONGLONG m_lFileLength; //store file length in bytes

	// TCP Socket
	WSADATA m_wsaData;
	SOCKET m_SockListenH;
	SOCKET m_SockListenD;
	sockaddr_in m_Server_Addr_H;
	sockaddr_in m_Server_Addr_D;
	sockaddr_in m_Client_Addr_H;
	sockaddr_in m_Client_Addr_D;
	SOCKET m_SockConnH;
	SOCKET m_SockConnD;

	// Thread
	HANDLE m_hAcceptThreadH, m_hAcceptThreadD;
	UINT m_iAcceptThreadH, m_iAcceptThreadD;
};
