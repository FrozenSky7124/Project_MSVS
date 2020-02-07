
// Demo_PMonDlg.cpp : ʵ���ļ�
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
		//printf(" �������ƣ�%s \n", pe32.szExeFile);
		if (stricmp(ProcessName, pe32.szExeFile) == 0)
		{
			//printf("����������");
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


// CDemo_PMonDlg �Ի���

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


// CDemo_PMonDlg ��Ϣ�������

BOOL CDemo_PMonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitMonitor();
	BOOL bUDPSocket = InitSocket();
	if (TRUE == bUDPSocket)
		SetDlgItemText(IDC_TEXT_udpstate, _T("UDP: ��"));
	else
		SetDlgItemText(IDC_TEXT_udpstate, _T("UDP: ��"));


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDemo_PMonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDemo_PMonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemo_PMonDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


void CDemo_PMonDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���������������

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

	// ��ʼ��SOCKET
	iRst = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (iRst != 0)
	{
		AfxMessageBox(_T("WSAStartup failed: %d\n"), iRst);
		return FALSE;
	}
	// ����UDP_Socket
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
		// �����쳣״̬����
		UINT iRst = 0;
		char *sendbuf = "CLOSE_04";
		iRst = sendto(m_socket, (char*)(sendbuf), lstrlen(sendbuf), 0, (SOCKADDR *)(&m_addrSendto), sizeof(m_addrSendto));
		if (iRst == 0)
			TRACE(_T("Error in sendto function. iRst == 0.\n"));
	}
	else
		SetDlgItemText(IDC_TEXT_State, _T("RUNNING"));
}
