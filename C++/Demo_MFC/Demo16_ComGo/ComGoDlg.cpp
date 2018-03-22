
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

// �����߳�
UINT WINAPI uiTransThread(LPVOID lpParam)
{
	ComGoDlg* pThis = (ComGoDlg*)lpParam;
	CListCtrl* pListCtrl = (CListCtrl*)pThis->m_pCListData;
	BYTE* pByteData = pThis->m_pByteData;
	DWORD dwBytesSent;
	UINT iRow = pThis->m_iRow;
	BYTE bSend[ROWLENGTH_BYTE];
	memset(bSend, 0, ROWLENGTH_BYTE);

	DWORD iTime1 = GetTickCount(); //��ʼʱ��
	while (pThis->m_iCur < iRow)
	{
		memcpy_s(bSend, ROWLENGTH_BYTE, pByteData + pThis->m_iCur * ROWLENGTH_BYTE, ROWLENGTH_BYTE);
		WriteFile(pThis->m_hComm, bSend, ROWLENGTH_BYTE, &dwBytesSent, NULL);
		if (dwBytesSent != ROWLENGTH_BYTE) break; //��⴫���쳣
		if (pThis->m_bPause == TRUE) break;
		pListCtrl->SetItemText(pThis->m_iCur, 2, _T("SEND"));
		pThis->m_iCur++;
		Sleep(pThis->m_dwInterval);
	}
	DWORD iTime2 = GetTickCount(); //����ʱ��

	CString tempStr;
	if (dwBytesSent != ROWLENGTH_BYTE)
	{
		tempStr.Format(_T("��⵽�����쳣����ֹ���䣡\n��ǰ�Ѵ������� %d ���κ�ʱ %dms"), pThis->m_iCur, iTime2 - iTime1);
		MessageBox(pThis->GetSafeHwnd(), tempStr, _T("QAQ"), MB_OK | MB_ICONERROR);
	}
	else
	{
		if (pThis->m_bPause == TRUE)
			tempStr.Format(_T("������ͣ�� �Ѵ������� %d ���κ�ʱ %dms"), pThis->m_iCur, iTime2 - iTime1);
		else
			tempStr.Format(_T("������ɣ� �Ѵ������� %d ���κ�ʱ %dms"), pThis->m_iCur, iTime2 - iTime1);
	}
	pThis->SetDlgItemText(IDC_STATIC_STATUS, tempStr);

	// ��������
	if (pThis->m_iCur == iRow)
	{
		pThis->m_iCur = 0;
		pThis->m_bPause = TRUE;
		pThis->SetDlgItemText(IDC_BUTTON_SEND, _T("��������"));
	}

	return 0;
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
	m_hTransThread = NULL;
	m_uIDTransThread = NULL;
	m_iRow = 0;
	m_iCur = 0;
	m_bCommOpen = FALSE;
	m_bPause = TRUE;
	m_dwInterval = 20;
	m_BaudRate = 115200;
	m_ByteSize = 8;
	m_Parity = NOPARITY;
	m_StopBits = ONESTOPBIT;

	// �ؼ�����
	m_pCListData = (CListCtrl*)GetDlgItem(IDC_LIST_DATA);
	m_pCListData->InsertColumn(0, _T(" ���"), LVCFMT_LEFT, 75);
	m_pCListData->InsertColumn(1, _T(" ����"), LVCFMT_LEFT, 350);
	m_pCListData->InsertColumn(2, _T(" ״̬"), LVCFMT_LEFT, 100);
	//m_pCListData->SetBkColor(RGB(205, 226, 252));
	//m_pCListData->SetTextBkColor(RGB(205, 226, 252));
	m_pCListData->SetExtendedStyle(m_pCListData->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); //����CListCtrl�ؼ�����ʽ

	((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->SetCurSel(3);
	SetDlgItemInt(IDC_COMBO_BTL, m_BaudRate);
	SetDlgItemInt(IDC_COMBO_SJW, m_ByteSize);
	((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->SetCurSel(0);

	SetDlgItemInt(IDC_EDIT_TIME, m_dwInterval);

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
	// �رն˿�
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	// �������
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	if (m_pByteData != NULL)
	{
		delete[] m_pByteData;
		m_pByteData = NULL;
	}

	CDialog::OnCancel();
}


void ComGoDlg::OnBnClickedButtonLoad()
{
	// �������
	m_pCListData->DeleteAllItems();
	m_iCur = 0;
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	if (m_pByteData != NULL)
	{
		delete[] m_pByteData;
		m_pByteData = NULL;
	}

	// �ؼ�����
	m_bPause = TRUE;
	SetDlgItemText(IDC_BUTTON_SEND, _T("��������"));

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
	if (dwFileLength % ROWLENGTH != 0) //�����ļ���ʽ�쳣
	{
		AfxMessageBox(_T("��⵽�ļ������쳣����˶����ݸ�ʽ��"));
		loadFile.Close();
		return;
	}

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
	m_iRow = dwFileLength / ROWLENGTH;
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
	if ((m_hComm != NULL) && (m_bCommOpen == FALSE))
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("�˿�״̬�쳣���޷��򿪣�"));
		return;
	}
	if ((m_hComm != NULL) && (m_bCommOpen == TRUE)) //���˿�״̬Ϊ�Ѵ򿪣���رն˿�
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
		SetDlgItemText(IDC_BUTTON_OPENCOM, _T("�����˿�"));
		((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(TRUE);
		return;
	}

	// ��ȡ�˿ں�
	((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->GetWindowText(m_PortNo);

	m_hComm = CreateFile(m_PortNo, //�˿�����(COMx)
		GENERIC_READ | GENERIC_WRITE, //�˿�����Ϊ�ɶ�д
		0, //�˿��豸���뱻��ռ�Եķ���
		NULL, //�ް�ȫ����
		OPEN_EXISTING, //�˿��豸����ʹ��OPEN_EXISTING����
		FILE_ATTRIBUTE_NORMAL, //ͬ��ʽ I/O
		0); //���ڶ˿��豸���Դ˲�������Ϊ0
	if (m_hComm == INVALID_HANDLE_VALUE) //����ö˿ڲ����ڻ�����������һ��Ӧ�ó���ʹ�ã����ʧ��
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("�˿��޷��򿪣��ö˿ڲ����ڻ���������Ӧ�ó���ռ�á�"));
		MessageBoxEx(AfxGetMainWnd()->m_hWnd, _T("�˿��޷��򿪣�\n�ö˿ڲ����ڻ���������Ӧ�ó���ռ�á�"), _T("QAQ"), MB_OK | MB_ICONERROR, 0);
		m_hComm = NULL;
		return;
	}
	
	// �˿�����
	GetCommState(m_hComm, &m_dcb); //��ȡ�ö˿ڵ�Ĭ�ϲ���
	m_dcb.BaudRate = m_BaudRate = GetDlgItemInt(IDC_COMBO_BTL); //�޸Ĳ�����
	m_dcb.ByteSize = m_ByteSize = GetDlgItemInt(IDC_COMBO_SJW); //ͨ���ֽ�λ��
	m_dcb.Parity = m_Parity = ((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->GetCurSel(); //��żУ�鷽����EVENPARITY��żУ�飬NOPARITY����У�飬MARKPARITY�����У�飬ODDPARITY����У��
	m_dcb.StopBits = m_StopBits = ((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->GetCurSel(); //ֹͣλ��λ����ONESTOPBIT��1λֹͣλ��TWOSTOPBITS��2λֹͣλ��ONE5STOPBITS��1.5λֹͣλ
	m_dcb.fParity = 1; //��żУ��λ
	UINT iResult = SetCommState(m_hComm, &m_dcb); //�������ò���
	if (iResult)
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("�˿ڴ򿪳ɹ���"));
		m_bCommOpen = TRUE;
		SetDlgItemText(IDC_BUTTON_OPENCOM, _T("�رն˿�"));
		((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(FALSE);
	}
	else
	{
		// ���˿������쳣��δ������ϵͳ���ܣ�������ؼ���ֵ
		GetCommState(m_hComm, &m_dcb);
		SetDlgItemInt(IDC_COMBO_BTL, m_BaudRate = m_dcb.BaudRate);
		SetDlgItemInt(IDC_COMBO_SJW, m_ByteSize = m_dcb.ByteSize);
		((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->SetCurSel(m_Parity = m_dcb.Parity);
		((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->SetCurSel(m_StopBits = m_dcb.StopBits);
		m_fParity = m_dcb.fParity;

		CString tempStrRst;
		tempStrRst.Format(_T("�˿ڴ򿪳ɹ���\n���ڲ��������쳣�������ã� \n[������] %d \n[����λ] %d \n[У��λ] %d \n[ֹͣλ] %d"),
			m_dcb.BaudRate, m_dcb.ByteSize, m_dcb.Parity, m_dcb.StopBits);
		SetDlgItemText(IDC_STATIC_STATUS, tempStrRst);
		MessageBoxEx(AfxGetMainWnd()->m_hWnd, tempStrRst, _T("QwQ"), MB_OK | MB_ICONINFORMATION, 0);
		m_bCommOpen = TRUE;
		SetDlgItemText(IDC_BUTTON_OPENCOM, _T("�رն˿�"));
		((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(FALSE);
	}
}

void ComGoDlg::OnBnClickedButtonSend()
{
	if (NULL == m_hComm)
	{
		AfxMessageBox(_T("�˿�״̬�쳣���޷����䣡"));
		return;
	}
	if (NULL == m_pByteData)
	{
		AfxMessageBox(_T("����״̬�쳣���޷����䣡"));
		return;
	}
	if (m_bPause == FALSE) //�����ڴ��䣬��...
	{
		m_bPause = TRUE;
		SetDlgItemText(IDC_BUTTON_SEND, _T("������������"));
		return;
	}

	UINT i = 0;
	while (i < m_iRow)
	{
		m_pCListData->SetItemText(i++, 2, _T("")); //����ǰ���б�״̬����Ϊ��
	}

	m_dwInterval = GetDlgItemInt(IDC_EDIT_TIME); //��ȡ������

	GetCommState(m_hComm, &m_dcb); //��ȡ�ö˿ڵ�Ĭ�ϲ���
	m_dcb.BaudRate = m_BaudRate = GetDlgItemInt(IDC_COMBO_BTL); //�޸Ĳ�����
	m_dcb.ByteSize = m_ByteSize = GetDlgItemInt(IDC_COMBO_SJW); //ͨ���ֽ�λ��
	m_dcb.Parity = m_Parity = ((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->GetCurSel(); //��żУ�鷽��
	m_dcb.StopBits = m_StopBits = ((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->GetCurSel(); //ֹͣλ��λ��
	m_dcb.fParity = 1; //��żУ��λ
	UINT iResult = SetCommState(m_hComm, &m_dcb); //�������ò���

	if ((!iResult) && (m_bCommOpen))
	{
		// ���˿������쳣��δ������ϵͳ���ܣ�������ؼ���ֵ
		GetCommState(m_hComm, &m_dcb);
		SetDlgItemInt(IDC_COMBO_BTL, m_BaudRate = m_dcb.BaudRate);
		SetDlgItemInt(IDC_COMBO_SJW, m_ByteSize = m_dcb.ByteSize);
		((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->SetCurSel(m_Parity = m_dcb.Parity);
		((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->SetCurSel(m_StopBits = m_dcb.StopBits);
		m_fParity = m_dcb.fParity;

		CString tempStrRst;
		tempStrRst.Format(_T("���ڲ��������쳣�������ã� \n[������] %d \n[����λ] %d \n[У��λ] %d \n[ֹͣλ] %d"),
			m_dcb.BaudRate, m_dcb.ByteSize, m_dcb.Parity, m_dcb.StopBits);
		SetDlgItemText(IDC_STATIC_STATUS, tempStrRst);
		MessageBoxEx(AfxGetMainWnd()->m_hWnd, tempStrRst, _T("QwQ"), MB_OK | MB_ICONINFORMATION, 0);
	}

	m_hTransThread = (HANDLE)_beginthreadex(NULL, 0, &uiTransThread, this, 0, &m_uIDTransThread);
	m_bPause = !m_bPause;
	SetDlgItemText(IDC_BUTTON_SEND, _T("��ͣ��������"));
}