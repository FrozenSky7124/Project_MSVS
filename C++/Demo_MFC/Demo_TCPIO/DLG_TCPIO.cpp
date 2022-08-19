
// DLG_TCPIO.cpp : implementation file
//

#include "stdafx.h"
#include "Demo_TCPIO.h"
#include "DLG_TCPIO.h"
#include "afxdialogex.h"
#include "ws2tcpip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UINT WINAPI uiThread_Accept(LPVOID lpParam)
{
	DLG_TCPIO * pThis = (DLG_TCPIO*)lpParam;

	int iClientAddr_length = sizeof(sockaddr_in);
	memset(&(pThis->m_Client_Addr), 0, iClientAddr_length);

	if ((pThis->m_SocketConn = accept(pThis->m_SocketListen, (sockaddr *)&(pThis->m_Client_Addr), &iClientAddr_length)) == INVALID_SOCKET)
	{
		printf("SocketConn accept failed !!!\n");
		closesocket(pThis->m_SocketConn);
		WSACleanup();
		return 1;
	}
	printf("SocketConn accept success !!!\n");
	pThis->ThreadRecvBegin();

	// Close listen socket when accept succeed
	closesocket(pThis->m_SocketListen);

	printf("Thread_Accept Terminated!\n");
	return 0;
}

UINT WINAPI uiThread_Recv(LPVOID lpParam)
{
	DLG_TCPIO * pThis = (DLG_TCPIO*)lpParam;

	while (pThis->m_SocketConn != INVALID_SOCKET)
	{
		int nRecvBytes = SOCKET_ERROR;
		char cRecvBuf[33] = "";
		nRecvBytes = recv(pThis->m_SocketConn, cRecvBuf, 32, 0);
		cRecvBuf[32] = '\0';
		printf("Bytes Recv: %ld\n", nRecvBytes);
		printf("%s\n", cRecvBuf);
		if (nRecvBytes <= 0) break;
	}

	pThis->CloseSocketConn();
	printf("Thread_Recv Terminated!\n");
	return 0;
}


// DLG_TCPIO dialog

DLG_TCPIO::DLG_TCPIO(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEMO_TCPIO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_SocketListen = NULL;
	m_SocketConn = NULL;
}

void DLG_TCPIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_1, m_Static_1);
	DDX_Control(pDX, IDC_STATIC_2, m_Static_2);
	DDX_Control(pDX, IDC_BUTTON_1, m_Button_1);
}

BEGIN_MESSAGE_MAP(DLG_TCPIO, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_1, &DLG_TCPIO::OnBnClickedButton1)
END_MESSAGE_MAP()


// DLG_TCPIO message handlers

BOOL DLG_TCPIO::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// Init Console
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);

	// Init UI
	m_Static_1.Init(0, 0, 0, 102, 204, 255, "ËÎÌו", 35);
	m_Static_2.Init(0, 0, 0, 102, 204, 255, "ËÎÌו", 35);
	m_Button_1.EnableWindow(false);

	// Init Socket
	if (0 == InitTCPSocket()) printf("Init TCP Socket success.\n");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void DLG_TCPIO::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags the minimized window.
HCURSOR DLG_TCPIO::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void DLG_TCPIO::OnCancel()
{
	// TODO:
	if (m_SocketConn != NULL)
	{
		closesocket(m_SocketConn);
		m_SocketConn = NULL;
	}
	WSACleanup();
	FreeConsole();

	CDialog::OnCancel();
}

void DLG_TCPIO::OnBnClickedButton1()
{
	if (m_SocketConn == NULL || m_SocketConn == INVALID_SOCKET)
	{
		printf("SocketConn Invalid.\n");
		return;
	}
	int nSentBytes = 0;
	char cSendBuf[32] = "Server: Test Data.";
	nSentBytes = send(m_SocketConn, cSendBuf, int(strlen(cSendBuf)), 0);
	printf("Bytes Sent: %d\n", nSentBytes);
}


/**
 * Init TCP Socket
 * Return   : [-1]: failed   [0]: success
 */
int DLG_TCPIO::InitTCPSocket()
{
	// Init winsock data
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
	{
		printf("WSAStartup failed !!!\n");
		return -1;
	}
	// Create listen socket: Head & Data
	if ((m_SocketListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Create SocketListen failed !!!\n");
		WSACleanup();
		return -1;
	}
	// Set server ip & port
	memset(&m_Server_Addr, 0, sizeof(SOCKADDR_IN));
	m_Server_Addr.sin_family = AF_INET;
	m_Server_Addr.sin_port = htons(37124);
	inet_pton(AF_INET, "192.168.5.48", &m_Server_Addr.sin_addr);
	//m_Server_Addr.sin_addr.S_un.S_addr = inet_addr("192.168.5.48"); InetPton("192.168.5.48");
	// Bind socket
	if (bind(m_SocketListen, (sockaddr *)&m_Server_Addr, sizeof(sockaddr_in)) != 0)
	{
		printf("Bind SocketListen failed !!!\n");
		closesocket(m_SocketListen);
		WSACleanup();
		return -1;
	}
	// Listen start
	if (listen(m_SocketListen, 2) != 0)
	{
		printf("Start listen failed !!!\n");
		closesocket(m_SocketListen);
		WSACleanup();
		return -1;
	}

	// Create accept thread
	m_hThread_Accept = (HANDLE)_beginthreadex(NULL, 0, &uiThread_Accept, this, 0, &m_iThread_Accept);

	return 0;
}

int DLG_TCPIO::ThreadRecvBegin()
{
	UINT m_iThread_Recv;
	HANDLE m_hThread_Recv = (HANDLE)_beginthreadex(NULL, 0, &uiThread_Recv, this, 0, &m_iThread_Recv);

	m_Button_1.EnableWindow(true);
	return 0;
}

int DLG_TCPIO::CloseSocketConn()
{
	if (m_SocketConn != NULL)
	{
		closesocket(m_SocketConn); // Close SocketConn when disconnected.
		m_SocketConn = NULL;
	}
	m_Button_1.EnableWindow(false);
	return 0;
}
