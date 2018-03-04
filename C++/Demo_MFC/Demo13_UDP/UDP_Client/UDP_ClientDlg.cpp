
// UDP_ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDP_Client.h"
#include "UDP_ClientDlg.h"
#include "afxdialogex.h"
#include "Dib_Ex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUDP_ClientDlg �Ի���



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


// CUDP_ClientDlg ��Ϣ�������

BOOL CUDP_ClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	// ����ͼ��Ԥ���ؼ��Ĵ�С��λ��
	CRect viewRect;
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&viewRect);
	viewRect.SetRect(7, 7, 7 + 550, 7 + 300);
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(viewRect);
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();

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
	CDib cDibImage;
	cDibImage.LoadFromFile(tcFilePath);
	cDibImage.Draw(m_pCDC, CPoint(0, 0), CSize(550, 300));
		
	CDialogEx::OnDropFiles(hDropInfo);
}


void CUDP_ClientDlg::OnBnClickedButtonSend()
{
	WSADATA wsaData = { 0 };
	UINT iResult = 0;
	
	// ��ʼ��
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		AfxMessageBox(_T("WSAStartup failed: %d\n"), iResult);
		return;
	}

	// ����UDP_Socket
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

		// �ر�UDP_Socket
		iResult = closesocket(clientSocket);
		if (iResult == SOCKET_ERROR)
			AfxMessageBox(_T("closesocket failed with error = %d\n"), WSAGetLastError());
	}

	WSACleanup();
}