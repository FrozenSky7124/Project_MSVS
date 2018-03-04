
// UDPDlg.cpp : ʵ���ļ�
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

	// ��ʼ��
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		AfxMessageBox(_T("WSAStartup failed: %d\n"), iResult);
		return 1;
	}

	// ����UDP_Socket
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

		// �ر�UDP_Socket
		iResult = closesocket(serverSocket);
		if (iResult == SOCKET_ERROR)
			AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	}

	WSACleanup();

	TRACE(_T("uiRecvThread Over!\n"));
	_endthreadex(0);
	return 0;
}

// UDPDlg �Ի���

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


// UDPDlg ��Ϣ�������

BOOL UDPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &uiRecvThread, this, 0, &m_iRecvThreadId);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void UDPDlg::OnPaint()
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
HCURSOR UDPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void UDPDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	TRACE(_T("OnOK\n"));

	//CDialogEx::OnOK();
}


void UDPDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	TRACE(_T("OnCancel\n"));
	
	CDialogEx::OnCancel();
}
