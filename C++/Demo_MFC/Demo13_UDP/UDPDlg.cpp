
// UDPDlg.cpp : ʵ���ļ�
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
		pThis->SetDlgItemTextA(IDC_STATIC_STATUS, _T("socket function succeeded!"));

		char RecvBuf[sizeof(UDPDlg::UDP_PACKAGE)];
		int BufLen = sizeof(RecvBuf);

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
			UDPDlg::UDP_PACKAGE* pTemp = (UDPDlg::UDP_PACKAGE*)RecvBuf;
			if (pTemp->flag == 1 && pTemp->iNum == 0)
			{
				// ����λͼÿ�е��ֽ���
				UINT uBmpLineByte = (pTemp->width * pTemp->bitCount + 31) / 8;
				uBmpLineByte = uBmpLineByte / 4 * 4;
				// ����λͼ�������ֽ���
				DWORD dwBmpDataSize = uBmpLineByte * pTemp->height;
				// Ϊλͼ���ݷ���ռ�
				pThis->m_pBmpData = new BYTE[dwBmpDataSize];
				memcpy_s(pThis->m_pBmpData, pTemp->dataLength, pTemp->data, pTemp->dataLength);
			}
			else if (pTemp->flag == 1 && pTemp->iNum > 0)
			{
				memcpy_s(pThis->m_pBmpData + pTemp->iNum * pTemp->dataLength, pTemp->dataLength, pTemp->data, pTemp->dataLength);
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
			// �������
			if (pTemp->iNum == pTemp->iTotal)
			{
				pThis->SetDlgItemTextA(IDC_STATIC_STATUS, _T("Transmission Complete!"));
				// �����8λBMP�ļ�����Ϊ��Ԥ������ɫ��
				if (pTemp->bitCount == 8)
					cDibImage.MakeRgbQuadMem(8);
				cDibImage.LoadFromBuffer(pThis->m_pBmpData, pTemp->width, pTemp->height, pTemp->bitCount);
				cDibImage.Draw(pThis->m_pCDC, CPoint(0, 0), CSize(1000, 530));
				cDibImage.~CDib();
				delete[] pThis->m_pBmpData;
				pThis->m_pBmpData = NULL;
			}
		}

		// �ر�UDP_Socket
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

// UDPDlg �Ի���

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

	// ����ͼ��Ԥ���ؼ��Ĵ�С��λ��
	CRect viewRect;
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&viewRect);
	viewRect.SetRect(7, 7, 7 + 1000, 7 + 530);
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(viewRect);
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();

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

BOOL UDPDlg::SendEndToRecvThread()
{
	UINT iResult = 0;
	WSADATA wsaData;
	SOCKET sendSocket;
	sockaddr_in addrSendTo;
	UDP_PACKAGE endPackage;

	// ��ʼ��SOCKET
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		AfxMessageBox(_T("WSAStartup failed: %d\n"), iResult);
		return FALSE;
	}
	// ����UDP_Socket
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
	// TODO: �ڴ����ר�ô����/����û���
	TRACE(_T("OnOK\n"));

	//CDialogEx::OnOK();
}

void UDPDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	TRACE(_T("OnCancel\n"));
	
	// ��ֹ�����߳�
	if (m_hRecvThread)
	{
		SendEndToRecvThread();
		::WaitForSingleObject(m_hRecvThread, INFINITE);
		CloseHandle(m_hRecvThread);
		m_hRecvThread = NULL;
	}

	CDialogEx::OnCancel();
}
