
// UDP_ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDP_Client.h"
#include "UDP_ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUDP_ClientDlg �Ի���



CUDP_ClientDlg::CUDP_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UDP_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_wsaData = { 0 };
	m_socket = INVALID_SOCKET;
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


// CUDP_ClientDlg ��Ϣ�������

BOOL CUDP_ClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	srand((unsigned int)time(0));

	// ����ͼ��Ԥ���ؼ��Ĵ�С��λ��
	CRect viewRect;
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&viewRect);
	viewRect.SetRect(7, 7, 7 + 1000, 7 + 530);
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(viewRect);
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();

	// ��ʼ��SOCKET
	if (FALSE == InitSocket())
		AfxMessageBox(_T("InitSocket Error!"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUDP_ClientDlg::OnPaint()
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
HCURSOR CUDP_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUDP_ClientDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	TRACE(_T("OnOK\n"));

	//CDialogEx::OnOK();
}

void CUDP_ClientDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	TRACE(_T("OnCancel\n"));
	// �ر�UDP_Socket
	UINT iResult = closesocket(m_socket);
	if (iResult == SOCKET_ERROR)
		AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	WSACleanup();

	CDialogEx::OnCancel();
}

void CUDP_ClientDlg::OnDropFiles(HDROP hDropInfo)
{
	// ��ȡ�����ļ�����
	UINT iFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount > 1) return;

	// ��ȡ�����ļ�·��
	TCHAR tcFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcFilePath, _countof(tcFilePath));
	TRACE(_T("FilePath: %s\n"), tcFilePath);

	// �ж��ļ�����
	// ......

	// ��������
	m_cDibImage.LoadFromFile(tcFilePath);
	m_cDibImage.Draw(m_pCDC, CPoint(0, 0), CSize(550, 300));

	CDialogEx::OnDropFiles(hDropInfo);
}

BOOL CUDP_ClientDlg::InitSocket()
{
	UINT iResult = 0;
	// ��ʼ��SOCKET
	iResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (iResult != 0)
	{
		AfxMessageBox(_T("WSAStartup failed: %d\n"), iResult);
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
		m_addrSendto.sin_port = htons(21001);
		m_addrSendto.sin_addr.S_un.S_addr = inet_addr(_T("127.0.0.1"));
		return TRUE;
	}
}

void CUDP_ClientDlg::OnBnClickedButtonSend()
{
	UINT iResult;
	int dataLengthTotal = m_cDibImage.GetSize();
	int iCount;
	if (dataLengthTotal % MAX_UDPDATA == 0)
		iCount = dataLengthTotal / MAX_UDPDATA - 1;
	else
		iCount = dataLengthTotal / MAX_UDPDATA;

	UDP_PACKAGE dataPackage;
	dataPackage.flag = 1;
	dataPackage.width = m_cDibImage.GetWidth();
	dataPackage.height = m_cDibImage.GetHeight();
	dataPackage.bitCount = m_cDibImage.GetBitCount();
	dataPackage.dataTotal = m_cDibImage.GetSize();
	dataPackage.dataLength = MAX_UDPDATA;
	dataPackage.iTotal = iCount;

	for (int i = 0; i <= iCount; i++)
	{
		dataPackage.iNum = i;
		if (i == iCount)
		{
			dataPackage.dataLength = dataPackage.dataTotal % MAX_UDPDATA;
			memcpy_s(&dataPackage.data, dataPackage.dataLength, m_cDibImage.GetData() + i * MAX_UDPDATA, dataPackage.dataLength);
		}
		else
			memcpy_s(&dataPackage.data, MAX_UDPDATA, m_cDibImage.GetData() + i * MAX_UDPDATA, MAX_UDPDATA);

		iResult = sendto(m_socket, (char *)&dataPackage, sizeof(UDP_PACKAGE), 0, (SOCKADDR *)& m_addrSendto, sizeof(m_addrSendto));
		if (iResult == SOCKET_ERROR)
			AfxMessageBox(_T("sendto failed with error: %d\n"), WSAGetLastError());
		//Sleep(1);
	}
	TRACE(_T("Data Already Sended.\n"));
}