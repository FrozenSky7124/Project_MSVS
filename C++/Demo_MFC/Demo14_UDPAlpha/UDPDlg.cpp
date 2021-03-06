
// UDPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo13_UDP.h"
#include "UDPDlg.h"
#include "afxdialogex.h"
#include "Dib_Ex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// RecvThread
UINT WINAPI uiRecvThread(LPVOID lpParam)
{
	UDPDlg* pThis = (UDPDlg*)lpParam;

	WSADATA wsaData = { 0 };
	UINT iResult = 0;

	CDib cDibImage;

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
		pThis->SetDlgItemTextA(IDC_STATIC_STATUS, _T("socket function succeeded!"));

		char RecvBuf[sizeof(UDPDlg::UDP_PACKAGE)];
		int BufLen = sizeof(UDPDlg::UDP_PACKAGE);

		char cConfirmPackage[sizeof(UDPDlg::UDP_CONFIRM)];
		int ConfirmPkgLen = sizeof(UDPDlg::UDP_CONFIRM);
		UDPDlg::UDP_CONFIRM* ConfirmPackage = (UDPDlg::UDP_CONFIRM*)cConfirmPackage;

		sockaddr_in addr_sender;
		int iSenderAddrSize = sizeof(addr_sender);

		sockaddr_in addr_recv;
		addr_recv.sin_family = AF_INET;
		addr_recv.sin_port = htons(21001);
		addr_recv.sin_addr.s_addr = htonl(INADDR_ANY);

		iResult = bind(serverSocket, (SOCKADDR *)& addr_recv, sizeof(addr_recv));
		if (iResult != 0)
			AfxMessageBox(_T("bind failed with error %d\n"), WSAGetLastError());

		while ((iResult = recvfrom(serverSocket, RecvBuf, BufLen, 0, (SOCKADDR *)& addr_sender, &iSenderAddrSize)) != SOCKET_ERROR)
		{
			//DWORD dwBmpDataSize;
			UDPDlg::UDP_PACKAGE* pTemp = (UDPDlg::UDP_PACKAGE*)RecvBuf;
			if (pTemp->flag == 1 && pTemp->iNum == 0)
			{
				// 计算位图每行的字节数
				//UINT uBmpLineByte = (pTemp->width * pTemp->bitCount + 31) / 8;
				//uBmpLineByte = uBmpLineByte / 4 * 4;
				// 计算位图数据区字节数
				//dwBmpDataSize = uBmpLineByte * pTemp->height;
				// 为位图数据分配空间
				pThis->m_pBmpData = new BYTE[pTemp->dataTotal * sizeof(BYTE)];
				memset(pThis->m_pBmpData, 0, pTemp->dataTotal * sizeof(BYTE));
				memcpy_s(pThis->m_pBmpData, pTemp->dataLength, &pTemp->data, pTemp->dataLength);
			}
			else if (pTemp->flag == 1 && pTemp->iNum > 0)
			{
				memcpy_s(pThis->m_pBmpData + pTemp->iNum * MAX_UDPDATA, pTemp->dataLength, &pTemp->data, pTemp->dataLength);
			}
			else if (pTemp->flag == 0)
			{
				if (pThis->m_pBmpData) delete[] pThis->m_pBmpData;
				pThis->m_pBmpData = NULL;
				break;
			}
			else
			{
				pThis->SetDlgItemTextA(IDC_STATIC_STATUS, _T("Transmission Error!"));
				if (pThis->m_pBmpData)
				{
					delete[] pThis->m_pBmpData;
					pThis->m_pBmpData = NULL;
				}
			}

			// 回复确认包
			ConfirmPackage->flag = 1;
			ConfirmPackage->iRecvNum = pTemp->iNum;
			addr_sender.sin_family = AF_INET;
			addr_sender.sin_port = htons(21011);

			iResult = sendto(serverSocket, (char *)ConfirmPackage, ConfirmPkgLen, 0, (SOCKADDR *)&addr_sender, sizeof(addr_sender));
			if (iResult == SOCKET_ERROR)
				AfxMessageBox(_T("ConfirmPackage sendto failed with error: %d\n"), WSAGetLastError());
			
			// 传输完成
			if (pTemp->iNum == pTemp->iTotal)
			{
				pThis->SetDlgItemTextA(IDC_STATIC_STATUS, _T("Transmission Complete!"));
				// 如果是8位BMP文件，则为其预创建颜色表
				if (pTemp->bitCount == 8)
					cDibImage.MakeRgbQuadMem(8);
				cDibImage.LoadFromBuffer(pThis->m_pBmpData, pTemp->width, pTemp->height, pTemp->bitCount);
				cDibImage.Draw(pThis->m_pCDC, CPoint(0, 0), CSize(pThis->m_viewWidth, pThis->m_viewHeight));
				cDibImage.~CDib();
				if (pThis->m_pBmpData)
				{
					delete [] pThis->m_pBmpData;
					pThis->m_pBmpData = NULL;
				}
			}
		}

		// 关闭UDP_Socket
		iResult = closesocket(serverSocket);
		if (iResult == SOCKET_ERROR)
			AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	}

	WSACleanup();

	cDibImage.~CDib();

	TRACE(_T("uiRecvThread Over!\n"));
	_endthreadex(0);
	return 0;
}

// UDPDlg 对话框

UDPDlg::UDPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO13_UDP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pBmpData = NULL; 
	m_pCDC = NULL;
	m_hRecvThread = NULL;
	m_iRecvThreadId = NULL;
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
	
	// 调整主窗体大小
	CRect mainRect;
	GetClientRect(&mainRect);

	// 调整图像预览控件的大小和位置
	CRect viewRect;
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&viewRect);
	viewRect.SetRect(7, 7, mainRect.right - 7, mainRect.bottom - 7 - 32);
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(viewRect);
	m_viewWidth = viewRect.Width();
	m_viewHeight = viewRect.Height();

	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();

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

BOOL UDPDlg::SendEndToRecvThread()
{
	UINT iResult = 0;
	WSADATA wsaData;
	SOCKET sendSocket;
	sockaddr_in addrSendTo;
	UDP_PACKAGE endPackage;

	// 初始化SOCKET
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		AfxMessageBox(_T("WSAStartup failed: %d\n"), iResult);
		return FALSE;
	}
	// 创建UDP_Socket
	sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sendSocket == INVALID_SOCKET)
	{
		AfxMessageBox(_T("socket function failed with error = %d\n"), WSAGetLastError());
		WSACleanup();
		return FALSE;
	}
	addrSendTo.sin_family = AF_INET;
	addrSendTo.sin_port = htons(21001);
	addrSendTo.sin_addr.S_un.S_addr = inet_addr(_T("127.0.0.1"));
	endPackage.flag = 0;

	iResult = sendto(sendSocket, (char *)&endPackage, sizeof(UDP_PACKAGE), 0, (SOCKADDR *)& addrSendTo, sizeof(addrSendTo));
	if (iResult == SOCKET_ERROR)
	{
		AfxMessageBox(_T("sendto failed with error: %d\n"), WSAGetLastError());
		closesocket(sendSocket);
		WSACleanup();
		return FALSE;
	}
	
	closesocket(sendSocket);
	WSACleanup();
	return TRUE;
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
	
	// 终止监听线程
	if (m_hRecvThread)
	{
		SendEndToRecvThread();
		::WaitForSingleObject(m_hRecvThread, INFINITE);
		CloseHandle(m_hRecvThread);
		m_hRecvThread = NULL;
	}

	CDialogEx::OnCancel();
}
