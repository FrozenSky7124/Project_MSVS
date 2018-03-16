
// ComGoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo16_ComGo.h"
#include "ComGoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ComGoDlg �Ի���

// CHAR2INT
UINT ConvChar2Int(CHAR _In_CHAR_)
{
	UINT _Out_UINT_;
	if (_In_CHAR_ >= '0' && _In_CHAR_ <= '9')
	{
		_Out_UINT_ = _In_CHAR_ - '0';
	}
	else
	{
		_Out_UINT_ = _In_CHAR_ - 'A' + 10;
	}
	return _Out_UINT_;
}

ComGoDlg::ComGoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEMO16_COMGO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ComGoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ComGoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &ComGoDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_OPENCOM, &ComGoDlg::OnBnClickedButtonOpencom)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &ComGoDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// ComGoDlg ��Ϣ�������

BOOL ComGoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// ��Ա��������
	m_pData = NULL;
	m_pByteData = NULL;
	m_hComm = NULL;

	m_pCListData = (CListCtrl*)GetDlgItem(IDC_LIST_DATA);
	m_pCListData->InsertColumn(0, _T("���"), LVCFMT_LEFT, 75);
	m_pCListData->InsertColumn(1, _T("����"), LVCFMT_LEFT, 300);
	//m_pCListData->SetBkColor(RGB(205, 226, 252));
	//m_pCListData->SetTextBkColor(RGB(205, 226, 252));
	m_pCListData->SetExtendedStyle(m_pCListData->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); //����CListCtrl�ؼ�����ʽ

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ComGoDlg::OnPaint()
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
HCURSOR ComGoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void ComGoDlg::OnOK()
{

	//CDialog::OnOK();
}


void ComGoDlg::OnCancel()
{
	// �رմ���
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	if (m_pData != NULL)
	{
		delete[] m_pData;
	}
	if (m_pByteData != NULL)
	{
		delete[] m_pByteData;
	}

	CDialog::OnCancel();
}


void ComGoDlg::OnBnClickedButtonLoad()
{
	// ��ȡ��ǰ����Ŀ¼
	TCHAR curDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curDir);

	// ����CFileDialog���ļ�
	CString filePath;
	CString defaultDir = curDir; //Ĭ��Ŀ¼����
	CString whiteList = _T("�����ļ� (*.txt)|*.txt||"); //�ļ���չ��������
	CFileDialog openFileDlg(TRUE, curDir, NULL, OFN_HIDEREADONLY | OFN_READONLY, whiteList, this);
	if (IDOK == openFileDlg.DoModal())
	{
		filePath = openFileDlg.GetPathName(); //��ȡ�ļ�·��
		SetDlgItemText(IDC_STATIC_STATUS, filePath); //״̬����ʾ
	}
	else
	{
		return;
	}

	// ��ȡ�ļ�
	CFile loadFile;
	CFileException e_loadFile;
	if (!loadFile.Open(filePath, CFile::modeRead, &e_loadFile))
	{
		AfxMessageBox(_T("�޷����ļ��� Error: %d"), e_loadFile.m_cause);
		return;
	}
	ULONGLONG dwFileLength = loadFile.GetLength();
	TRACE(_T("FileLength: %d Bytes\n"), dwFileLength);


	m_pData = new CHAR[dwFileLength]; //����ռ�
	if (NULL == m_pData)
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("�ռ�����쳣�� TextData"));
		return;
	}
	memset(m_pData, 0, dwFileLength);

	if (loadFile.Read(m_pData, dwFileLength) != dwFileLength) //��ȡ����
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("�ļ���ȡ�쳣��"));
		delete[] m_pData;
		loadFile.Close();
		return;
	}
	loadFile.Close();

	// ת���ı�����ΪBYTE����
	UINT iRow = dwFileLength / ROWLENGTH; //��������
	m_pByteData = new BYTE[iRow * ROWLENGTH_BYTE]; //����ռ�
	if (NULL == m_pByteData)
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("�ռ�����쳣�� ByteData"));
		return;
	}
	memset(m_pByteData, 0, iRow * ROWLENGTH_BYTE);
	UINT index = 0;
	for (UINT tRow = 0; tRow < iRow; tRow++ )
	{
		for (UINT tCol = 0; tCol < ROWLENGTH_BYTE; tCol++ )
		{
			UINT tempH = ConvChar2Int(m_pData[tRow * ROWLENGTH + 3 * tCol + 0]);
			UINT tempL = ConvChar2Int(m_pData[tRow * ROWLENGTH + 3 * tCol + 1]);
			m_pByteData[index] = tempH << 4 | tempL;
			index++;
		}
	}

	CString tempStr;
	CHAR tempChr[ROWLENGTH + 1]; //���ڴ洢һ���ı�����ʱ��������1�ֽ������ַ�����β
	memset(tempChr, 0, ROWLENGTH + 1);
	for (UINT i = 0; i < iRow; i++)
	{
		tempStr.Format(_T("%4d"), i + 1);
		m_pCListData->InsertItem(i, tempStr);
		memcpy_s(tempChr, ROWLENGTH, m_pData + i * ROWLENGTH, ROWLENGTH);
		tempStr.Format(_T("%s"), tempChr);
		m_pCListData->SetItemText(i, 1, tempStr);
	}
	SetDlgItemText(IDC_STATIC_STATUS, _T("���ݶ�ȡ��ɣ�"));
}


void ComGoDlg::OnBnClickedButtonOpencom()
{
	if (m_hComm != NULL)
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("����״̬�쳣���޷��򿪣�"));
		return;
	}
	m_hComm = CreateFile("COM4", //��������(COMx)
		GENERIC_READ | GENERIC_WRITE, //��������Ϊ�ɶ�д
		0, //�����豸���뱻��ռ�Եķ���
		NULL, //�ް�ȫ����
		OPEN_EXISTING, //�����豸����ʹ��OPEN_EXISTING����
		FILE_ATTRIBUTE_NORMAL, //ͬ��ʽ I/O
		0); //���ڴ����豸���Դ˲�������Ϊ0
	if (m_hComm == INVALID_HANDLE_VALUE) //����ô��ڲ����ڻ�����������һ��Ӧ�ó���ʹ�ã����ʧ��
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("�����޷��򿪣�"));
		m_hComm = NULL;
		return;
	}
	
	GetCommState(m_hComm, &m_dcb); //��ȡ�ö˿ڵ�Ĭ�ϲ���
	m_dcb.BaudRate = 115200; //�޸Ĳ�����
	m_dcb.fParity = 1; //��żУ��λ
	m_dcb.Parity = NOPARITY; //��żУ�鷽����EVENPARITY��żУ�飬NOPARITY����У�飬MARKPARITY�����У�飬ODDPARITY����У��
	m_dcb.ByteSize = 8; //ͨ���ֽ�λ��
	m_dcb.StopBits = ONESTOPBIT; //ֹͣλ��λ����ONESTOPBIT��1λֹͣλ��TWOSTOPBITS��2λֹͣλ��ONE5STOPBITS��1.5λֹͣλ
	UINT iResult = SetCommState(m_hComm, &m_dcb); //�������ò���
	if (iResult)
		SetDlgItemText(IDC_STATIC_STATUS, _T("���ڴ򿪳ɹ���"));
}

void ComGoDlg::OnBnClickedButtonSend()
{
	char lpBuffer[] = "FrozenSky's COMM!";
	DWORD nNumberOfBytesToWrite = strlen(lpBuffer);
	DWORD nBytesSent;
	WriteFile(m_hComm, lpBuffer, nNumberOfBytesToWrite, &nBytesSent, NULL);
	CString tempStr;
	tempStr.Format(_T("���ݷ�����ɣ� ������ %d �ֽڡ�"), nBytesSent);
	SetDlgItemText(IDC_STATIC_STATUS, tempStr);
}