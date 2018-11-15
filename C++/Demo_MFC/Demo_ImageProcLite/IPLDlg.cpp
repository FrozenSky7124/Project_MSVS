
// IPLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Demo_ImageProcLite.h"
#include "IPLDlg.h"
#include <iostream>
#include <cmath>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// IPLDlg �Ի���


IPLDlg::IPLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEMO_IMAGEPROCLITE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iProcConfig = 0;
	m_ipFitsDataTmp = NULL;
}

void IPLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditFitsInfo, m_EditFitsInfo);
	DDX_Control(pDX, IDC_EditImgInfo, m_EditImgInfo);
}

BEGIN_MESSAGE_MAP(IPLDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_MenuFile_Open, &IPLDlg::OnMenu_File_Open)
	ON_BN_CLICKED(IDC_BtnLinearGE, &IPLDlg::OnBnClickedBtnLinearGE)
	ON_BN_CLICKED(IDC_BtnReset, &IPLDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BtnBinaryConv, &IPLDlg::OnBnClickedBtnBinaryConv)
	ON_COMMAND(ID_MenuFile_Save, &IPLDlg::OnMenu_File_Save)
	ON_COMMAND(ID_MenuFile_Quit, &IPLDlg::OnMenu_File_Quit)
	ON_COMMAND(ID_MenuAnalyse_OutStand, &IPLDlg::OnMenu_Analyse_OutStand)
	ON_COMMAND(ID_MenuOpenCV_OpenImg, &IPLDlg::OnMenu_OpenCV_OpenImg)
	ON_COMMAND(ID_MenuOpenCV_Threshold, &IPLDlg::OnMenu_OpenCV_Threshold)
	ON_COMMAND(ID_MenuOpenCV_NolinearGE, &IPLDlg::OnMenu_OpenCV_NolinearGE)
	ON_COMMAND(ID_MenuOpenCV_EqualH, &IPLDlg::OnMenu_OpenCV_EqualizeHist)
	ON_BN_CLICKED(IDC_BtnProc, &IPLDlg::OnBnClickedBtnProc)
END_MESSAGE_MAP()


// IPLDlg ��Ϣ�������

BOOL IPLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_NORMAL);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pCDCImgMain = GetDlgItem(IDC_StaticImgMain)->GetDC();

	m_FontStandard.CreatePointFont(90, _T("����"));
	m_EditFitsInfo.SetFont(&m_FontStandard);
	m_EditImgInfo.SetFont(&m_FontStandard);

	RECT editRect;
	GetDlgItem(IDC_EditLowPer)->GetClientRect(&editRect);
	OffsetRect(&editRect, 0, 4);
	GetDlgItem(IDC_EditLowPer)->SendMessage(EM_SETRECT, 0, (LPARAM)&editRect);
	GetDlgItem(IDC_EditHighPer)->GetClientRect(&editRect);
	OffsetRect(&editRect, 0, 4);
	GetDlgItem(IDC_EditHighPer)->SendMessage(EM_SETRECT, 0, (LPARAM)&editRect);
	GetDlgItem(IDC_EditBinThreshold)->GetClientRect(&editRect);
	OffsetRect(&editRect, 0, 4);
	GetDlgItem(IDC_EditBinThreshold)->SendMessage(EM_SETRECT, 0, (LPARAM)&editRect);

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
		CDialog::OnPaint();
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


void IPLDlg::OnCancel()
{
	// Release Fits temp data
	if (m_ipFitsDataTmp) delete[] m_ipFitsDataTmp;
	m_ipFitsDataTmp = NULL;
	// Release OpenCV Mat
	m_cvMat.release();
	m_cvMat8U.release();

	CDialog::OnCancel();
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

	CDialog::OnDropFiles(hDropInfo);
}


bool IPLDlg::OpenFile_FITS(LPCTSTR lpszPath)
{
	// ��������
	if (m_ipFitsDataTmp) delete[] m_ipFitsDataTmp;
	m_ipFitsDataTmp = NULL;
	// ��ȡ FITS �ļ�
	m_FSCFitsX.OpenFitsFile(lpszPath);
	// ���� FITS ������ʱ�洢�ռ�
	long lDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight() * sizeof(int);
	m_ipFitsDataTmp = new int[lDataSize];
	memcpy_s(m_ipFitsDataTmp, lDataSize, m_FSCFitsX.GetFitsDataPtr(), lDataSize);
	// ��ȡ FITS �ļ� PixelCount ���ֵ����Сֵ
	m_iMinPixelCount = m_FSCFitsX.GetMinPixelCount();
	m_iMaxPixelCount = m_FSCFitsX.GetMaxPixelCount();
	// ���� OpenCV 16λ�洢�ռ�
	m_cvMat.release();
	m_cvMat.create(m_FSCFitsX.GetHeight(), m_FSCFitsX.GetWidth(), CV_16U);
	for (int i = 0; i < m_FSCFitsX.GetHeight() * m_FSCFitsX.GetWidth(); i++)
	{
		unsigned short usTmp = unsigned short(*(m_ipFitsDataTmp + i));
		m_cvMat.at<unsigned short>(i) = usTmp;
	}
	m_cvMat8U.release();
	m_cvMat.convertTo(m_cvMat8U, CV_8U, 1 / 255.0, 0.0);
	// ����Ĭ�ϲ����������ԻҶ���ֵ
	ComputeGrayLimit(0.432506, 0.977250);
	// ���� FSC_DibX λͼ����
	CreateDibX();

	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	ListFitsHDU();
	ListImgInfo();
	return true;
}


bool IPLDlg::CreateDibX()
{
	// ���� BMP �ļ��������ĳ���(Byte)
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	long lBmpDataSize = iWidth * iHeight;
	// ���� BMP ���ݿռ�
	BYTE* pBmpData = (BYTE*) new BYTE[lBmpDataSize];
	memset(pBmpData, 0, lBmpDataSize);
	double dRate = 1.0f / (float)(m_iHighPixelCount - m_iLowPixelCount) * 255.;
	//double dRate = 1.0f / (float)(3444 - 2760) * 255.;
	// ���� FITS ���ݵ� BMP ���ݿռ�
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			//int iFValue = m_FSCFitsX.GetFitsData(j, i);
			int iFValue = *(m_ipFitsDataTmp + i * iWidth + j);
			// ʹ���Զ��Ҷ���ֵ�������ԻҶ���ǿ
			iFValue = ((float)iFValue - (float)m_iLowPixelCount) * dRate;
			//iFValue = 255 * double(iFValue) / 65535;
			if (iFValue < 0) iFValue = 0;
			if (iFValue > 255) iFValue = 255;
			BYTE tempValue;
			memset(&tempValue, 0, 1);
			tempValue = tempValue + BYTE(iFValue);
			// ���Ƶ� BMP �ļ�������
			memcpy_s(pBmpData + (iHeight - i - 1) * iWidth + j, 1, &tempValue, 1);
		}
	}
	// ���ⲿ�������� BMP λͼ
	m_FSCDibX.LoadFromBuffer(pBmpData, iWidth, iHeight, 8);
	delete[] pBmpData;
	return true;
}


bool IPLDlg::CreateDibX_BinaryConv(int iBinThreshoud)
{
	// ���� BMP �ļ��������ĳ���(Byte)
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	long lBmpDataSize = iWidth * iHeight;
	// ���� BMP ���ݿռ�
	BYTE* pBmpData = (BYTE*) new BYTE[lBmpDataSize];
	memset(pBmpData, 0, lBmpDataSize);
	// ���� FITS ���ݵ� BMP ���ݿռ�
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			int iFValue = *(m_ipFitsDataTmp + i * iWidth + j);
			if (iFValue >= iBinThreshoud) iFValue = 255;
			else iFValue = 0;
			BYTE tempValue;
			memset(&tempValue, 0, 1);
			tempValue = tempValue + BYTE(iFValue);
			// ���Ƶ� BMP �ļ�������
			memcpy_s(pBmpData + (iHeight - i - 1) * iWidth + j, 1, &tempValue, 1);
		}
	}
	// ���ⲿ�������� BMP λͼ
	m_FSCDibX.LoadFromBuffer(pBmpData, iWidth, iHeight, 8);
	delete[] pBmpData;
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


void IPLDlg::OutputContoursFile(TCHAR * pszFileName, std::vector<std::vector<cv::Point>>& contours)
{
	CFile output;
	CFileException e;
	if (!output.Open(pszFileName, CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE(_T("Output file could not be opened %d\n"), e.m_cause);
	}
	CString csOutput = _T("");
	for (int i = 0; i < contours.size(); i++)
	{
		CString csTmp;
		for (int j = 0; j < contours[i].size(); j++)
		{
			csTmp.Format(_T("%s[%4d,%4d] "), csTmp, contours[i][j].x, contours[i][j].y);
		}
		csOutput.Format(_T("%s%3d %s\n"), csOutput, i, csTmp);
	}
	output.Write(csOutput, csOutput.GetLength());
	output.Flush();
	output.Close();
}


void IPLDlg::ComputeGrayLimit(double dLowPer, double dHighPer)
{
	CString csTmp;
	csTmp.Format(_T("%.4f"), dLowPer * 100);
	SetDlgItemText(IDC_EditLowPer, csTmp);
	csTmp.Format(_T("%.4f"), dHighPer * 100);
	SetDlgItemText(IDC_EditHighPer, csTmp);
	// Compute Low Percentile Pixel Value & High Percentile Pixel Value
	int iLow; //Low percentile pixel value
	int iHigh; //High percentile pixel value
	long lPixelNum = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight();
	int iSampleNum = lPixelNum >= 10000 ? 10000 : lPixelNum; //Sample Count (Max: 10000)
	int iLowNum = iSampleNum * dLowPer; //0.432506 default
	int iHighNum = iSampleNum * dHighPer; //0.97725 default
	if (iLowNum < 0) iLowNum = 0;
	if (iHighNum >= iSampleNum) iHighNum = iSampleNum - 1;
	double dRate = lPixelNum * 1.0f / iSampleNum;
	int* pSampleData = new int[iSampleNum];
	for (int i = 0; i < iSampleNum; i++)
	{
		//pSampleData[i] = m_FSCFitsX.GetFitsData(i * dRate);
		pSampleData[i] = *(m_ipFitsDataTmp + int(i * dRate));
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


void IPLDlg::OnBnClickedBtnLinearGE()
{
	// TODO:
	CString csTmp;
	GetDlgItemText(IDC_EditLowPer, csTmp);
	double dLowPercent  = atof(csTmp) / 100.0;
	GetDlgItemText(IDC_EditHighPer, csTmp);
	double dHighPercent = atof(csTmp) / 100.0;
	if (dLowPercent < 0) dLowPercent = 0.00000000;
	if (dHighPercent > 1) dHighPercent = 1.00000000;
	// �����Զ�������������ԻҶ���ֵ
	ComputeGrayLimit(dLowPercent, dHighPercent);
	// ���� FSC_DibX λͼ����
	CreateDibX();

	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	ListFitsHDU();
	ListImgInfo();
}


void IPLDlg::OnBnClickedBtnBinaryConv()
{
	// TODO:
	int iBinThreshoud = GetDlgItemInt(IDC_EditBinThreshold);
	// ���� FSC_DibX λͼ����
	CreateDibX_BinaryConv(iBinThreshoud);

	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	ListFitsHDU();
	ListImgInfo();
}


void IPLDlg::OnBnClickedBtnReset()
{
	// TODO:
	// ��������
	if (m_ipFitsDataTmp) delete[] m_ipFitsDataTmp;
	m_ipFitsDataTmp = NULL;
	// ���� FITS ������ʱ�洢�ռ�
	long lDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight() * sizeof(int);
	m_ipFitsDataTmp = new int[lDataSize];
	memcpy_s(m_ipFitsDataTmp, lDataSize, m_FSCFitsX.GetFitsDataPtr(), lDataSize);
	// ��ȡ FITS �ļ� PixelCount ���ֵ����Сֵ
	m_iMinPixelCount = m_FSCFitsX.GetMinPixelCount();
	m_iMaxPixelCount = m_FSCFitsX.GetMaxPixelCount();
	// ����Ĭ�ϲ����������ԻҶ���ֵ
	ComputeGrayLimit(0.432506, 0.977250);
	// ���� FSC_DibX λͼ����
	CreateDibX();

	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	ListFitsHDU();
	ListImgInfo();
}


void IPLDlg::OnMenu_File_Save()
{
	// TODO:
	CString csSavePath(_T(".\\SaveImg.bmp"));
	if (m_FSCDibX.IsValid())
		m_FSCDibX.SaveToFile(csSavePath);
}


void IPLDlg::OnMenu_File_Quit()
{
	// TODO:
	this->OnCancel();
}


void IPLDlg::OnMenu_Analyse_OutStand()
{
	// TODO:
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();

	int iAve = m_FSCFitsX.GetAveragePixelCount();
	int iMin = INT_MAX;
	int iMax = INT_MIN;
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			int *pValue = m_ipFitsDataTmp + i * iWidth + j;
			int iValue = *pValue;
			int S1 = pow(iValue - iAve, 2);
			*pValue = S1;
			if (S1 < iMin) iMin = S1;
			if (S1 > iMax) iMax = S1;
		}
	}
	int iFSMin = INT_MAX;
	int iFSMax = INT_MIN;
	for (long i = 0; i < iHeight * iWidth; i++)
	{
		int *pValue = m_ipFitsDataTmp + i;
		*pValue = 65535 * double(*pValue - iMin) / (*pValue - iMax);
		//*pValue = 65535 * double(*pValue - iMax) / (*pValue - iMin);
		//if (*pValue < 0) *pValue = 0;
		//if (*pValue > 65535) *pValue = 65535;
	}


	CString tempStr;
	tempStr.Format(_T("AvePixelCount = %d\n"), iAve);
	int iLength = m_EditImgInfo.GetWindowTextLength();
	m_EditImgInfo.SetSel(iLength, iLength);
	m_EditImgInfo.ReplaceSel(tempStr);
	// ���� BMP �ļ��������ĳ���(Byte)
	long lBmpDataSize = iWidth * iHeight;
	// ���� BMP ���ݿռ�
	BYTE* pBmpData = (BYTE*) new BYTE[lBmpDataSize];
	memset(pBmpData, 0, lBmpDataSize);
	// ���� FITS ���ݵ� BMP ���ݿռ�
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			int FS1 = *(m_ipFitsDataTmp + i * iWidth + j);
			FS1 = 255 * double(FS1) / 65535;
			BYTE tempValue;
			memset(&tempValue, 0, 1);
			tempValue = tempValue + BYTE(FS1);
			// ���Ƶ� BMP �ļ�������
			memcpy_s(pBmpData + (iHeight - i - 1) * iWidth + j, 1, &tempValue, 1);
		}
	}
	// ���ⲿ�������� BMP λͼ
	m_FSCDibX.LoadFromBuffer(pBmpData, iWidth, iHeight, 8);
	delete[] pBmpData;
	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	return;
}


void IPLDlg::OnMenu_OpenCV_OpenImg()
{
	using namespace std;
	// TODO:
	CString strFilePath;
	CString strFileType;
	static TCHAR strFileFilter[] = _T("Image File (*.bmp;*.jpg;*.png;*.fit;*.fits)|*.bmp;*.jpg;*.png;*.fit;*.fits||");
	CFileDialog selectFileDlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFileFilter, this);
	if (IDOK == selectFileDlg.DoModal())
	{
		strFilePath = selectFileDlg.GetPathName();
		// �����ļ���
		int iLength = strFilePath.GetLength();
		for (int i = 0; i < iLength; i++)
		{
			if (strFilePath.GetAt(iLength - i - 1) == '.')
			{
				iLength = i;
				break;
			}
		}
		strFileType = strFilePath.Right(iLength);
		if ((strFileType == "fit") || (strFileType == "fits"))
		{
			// ��������
			if (m_ipFitsDataTmp) delete[] m_ipFitsDataTmp;
			m_ipFitsDataTmp = NULL;
			// ��ȡ FITS �ļ�
			m_FSCFitsX.OpenFitsFile(strFilePath);
			// ���� FITS ������ʱ�洢�ռ�
			long lDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight() * sizeof(int);
			m_ipFitsDataTmp = new int[lDataSize];
			memcpy_s(m_ipFitsDataTmp, lDataSize, m_FSCFitsX.GetFitsDataPtr(), lDataSize);
			// ��ȡ FITS �ļ� PixelCount ���ֵ����Сֵ
			m_iMinPixelCount = m_FSCFitsX.GetMinPixelCount();
			m_iMaxPixelCount = m_FSCFitsX.GetMaxPixelCount();
			cv::Mat cvMa(m_FSCFitsX.GetHeight(), m_FSCFitsX.GetWidth(), CV_16U);
			for (int i = 0; i < m_FSCFitsX.GetHeight() * m_FSCFitsX.GetWidth(); i++)
			{
				unsigned short usTmp = unsigned short(*(m_ipFitsDataTmp + i));
				cvMa.at<unsigned short>(i)= usTmp;
			}
			cv::namedWindow("OpenCV Viewer", CV_WINDOW_NORMAL);
			cv::resizeWindow("OpenCV Viewer", m_FSCFitsX.GetWidth() / 4, m_FSCFitsX.GetHeight() / 4);
			cv::Mat cvGrayMa;
			cvMa.convertTo(cvGrayMa, CV_8U, 1/255.0, 0.0);
			cv::imshow("OpenCV Viewer", cvGrayMa);
			cv::waitKey(0);
		}
		else
		{
			cv::Mat cvImg;
			cvImg = cv::imread(strFilePath.GetString());
			cv::imshow("OpenCV Viewer", cvImg);
			cv::waitKey(0);
		}
	}
}


void IPLDlg::OnMenu_OpenCV_Threshold()
{
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	// OpenCV Threshold Process
	cv::Mat cvGrayMat;
	cv::Mat cvTmpMat;
	cvGrayMat.create(iHeight, iWidth, CV_8U);
	cvTmpMat.create(iHeight, iWidth, CV_8U);
	m_cvMat.convertTo(cvGrayMat, CV_8U, 1 / 255.0, 0.0);
	cv::threshold(cvGrayMat, cvTmpMat, 100, 255, CV_THRESH_BINARY);
	cv::namedWindow("OpenCV Viewer", CV_WINDOW_NORMAL);
	cv::resizeWindow("OpenCV Viewer", iWidth / 4, iHeight / 4);
	cv::imshow("OpenCV Viewer", cvTmpMat);
	cv::waitKey(0);
}


void IPLDlg::OnMenu_OpenCV_NolinearGE()
{
	int m = m_iMinPixelCount;
	int M = m_iMaxPixelCount;
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	m_cvMat8U.release();
	m_cvMat8U.create(m_FSCFitsX.GetHeight(), iWidth, CV_8U);
	for (int i = 0; i < iHeight * iWidth; i++)
	{
		unsigned short usTmp = unsigned short(*(m_ipFitsDataTmp + i));
		if (usTmp <= m)
			m_cvMat8U.at<unsigned char>(i) = 0;
		else if (usTmp >= M)
			m_cvMat8U.at<unsigned char>(i) = 255;
		else
			m_cvMat8U.at<unsigned char>(i) = unsigned char((255.0 / log(256)) * log(1 + 255 * (usTmp - m) / (M - m)));
	}
	cv::namedWindow("OpenCV Viewer", CV_WINDOW_NORMAL);
	cv::resizeWindow("OpenCV Viewer", iWidth / 4, iHeight / 4);
	cv::imshow("OpenCV Viewer", m_cvMat8U);
	cv::waitKey(0);
}


void IPLDlg::OnMenu_OpenCV_EqualizeHist()
{
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	cv::Mat tmpMat(iHeight, iWidth, CV_8U);
	cv::equalizeHist(m_cvMat8U, tmpMat);
	cv::namedWindow("OpenCV Viewer", CV_WINDOW_NORMAL);
	cv::resizeWindow("OpenCV Viewer", iWidth / 4, iHeight / 4);
	cv::imshow("OpenCV Viewer", tmpMat);
	cv::waitKey(0);
}


void IPLDlg::OnBnClickedBtnProc()
{
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	m_cvMat8U.release();
	m_cvMat8U.create(iHeight, iWidth, CV_8U);
	for (int i = 0; i < iHeight * iWidth; i++)
	{
		unsigned short usTmp = unsigned short(*(m_ipFitsDataTmp + i));
		m_cvMat8U.at<unsigned char>(i) = usTmp / 255.0;
	}
	cv::Mat tmpMat;
	cv::threshold(m_cvMat8U, tmpMat, 5, 255, CV_THRESH_BINARY);
	// ��̬ѧ�˲�������ͼ��
	//cv::Mat element10(10, 10, CV_8U, cv::Scalar(255));
	//cv::morphologyEx(tmpMat, tmpMat, cv::MORPH_CLOSE, element10);
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20));
	cv::dilate(tmpMat, tmpMat, element);

	cv::namedWindow("OpenCV Viewer", CV_WINDOW_NORMAL);
	cv::resizeWindow("OpenCV Viewer", iWidth / 4, iHeight / 4);
	cv::imshow("OpenCV Viewer", tmpMat);
	cv::imwrite("./ImgProc_Ori.bmp", tmpMat);

	// ��ȡ����
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(tmpMat, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	// ��������
	/*
	std::vector<std::vector<cv::Point>> contoursFinal;
	int iCMin = 10;
	int iCMax = 70;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > iCMin && contours[i].size() < iCMax)
			contoursFinal.push_back(contours[i]);
	}
	*/
	std::vector<cv::Point> vCenterPoint;
	for (int i = 0; i < contours.size(); i++)
	{
		int ixm = INT_MAX, ixM = INT_MIN;
		int iym = INT_MAX, iyM = INT_MIN;
		for (int j = 0; j < contours[i].size(); j++)
		{
			if (contours[i][j].x < ixm) ixm = contours[i][j].x;
			if (contours[i][j].x > ixM) ixM = contours[i][j].x;
			if (contours[i][j].y < iym) iym = contours[i][j].y;
			if (contours[i][j].y > iyM) iyM = contours[i][j].y;
		}
		vCenterPoint.push_back(cv::Point((ixM - ixm) / 2, (iyM - iym) / 2));
	}

	// �����������
	TCHAR* pszFileName = _T("ContoursFile.txt");
	OutputContoursFile(pszFileName, contours);

	cv::Mat showMat(m_cvMat8U.size(), CV_8U, cv::Scalar(255));
	cv::drawContours(showMat, contours, -1, 0, 2);
	// �����������
	for (int i = 0; i < contours.size(); i++)
	{
		cv::Rect cvRec = cv::boundingRect(contours[i]);
		cv::rectangle(showMat, cvRec, cv::Scalar(0), 2);
	}
	TRACE(_T("\nNumber of contours: %d\n"), contours.size());

	cv::namedWindow("OpenCV Viewer 2", CV_WINDOW_NORMAL);
	cv::resizeWindow("OpenCV Viewer 2", iWidth / 4, iHeight / 4);
	cv::imshow("OpenCV Viewer 2", showMat);
	cv::imwrite("./ImgProc_Show.bmp", showMat);
	cv::waitKey(0);
}
