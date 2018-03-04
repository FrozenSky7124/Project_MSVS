
// UDP_ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDP_Client.h"
#include "UDP_ClientDlg.h"
#include "afxdialogex.h"
#include "Dib_Ex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUDP_ClientDlg 对话框



CUDP_ClientDlg::CUDP_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UDP_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDP_ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUDP_ClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CUDP_ClientDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CUDP_ClientDlg 消息处理程序

BOOL CUDP_ClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	// 调整图像预览控件的大小和位置
	CRect viewRect;
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&viewRect);
	viewRect.SetRect(7, 7, 7 + 550, 7 + 300);
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(viewRect);
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUDP_ClientDlg::OnPaint()
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
HCURSOR CUDP_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDP_ClientDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	TRACE(_T("OnOK\n"));

	//CDialogEx::OnOK();
}


void CUDP_ClientDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	TRACE(_T("OnCancel\n"));

	CDialogEx::OnCancel();
}


void CUDP_ClientDlg::OnDropFiles(HDROP hDropInfo)
{
	// 获取拖入文件数量
	UINT iFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount > 1) return;

	// 获取拖入文件路径
	TCHAR tcFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcFilePath, _countof(tcFilePath));
	TRACE(_T("FilePath: %s\n"), tcFilePath);

	// 判断文件类型
	// ......
	
	// 载入数据
	CDib cDibImage;
	cDibImage.LoadFromFile(tcFilePath);
	cDibImage.Draw(m_pCDC, CPoint(0, 0), CSize(550, 300));
		
	CDialogEx::OnDropFiles(hDropInfo);
}


void CUDP_ClientDlg::OnBnClickedButtonSend()
{
	WSADATA wsaData = { 0 };
	UINT iResult = 0;
	
	// 初始化
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		AfxMessageBox(_T("WSAStartup failed: %d\n"), iResult);
		return;
	}

	// 创建UDP_Socket
	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientSocket == INVALID_SOCKET)
		AfxMessageBox(_T("socket function failed with error = %d\n"), WSAGetLastError());
	else
	{
		AfxMessageBox(_T("socket function succeeded\n"));

		sockaddr_in addr_in;
		addr_in.sin_family = AF_INET;
		addr_in.sin_port = htons(21001);
		addr_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		iResult = sendto(clientSocket, _T("Hi!"), _countof(_T("Hi!")), 0, (SOCKADDR *)& addr_in, sizeof(addr_in));
		if (iResult == SOCKET_ERROR)
			AfxMessageBox(_T("sendto failed with error: %d\n"), WSAGetLastError());

		// 关闭UDP_Socket
		iResult = closesocket(clientSocket);
		if (iResult == SOCKET_ERROR)
			AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	}

	WSACleanup();
}