
// U-TransDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo18_U-Trans.h"
#include "U-TransDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UINT WINAPI uiSendThread(LPVOID lpParam)
{
	UTransDlg * pThis = static_cast<UTransDlg*>(lpParam);
	ULONGLONG lFileLength = pThis->m_lFileLength;
	UINT iRst = 0;

	int iPkgCount = lFileLength / BLOCKSIZE;
	if (lFileLength % BLOCKSIZE != 0) iPkgCount++;

	pThis->m_addrSendto.sin_addr.S_un.S_addr = inet_addr(_T("192.168.1.58"));

	for (int i = 0; i < iPkgCount; i++)
	{
		int iDataSend = BLOCKSIZE;
		if (i == iPkgCount - 1) iDataSend = lFileLength - i * BLOCKSIZE;
		iRst = sendto(pThis->m_socket, (char*)(pThis->m_pFileData + i * BLOCKSIZE), iDataSend, 0, (SOCKADDR *)(&(pThis->m_addrSendto)), sizeof(pThis->m_addrSendto));
		if (iRst == 0)
			TRACE(_T("Error in sendto function. iRst == 0.\n"));
		//Sleep(10);
	}
	return 0x00;
}


// UTransDlg 对话框


UTransDlg::UTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEMO18_UTRANS_DIALOG, pParent)
	, m_socket(INVALID_SOCKET)
	, m_pFileData(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void UTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(UTransDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(ID_BtnSend, &UTransDlg::OnBnClickedSend)
	ON_BN_CLICKED(ID_BtnQuit, &UTransDlg::OnBnClickedQuit)
END_MESSAGE_MAP()


// UTransDlg 消息处理程序

BOOL UTransDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:
	if (FALSE == InitSocket())
		AfxMessageBox(_T("InitSocket Error!"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void UTransDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR UTransDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void UTransDlg::OnDropFiles(HDROP hDropInfo)
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


void UTransDlg::OnBnClickedSend()
{
	// Start send thread
	m_hSendThread = (HANDLE)_beginthreadex(NULL, 0, &uiSendThread, this, 0, &m_iSendThread);
}


BOOL UTransDlg::InitSocket()
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
		m_addrSendto.sin_family = AF_INET;
		m_addrSendto.sin_port = htons(21001);
		m_addrSendto.sin_addr.S_un.S_addr = inet_addr(_T("127.0.0.1"));
		
		sockaddr_in addr_recv;
		addr_recv.sin_family = AF_INET;
		addr_recv.sin_port = htons(21011);
		addr_recv.sin_addr.s_addr = htonl(INADDR_ANY);
		iRst = bind(m_socket, (SOCKADDR *)& addr_recv, sizeof(addr_recv));
		if (iRst != 0)
		{
			AfxMessageBox(_T("bind failed with error %d\n"), WSAGetLastError());
			return FALSE;
		}
		return TRUE;
	}
}


BOOL UTransDlg::LoadFile(CString csPath)
{
	if (m_pFileData != NULL) delete[] m_pFileData;
	m_pFileData = NULL;

	CFile file;
	if (!file.Open(csPath, CFile::modeRead | CFile::shareDenyWrite)) return FALSE;
	m_lFileLength = file.GetLength();
	m_pFileData = new BYTE[m_lFileLength];
	file.Read(m_pFileData, m_lFileLength);
	file.Close();
	return TRUE;
}


void UTransDlg::OnBnClickedQuit()
{
	// TODO:
	if (m_pFileData != NULL) delete[] m_pFileData;
	m_pFileData = NULL;
	// 关闭UDP_Socket
	UINT iRst = closesocket(m_socket);
	if (iRst == SOCKET_ERROR)
		AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	WSACleanup();

	CDialog::OnCancel();
}


void UTransDlg::OnCancel()
{
	// TODO:
}