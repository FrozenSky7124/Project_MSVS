
// IPLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Demo_ImageProcLite.h"
#include "IPLDlg.h"
#include <iostream>
#include <algorithm>

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
	DDX_Control(pDX, IDC_EditFitsInfo, m_EditFitsInfo);
	DDX_Control(pDX, IDC_EditImgInfo, m_EditImgInfo);
}

BEGIN_MESSAGE_MAP(IPLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_MenuFile_Open, &IPLDlg::OnMenu_File_Open)
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

	m_FontStandard.CreatePointFont(110, _T("����"));
	m_EditFitsInfo.SetFont(&m_FontStandard);
	m_EditImgInfo.SetFont(&m_FontStandard);

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

	ComputeGrayLimit(0.432506, 0.92);

	// ���� BMP �ļ��������ĳ���(Byte)
	long lBmpDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight();
	// ���� BMP ���ݿռ�
	BYTE* pBmpData = (BYTE*) new BYTE[lBmpDataSize];
	memset(pBmpData, 0, lBmpDataSize);
	// ���� FITS ���ݵ� BMP ���ݿռ�
	int minPixelCount = 65535;
	int maxPixelCount = 0;
	double dRate = 1.0f / (float)(m_iHighPixelCount - m_iLowPixelCount) * 255.;
	//double dRate = 1.0f / (float)(3444 - 2760) * 255.;
	for (int i = 0; i < m_FSCFitsX.GetHeight(); i++)
	{
		for (int j = 0; j < m_FSCFitsX.GetWidth(); j++)
		{
			int iFValue = m_FSCFitsX.GetFitsData(j, i);
			if (iFValue < minPixelCount) minPixelCount = iFValue;
			if (iFValue > maxPixelCount) maxPixelCount = iFValue;

			iFValue = ((float)iFValue - (float)m_iLowPixelCount) * dRate;
			if (iFValue < 0) iFValue = 0;
			if (iFValue > 255) iFValue = 255;
			BYTE tempValue;
			memset(&tempValue, 0, 1);
			tempValue = tempValue + BYTE(iFValue);
			// ���Ƶ� BMP �ļ�������
			memcpy_s(pBmpData + (m_FSCFitsX.GetHeight() - i - 1) * m_FSCFitsX.GetWidth() + j, 1, &tempValue, 1);
		}
	}
	m_iMinPixelCount = minPixelCount;
	m_iMaxPixelCount = maxPixelCount;
	// ���ⲿ�������� BMP λͼ
	m_FSCDibX.LoadFromBuffer(pBmpData, m_FSCFitsX.GetWidth(), m_FSCFitsX.GetHeight(), 8);
	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(780, 780));
	delete[] pBmpData;
	ListFitsHDU();
	ListImgInfo();
	return true;
}


void IPLDlg::ListFitsHDU()
{
	int iLength;
	m_EditFitsInfo.SetSel(0, -1);
	m_EditFitsInfo.ReplaceSel(_T(""));
	for (int i = 0; i < m_FSCFitsX.GetHDUNum(); i++)
	{
		CString tempStr;
		tempStr.Format(_T("%-8s = %s\n"), m_FSCFitsX.GetHDUKey(i), m_FSCFitsX.GetHDUValue(i));
		iLength = m_EditFitsInfo.GetWindowTextLength();
		m_EditFitsInfo.SetSel(iLength, iLength);
		m_EditFitsInfo.ReplaceSel(tempStr);
	}
}


void IPLDlg::ListImgInfo()
{
	int iLength;
	CString tempStr;

	m_EditImgInfo.SetSel(0, -1);
	m_EditImgInfo.ReplaceSel(_T(""));

	tempStr.Format(_T("MinPixelCount = %d\n"), m_iMinPixelCount);
	iLength = m_EditImgInfo.GetWindowTextLength();
	m_EditImgInfo.SetSel(iLength, iLength);
	m_EditImgInfo.ReplaceSel(tempStr);

	tempStr.Format(_T("MaxPixelCount = %d\n"), m_iMaxPixelCount);
	iLength = m_EditImgInfo.GetWindowTextLength();
	m_EditImgInfo.SetSel(iLength, iLength);
	m_EditImgInfo.ReplaceSel(tempStr);

	tempStr.Format(_T("Low Percentile  = %d\n"), m_iLowPixelCount);
	iLength = m_EditImgInfo.GetWindowTextLength();
	m_EditImgInfo.SetSel(iLength, iLength);
	m_EditImgInfo.ReplaceSel(tempStr);

	tempStr.Format(_T("High Percentile = %d\n"), m_iHighPixelCount);
	iLength = m_EditImgInfo.GetWindowTextLength();
	m_EditImgInfo.SetSel(iLength, iLength);
	m_EditImgInfo.ReplaceSel(tempStr);
}


void IPLDlg::ComputeGrayLimit(double dLowPer, double dHighPer)
{
	// Compute Low Percentile Pixel Value & High Percentile Pixel Value
	int iLow; //Low percentile pixel value
	int iHigh; //High percentile pixel value
	long lPixelNum = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight();
	int iSampleNum = lPixelNum >= 10000 ? 10000 : lPixelNum; //Sample Count (Max: 10000)
	int iLowNum = iSampleNum * dLowPer; //0.432506 default
	int iHighNum = iSampleNum * dHighPer; //0.97725 default
	double dRate = lPixelNum * 1.0f / iSampleNum;
	int* pSampleData = new int[iSampleNum];
	for (int i = 0; i < iSampleNum; i++)
	{
		pSampleData[i] = m_FSCFitsX.GetFitsData(i * dRate);
	}
	std::nth_element(pSampleData, pSampleData + iLowNum, pSampleData + iSampleNum);
	iLow = pSampleData[iLowNum];
	std::nth_element(pSampleData, pSampleData + iHighNum, pSampleData + iSampleNum);
	iHigh = pSampleData[iHighNum];
	delete[] pSampleData;
	m_iLowPixelCount = iLow;
	m_iHighPixelCount = iHigh;
}


void IPLDlg::OnMenu_File_Open()
{
	// TODO:
	CString strFilePath;
	static TCHAR strFileFilter[] = _T("FITS File (*.fits;*.fit)|*.fits;*.fit||");
	CFileDialog selectFileDlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFileFilter, this);
	if (IDOK == selectFileDlg.DoModal())
	{
		strFilePath = selectFileDlg.GetPathName();
		OpenFile_FITS(strFilePath);
	}
}
