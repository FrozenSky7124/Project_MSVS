
// UDPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo13_UDP.h"
#include "UDPDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// RecvThread
UINT WINAPI uiRecvThread(LPVOID lpParam)
{
	WSADATA wsaData = { 0 };
	UINT iResult = 0;

	// 初始化
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		AfxMessageBox(_T("WSAStartup failed: %d\n"), iResult);
		return 1;
	}

	// 创建UDP_Socket
	SOCKET serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serverSocket == INVALID_SOCKET)
		AfxMessageBox(_T("socket function failed with error = %d\n"), WSAGetLastError());
	else
	{
		AfxMessageBox(_T("socket function succeeded\n"));

		TCHAR RecvBuf[256] = { 0 };
		int BufLen = _countof(RecvBuf);

		sockaddr_in addr_sender;
		int iSenderAddrSize = sizeof(addr_sender);

		sockaddr_in addr_recv;
		addr_recv.sin_family = AF_INET;
		addr_recv.sin_port = htons(21001);
		addr_recv.sin_addr.s_addr = htonl(INADDR_ANY);

		iResult = bind(serverSocket, (SOCKADDR *)& addr_recv, sizeof(addr_recv));
		if (iResult != 0)
			AfxMessageBox(_T("bind failed with error %d\n"), WSAGetLastError());

		iResult = recvfrom(serverSocket, RecvBuf, BufLen, 0, (SOCKADDR *)& addr_sender, &iSenderAddrSize);
		if (iResult == SOCKET_ERROR)
			AfxMessageBox(_T("recvfrom failed with error %d\n"), WSAGetLastError());

		((UDPDlg*)lpParam)->SetDlgItemTextA(IDC_STATIC_STATUS, RecvBuf);

		// 关闭UDP_Socket
		iResult = closesocket(serverSocket);
		if (iResult == SOCKET_ERROR)
			AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	}

	WSACleanup();

	TRACE(_T("uiRecvThread Over!\n"));
	_endthreadex(0);
	return 0;
}

// UDPDlg 对话框

UDPDlg::UDPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO13_UDP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void UDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(UDPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// UDPDlg 消息处理程序

BOOL UDPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &uiRecvThread, this, 0, &m_iRecvThreadId);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void UDPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR UDPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void UDPDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	TRACE(_T("OnOK\n"));

	//CDialogEx::OnOK();
}


void UDPDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	TRACE(_T("OnCancel\n"));
	
	CDialogEx::OnCancel();
}
