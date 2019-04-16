
// TTransDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo19_T-Trans.h"
#include "TTransDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UINT WINAPI uiAcceptThreadH(LPVOID lpParam)
{
	TTransDlg * pThis = (TTransDlg*)lpParam;

	int iClientAddrH_length = sizeof(sockaddr_in);
	memset(&(pThis->m_Client_Addr_H), 0, iClientAddrH_length);

	if ((pThis->m_SockConnH = accept(pThis->m_SockListenH, (sockaddr *)&(pThis->m_Client_Addr_H), &iClientAddrH_length)) == INVALID_SOCKET)
	{
		printf("SockConnH accept failed !!!\n");
		closesocket(pThis->m_SockConnH);
		WSACleanup();
		return -1;
	}
	printf("SockConnH accept success !!!\n");

	// Close listen socket when accept succeed
	closesocket(pThis->m_SockListenH);

	return 0;
}


UINT WINAPI uiAcceptThreadD(LPVOID lpParam)
{
	TTransDlg * pThis = (TTransDlg*)lpParam;

	int iClientAddrD_length	= sizeof(sockaddr_in);
	memset(&(pThis->m_Client_Addr_D), 0, iClientAddrD_length);
	if ((pThis->m_SockConnD = accept(pThis->m_SockListenD, (sockaddr *)&(pThis->m_Client_Addr_D), &iClientAddrD_length)) == INVALID_SOCKET)
	{
		printf("SockConnD accept failed !!!\n");
		closesocket(pThis->m_SockConnD);
		WSACleanup();
		return -1;
	}
	printf("SockConnD accept success !!!\n");

	// Close listen socket when accept succeed
	closesocket(pThis->m_SockListenD);

	return 0;
}


// TTransDlg 对话框


TTransDlg::TTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEMO19_TTRANS_DIALOG, pParent)
	, m_pFileData(NULL)
	, m_lFileLength(0)
	, m_SockListenH(NULL)
	, m_SockListenD(NULL)
	, m_SockConnH(NULL)
	, m_SockConnD(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void TTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TTransDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// TTransDlg 消息处理程序

BOOL TTransDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:
	// Init Console
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);

	// Init TCPSocket
	InitTCPSocket();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。
// 对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。

void TTransDlg::OnPaint()
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
HCURSOR TTransDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int TTransDlg::InitTCPSocket()
{
	// Init winsock data
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
	{
		printf("WSAStartup failed !!!\n");
		return -1;
	}
	// Create listen socket: Head & Data
	if ((m_SockListenH = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Create SocketListenH failed !!!\n");
		WSACleanup();
		return -1;
	}
	if ((m_SockListenD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Create SocketListenD failed !!!\n");
		WSACleanup();
		return -1;
	}

	// Set server ip & port
	memset(&m_Server_Addr_H, 0, sizeof(sockaddr_in));
	memset(&m_Server_Addr_D, 0, sizeof(sockaddr_in));
	m_Server_Addr_H.sin_family = AF_INET;
	m_Server_Addr_H.sin_port = htons(21001);
	m_Server_Addr_H.sin_addr.S_un.S_addr = inet_addr("10.0.0.117");
	m_Server_Addr_D.sin_family = AF_INET;
	m_Server_Addr_D.sin_port = htons(21002);
	m_Server_Addr_D.sin_addr.S_un.S_addr = inet_addr("10.0.0.117");
	
	// Bind socket
	if (bind(m_SockListenH, (sockaddr *)&m_Server_Addr_H, sizeof(sockaddr_in)) != 0)
	{
		printf("Bind SocketListenH failed !!!\n");
		closesocket(m_SockListenH);
		WSACleanup();
		return -1;
	}
	if (bind(m_SockListenD, (sockaddr *)&m_Server_Addr_D, sizeof(sockaddr_in)) != 0)
	{
		printf("Bind SocketListenD failed !!!\n");
		closesocket(m_SockListenD);
		WSACleanup();
		return -1;
	}

	// Listen start
	if (listen(m_SockListenH, 2) != 0)
	{
		printf("Listen SockListenH failed !!!\n");
		closesocket(m_SockListenH);
		WSACleanup();
		return -1;
	}
	if (listen(m_SockListenD, 2) != 0)
	{
		printf("Listen SockListenD failed !!!\n");
		closesocket(m_SockListenD);
		WSACleanup();
		return -1;
	}

	// Accept connect
	
	// Create accept thread
	m_hAcceptThreadH = (HANDLE)_beginthreadex(NULL, 0, &uiAcceptThreadH, this, 0, &m_iAcceptThreadH);
	m_hAcceptThreadD = (HANDLE)_beginthreadex(NULL, 0, &uiAcceptThreadD, this, 0, &m_iAcceptThreadD);

	return 0;
}


void TTransDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:
	// 获取拖入文件数量
	int iFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount > 1) return;

	// 获取拖入文件路径
	TCHAR tcFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcFilePath, _countof(tcFilePath));
	TRACE(_T("FilePath: %s\n"), tcFilePath);

	// 判断文件类型
	// ......

	// 载入文件
	if (FALSE == LoadFile(tcFilePath))
	{
		AfxMessageBox(_T("Failed in LoadFile(tcFilePath)"));
	}

	CDialog::OnDropFiles(hDropInfo);
}


BOOL TTransDlg::LoadFile(CString csPath)
{
	if (m_pFileData != NULL) delete[] m_pFileData;
	m_pFileData = NULL;

	CFile file;
	if (!file.Open(csPath, CFile::modeRead | CFile::shareDenyWrite)) return FALSE;
	m_lFileLength = file.GetLength();
	m_pFileData = new BYTE[(unsigned int)m_lFileLength];
	file.Read(m_pFileData, (unsigned int)m_lFileLength);
	file.Close();
	return TRUE;
}


void TTransDlg::OnCancel()
{
	// TODO:
	if (m_pFileData != NULL) delete[] m_pFileData;
	m_pFileData = NULL;
	if (m_SockConnH != NULL) closesocket(m_SockConnH);
	if (m_SockConnD != NULL) closesocket(m_SockConnD);

	// Clean wsaData
	WSACleanup();

	// Release Console
	FreeConsole();
	
	CDialog::OnCancel();
}
