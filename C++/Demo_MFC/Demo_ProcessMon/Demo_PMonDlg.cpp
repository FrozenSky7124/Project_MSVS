
// Demo_PMonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo_ProcessMon.h"
#include "Demo_PMonDlg.h"
#include "afxdialogex.h"

#include <windows.h>
#include <tlhelp32.h>
#include "psapi.h"

#pragma comment(lib,"ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// GLOBAL

bool FindProcess(const char *ProcessName)
{
	int i = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		i = 11;
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		//printf(" 进程名称：%s \n", pe32.szExeFile);
		if (stricmp(ProcessName, pe32.szExeFile) == 0)
		{
			//printf("进程运行中");
			i = 1;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	if (i == 1) 
		return true;
	if (i == 11) 
		return false;
	return false;
}


// CDemo_PMonDlg 对话框

CDemo_PMonDlg::CDemo_PMonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO_PROCESSMON_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemo_PMonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDemo_PMonDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_COMMAND(IDCANCEL, &CDemo_PMonDlg::OnIDCancel)
END_MESSAGE_MAP()


// CDemo_PMonDlg 消息处理程序

BOOL CDemo_PMonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitMonitor();
	BOOL bUDPSocket = InitSocket();
	if (TRUE == bUDPSocket)
		SetDlgItemText(IDC_TEXT_udpstate, _T("UDP: √"));
	else
		SetDlgItemText(IDC_TEXT_udpstate, _T("UDP: ×"));


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDemo_PMonDlg::OnPaint()
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
HCURSOR CDemo_PMonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemo_PMonDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CDemo_PMonDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1701:
		SearchProcess();
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDemo_PMonDlg::OnIDCancel()
{
	// TODO: 在此添加命令处理程序代码

	// Kill Timer 1701
	if (false == KillTimer(1701))
		MessageBoxEx(this->GetSafeHwnd(), _T("Error in KillTimer."), _T("QAQ"), MB_OK | MB_ICONINFORMATION, 0);

	// Close Socket
	UINT iRst = closesocket(m_socket);
	if (iRst == SOCKET_ERROR)
		AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	WSACleanup();

	CDialogEx::OnCancel();
}


void CDemo_PMonDlg::InitMonitor()
{
	SetTimer(1701, 10000, NULL);
}


BOOL CDemo_PMonDlg::InitSocket()
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
		m_addrSendto.sin_port = htons(LISTENPORT);
		m_addrSendto.sin_addr.S_un.S_addr = inet_addr(_T("192.168.100.205"));
		return TRUE;
	}
}


void CDemo_PMonDlg::SearchProcess()
{
	bool bFind = false;
	bFind = FindProcess(_T("SODD.exe"));
	if (bFind == false)
	{
		SetDlgItemText(IDC_TEXT_State, _T("CLOSED"));
		// 发送异常状态报告
		UINT iRst = 0;
		char *sendbuf = "CLOSE_04";
		iRst = sendto(m_socket, (char*)(sendbuf), lstrlen(sendbuf), 0, (SOCKADDR *)(&m_addrSendto), sizeof(m_addrSendto));
		if (iRst == 0)
			TRACE(_T("Error in sendto function. iRst == 0.\n"));
	}
	else
		SetDlgItemText(IDC_TEXT_State, _T("RUNNING"));
}
