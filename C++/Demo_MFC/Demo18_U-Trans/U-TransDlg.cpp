
// U-TransDlg.cpp : ʵ���ļ�
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


// UTransDlg �Ի���


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


// UTransDlg ��Ϣ�������

BOOL UTransDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:
	if (FALSE == InitSocket())
		AfxMessageBox(_T("InitSocket Error!"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void UTransDlg::OnPaint()
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR UTransDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void UTransDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:
	// ��ȡ�����ļ�����
	int iFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount > 1) return;

	// ��ȡ�����ļ�·��
	TCHAR tcFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcFilePath, _countof(tcFilePath));
	TRACE(_T("FilePath: %s\n"), tcFilePath);

	// �ж��ļ�����
	// ......

	// �����ļ�
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
	// �ر�UDP_Socket
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