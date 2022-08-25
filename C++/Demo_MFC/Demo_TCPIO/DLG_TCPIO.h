// DLG_TCPIO.h : header file
#pragma once

#include "afxwin.h"
#include "FS_Static.h"
#include "FS_Button.h"
#include "D:\GitHub\Project_CHO\Library\MFC_FSUI\FS_Static.h"

// DLG_TCPIO dialog
class DLG_TCPIO : public CDialog
{
	// Construction
public:
	DLG_TCPIO(CWnd* pParent = NULL);	// standard constructor

										// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_TCPIO_DIALOG };
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	// TCP Socket
	WSADATA m_wsaData;
	SOCKET m_SocketListen;
	SOCKET m_SocketConn;
	SOCKADDR_IN m_Server_Addr;
	SOCKADDR_IN m_Client_Addr;
	HANDLE m_hThread_Accept;
	UINT m_iThread_Accept;
	int InitTCPServer();
	int ConnectToServer();
	int ThreadRecvBegin();
	int MsgProc(char* msg);
	int CloseSocketConn();
	
	// UI
	UINT m_winSzW, m_winSzH;
	FS_Static m_UI_Caption;
	FS_Static m_UI_State;
	FS_Static m_Static_1;
	FS_Static m_Static_2;
	FS_Button m_Button_1;
	FS_Button m_Button_2;
};
