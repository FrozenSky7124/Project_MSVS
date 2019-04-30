
// UT_ClientDlg.cpp : ʵ���ļ�
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


// UT_ClientDlg �Ի���


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


// UT_ClientDlg ��Ϣ�������

BOOL UT_ClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:
	if (FALSE == InitSocket())
		AfxMessageBox(_T("InitSocket Error!"));

	// Start recv thread
	m_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &uiRecvThread, this, 0, &m_iRecvThread);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


// �����Ի��������С����ť������Ҫ����Ĵ��������Ƹ�ͼ�ꡣ
// ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó����⽫�ɿ���Զ���ɡ�
void UT_ClientDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}


//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR UT_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL UT_ClientDlg::InitSocket()
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
	// �ر�UDP_Socket
	UINT iRst = closesocket(m_socket);
	if (iRst == SOCKET_ERROR)
		AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	WSACleanup();

	CDialog::OnCancel();
}
