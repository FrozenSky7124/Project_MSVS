
// IPLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Demo_ImageProcLite.h"
#include "IPLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// IPLDlg �Ի���


IPLDlg::IPLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO_IMAGEPROCLITE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void IPLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(IPLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// IPLDlg ��Ϣ�������

BOOL IPLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_NORMAL);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pCDCImgMain = GetDlgItem(IDC_StaticImgMain)->GetDC();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void IPLDlg::OnPaint()
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
HCURSOR IPLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void IPLDlg::OnOK()
{
	// Disable OnOK() Function.
	/*
	CDialogEx::OnOK();
	*/
}


void IPLDlg::OnDropFiles(HDROP hDropInfo)
{
	// Get count of files drop in
	UINT iFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount > 1) return;

	// Get file path
	TCHAR tcFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcFilePath, _countof(tcFilePath));
	TRACE(_T("FilePath: %s\n"), tcFilePath);

	// Filter file type
	// ......

	// FITS
	OpenFile_FITS(tcFilePath);

	CDialogEx::OnDropFiles(hDropInfo);
}


bool IPLDlg::OpenFile_FITS(LPCTSTR lpszPath)
{
	m_FSCFitsX.OpenFitsFile(lpszPath);

	// ���� BMP �ļ��������ĳ���(Byte)
	long lBmpDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight();
	// ���� BMP ���ݿռ�
	BYTE* pBmpData = (BYTE*) new BYTE[lBmpDataSize];
	memset(pBmpData, 0, lBmpDataSize);
	// ���� FITS ���ݵ� BMP ���ݿռ�
	int minPixelCount = 65535;
	int maxPixelCount = 0;
	double dRate = 1.0f / 65535 * 255;
	for (int i = 0; i < m_FSCFitsX.GetHeight(); i++)
	{
		for (int j = 0; j < m_FSCFitsX.GetWidth(); j++)
		{
			int iFValue = m_FSCFitsX.GetFitsData(j, i);
			if (iFValue < minPixelCount) minPixelCount = iFValue;
			if (iFValue > maxPixelCount) maxPixelCount = iFValue;

			iFValue = (float)(iFValue - 0) * dRate;
			if (iFValue < 0) iFValue = 0;
			if (iFValue > 255) iFValue = 255;
			BYTE tempValue;
			memset(&tempValue, 0, 1);
			tempValue = tempValue + BYTE(iFValue);
			// ���Ƶ� BMP �ļ�������
			memcpy_s(pBmpData + (m_FSCFitsX.GetHeight() - i - 1) * m_FSCFitsX.GetWidth() + j, 1, &tempValue, 1);
		}
	}
	TRACE(_T("\nminPixValue = %d\nmaxPixValue = %d\n"), minPixelCount, maxPixelCount);
	// ���ⲿ�������� BMP λͼ
	m_FSCDibX.LoadFromBuffer(pBmpData, m_FSCFitsX.GetWidth(), m_FSCFitsX.GetHeight(), 8);
	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(780, 780));
	delete[] pBmpData;
	return true;
}