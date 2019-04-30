
// UT_ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UT_Client.h"
#include "UT_ClientDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UINT WINAPI uiRecvThread(LPVOID lpParam)
{
	UT_ClientDlg * pThis = (UT_ClientDlg*)lpParam;
	UINT iRst = 0;
	ULONGLONG lRecvSize = 0;
	char * RecvBuf = new char[BLOCKSIZE];

	sockaddr_in addr_sender;
	int iSenderAddrSize = sizeof(addr_sender);
	
	while ((iRst = recvfrom(pThis->m_socket, RecvBuf, BLOCKSIZE, 0, (SOCKADDR *)& addr_sender, &iSenderAddrSize)) != SOCKET_ERROR)
	{
		lRecvSize += iRst;
		if (iRst < BLOCKSIZE) break;
	}
	
	TRACE(_T("lRecvSize = %d\n"), lRecvSize); //33655680
	
	if (RecvBuf != NULL) delete RecvBuf;
	RecvBuf = NULL;

	return 0x00;
}


// UT_ClientDlg 对话框


UT_ClientDlg::UT_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UT_CLIENT_DIALOG, pParent)
	, m_socket(INVALID_SOCKET)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void UT_ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(UT_ClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// UT_ClientDlg 消息处理程序

BOOL UT_ClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:
	if (FALSE == InitSocket())
		AfxMessageBox(_T("InitSocket Error!"));

	// Start recv thread
	m_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &uiRecvThread, this, 0, &m_iRecvThread);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。
// 对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。
void UT_ClientDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR UT_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL UT_ClientDlg::InitSocket()
{
	UINT iRst = 0;

	// 初始化SOCKET
	iRst = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (iRst != 0)
	{
		AfxMessageBox(_T("WSAStartup failed: %d\n"), iRst);
		return FALSE;
	}
	// 创建UDP_Socket
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		AfxMessageBox(_T("socket function failed with error = %d\n"), WSAGetLastError());
		WSACleanup();
		return FALSE;
	}
	else
	{
		m_addrRecvin.sin_family = AF_INET;
		m_addrRecvin.sin_port = htons(21001);
		m_addrRecvin.sin_addr.s_addr = htonl(INADDR_ANY);
		iRst = bind(m_socket, (SOCKADDR *)& m_addrRecvin, sizeof(m_addrRecvin));
		if (iRst != 0)
		{
			AfxMessageBox(_T("bind failed with error %d\n"), WSAGetLastError());
			return FALSE;
		}
		return TRUE;
	}
}


void UT_ClientDlg::OnCancel()
{
	// TODO:
	// 关闭UDP_Socket
	UINT iRst = closesocket(m_socket);
	if (iRst == SOCKET_ERROR)
		AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	WSACleanup();

	CDialog::OnCancel();
}
