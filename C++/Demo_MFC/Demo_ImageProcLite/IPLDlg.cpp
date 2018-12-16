
// IPLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Demo_ImageProcLite.h"
#include "IPLDlg.h"
#include "shlwapi.h"
#include <iostream>
#include <cmath>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cv;
using namespace Gdiplus;


// Global

bool CompareFitsTime(CString & cs1, CString & cs2)
{
	CString sub1 = cs1.Right(21);
	CString sub2 = cs2.Right(21);
	long long time1 = atoll(sub1);
	long long time2 = atoll(sub2);
	if (time1 > time2)
		return false;
	else
		return true;
}

bool CompareIKMap(ObjIKMap & ik1, ObjIKMap & ik2)
{
	return ik1.KValue < ik2.KValue;
}


UINT WINAPI uiFunc_Proc(LPVOID lpParam)
{
	IPLDlg* pIPLDLG = (IPLDlg*)lpParam;
	std::vector<CString>* lpvFitsName = &(pIPLDLG->m_vFitsName);
	std::vector<cv::Point2f>* & vpPixelPre = pIPLDLG->m_vpPixelPre;
	std::vector<cv::Point2f>* & vpPixelCur = pIPLDLG->m_vpPixelCur;
	std::vector<cv::Point2f>* & vpRDPre = pIPLDLG->m_vpRDPre;
	std::vector<cv::Point2f>* & vpRDCur = pIPLDLG->m_vpRDCur;
	double* & pcRA_Pre = pIPLDLG->m_pcRA_Pre;
	double* & pcRA_Cur = pIPLDLG->m_pcRA_Cur;
	double* & pcDEC_Pre = pIPLDLG->m_pcDEC_Pre;
	double* & pcDEC_Cur = pIPLDLG->m_pcDEC_Cur;
	int & iPreObjNID = pIPLDLG->m_iPreObjNID;
	int & iCurObjNID = pIPLDLG->m_iCurObjNID;
	int & iImgInQueue = pIPLDLG->m_iImgInQueue;
	int & iImgIndex = pIPLDLG->m_iImgIndex;

	for (; iImgIndex < lpvFitsName->size(); iImgIndex++)
	{
		pIPLDLG->UpdateUI();
		// Get OBJ-NID
		iCurObjNID = atoi(lpvFitsName->at(iImgIndex).Left(6));
		// New?
		if (iPreObjNID != iCurObjNID)
		{
			// Reset Ptr
			vpPixelPre = &(pIPLDLG->vPixel[1]);
			vpPixelCur = &(pIPLDLG->vPixel[0]);
			vpRDPre = &(pIPLDLG->vRaDec[1]);
			vpRDCur = &(pIPLDLG->vRaDec[0]);
			pcRA_Pre = &(pIPLDLG->cRA[1]);
			pcRA_Cur = &(pIPLDLG->cRA[0]);
			pcDEC_Pre = &(pIPLDLG->cDEC[1]);
			pcDEC_Cur = &(pIPLDLG->cDEC[0]);
			// Release Data
			iPreObjNID = iCurObjNID;
			iImgInQueue = 1;
			vpPixelPre->clear();
			vpPixelCur->clear();
			vpRDPre->clear();
			vpRDCur->clear();
		}
		else
		{
			// Swap Ptr
			std::swap(vpPixelPre, vpPixelCur);
			std::swap(vpRDPre, vpRDCur);
			std::swap(pcRA_Pre, pcRA_Cur);
			std::swap(pcDEC_Pre, pcDEC_Cur);
			vpPixelCur->clear();
			vpRDCur->clear();
			iImgInQueue++;
		}
		// Load Fits
		CString csFitsPath = pIPLDLG->m_csFitsDir + _T("\\") + lpvFitsName->at(iImgIndex);
		pIPLDLG->Proc_LoadFile(csFitsPath);
		pIPLDLG->DrawMarkPre();
		// Outstanding conversion
		pIPLDLG->Proc_OutStand();
		// Extract posible objects
		pIPLDLG->Proc_ExtractObject(iImgIndex, vpPixelCur, vpRDCur, pcRA_Cur, pcDEC_Cur);
		// Search main object
		if (iImgInQueue >= 2)
		{
			pIPLDLG->Proc_SearchObject(iImgIndex, pcRA_Pre, pcRA_Cur, pcDEC_Pre, pcDEC_Cur, vpPixelPre, vpPixelCur, vpRDPre, vpRDCur);
			pIPLDLG->DrawMarkCur();
		}

		// Stop and Release when ThreadStatus is 0;
		if (pIPLDLG->m_iThreadStatus == 0) break;
		// Pause 
		if (pIPLDLG->m_iThreadStatus == 2)
		{
			iImgIndex++; //Avoid next proc using same img.
			break;
		}
	}
	// Proc End.
	if (iImgIndex == lpvFitsName->size())
	{
		pIPLDLG->m_iThreadStatus = 2;
		//Set BtnSTStart to START
		pIPLDLG->m_BtnSTStart.SetIcon(IDI_ICON_START, 48, 48);
		MessageBoxEx(pIPLDLG->GetSafeHwnd(), _T("Auto Process Over!\nNow Paused, waiting for next."), _T("QwQ"), MB_OK | MB_ICONINFORMATION, 0);
		pIPLDLG->PrintLog(_T("Auto Proc Paused. Waiting for next..."));
	}
	return 0;
}


// IPLDlg 对话框


IPLDlg::IPLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEMO_IMAGEPROCLITE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iLowPixelCount = -1;
	m_iHighPixelCount = -1;
	// Auto Proc Data
	m_uipFitsDataTmp = NULL;
	m_iImgCount		= 0;
	m_iThreadStatus = 0;
	m_vpPixelPre	= &(vPixel[1]);
	m_vpPixelCur	= &(vPixel[0]);
	m_vpRDPre		= &(vRaDec[1]);
	m_vpRDCur		= &(vRaDec[0]);
	m_pcRA_Pre		= &(cRA[1]);
	m_pcRA_Cur		= &(cRA[0]);
	m_pcDEC_Pre		= &(cDEC[1]);
	m_pcDEC_Cur		= &(cDEC[0]);
	m_iPreObjNID	= 0;
	m_iCurObjNID	= 0;
	m_iImgInQueue	= 0;
	m_iImgIndex		= 0;
	m_MarkXPre		= -1;
	m_MarkXCur		= -1;
	m_MarkYPre		= -1;
	m_MarkYCur		= -1;
	m_iProcErrCount = 0;
}

void IPLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EditFitsInfo, m_EditFitsInfo);
	DDX_Control(pDX, IDC_EditImgInfo, m_EditImgInfo);
	DDX_Control(pDX, IDC_BtnProcStart, m_BtnSTStart);
	DDX_Control(pDX, IDC_BtnProcStop, m_BtnSTStop);
	DDX_Control(pDX, IDC_ProgressMain, m_ProgressMain);
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
	ON_BN_CLICKED(IDC_BtnTest, &IPLDlg::OnBnClickedBtnTEST)
	ON_BN_CLICKED(IDC_BtnAutoProc, &IPLDlg::OnBnClickedBtnAutoProc)
	ON_COMMAND(ID_MenuAnalyse_LineGrayEnhance, &IPLDlg::OnMenu_Analyse_LineGrayEnhance)
	ON_COMMAND(ID_MenuAnalyse_Threshold, &IPLDlg::OnMenu_Analyse_Threshold)
	ON_COMMAND(ID_MenuProcess_ManualProc, &IPLDlg::OnMenu_Process_ManualProc)
	ON_COMMAND(ID_MenuFile_Reset, &IPLDlg::OnMenu_File_Reset)
	ON_COMMAND(ID_MenuProcess_Stop, &IPLDlg::OnMenu_Process_Stop)
	ON_BN_CLICKED(IDC_BtnProcStart, &IPLDlg::OnBnClickedBtnProcStart)
	ON_BN_CLICKED(IDC_BtnProcStop, &IPLDlg::OnBnClickedBtnProcStop)
END_MESSAGE_MAP()


// IPLDlg 消息处理程序

BOOL IPLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
	m_pCDCImgMain = GetDlgItem(IDC_StaticImgMain)->GetDC();

	m_pCDCImgSub = GetDlgItem(IDC_StaticImgSub)->GetDC();
	RECT rtSubImg;
	GetDlgItem(IDC_StaticImgSub)->GetClientRect(&rtSubImg);
	m_iSubImgW = rtSubImg.right - rtSubImg.left;
	m_iSubImgH = rtSubImg.bottom - rtSubImg.top;

	m_FontStandard.CreatePointFont(90, _T("宋体"));
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

	m_BtnSTStart.SetIcon(IDI_ICON_START, 48, 48);
	m_BtnSTStart.DrawBorder(FALSE);
	m_BtnSTStop.SetIcon(IDI_ICON_STOP, 48, 48);
	m_BtnSTStop.DrawBorder(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void IPLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	if (m_iThreadStatus == 1 || m_iThreadStatus == 2)
	{
		MessageBoxEx(GetSafeHwnd(), _T("Please stop Auto-Process."), _T("QwQ"), MB_OK | MB_ICONINFORMATION, 0);
		return;
	}
	// Release Fits temp data
	if (m_uipFitsDataTmp) delete[] m_uipFitsDataTmp;
	m_uipFitsDataTmp = NULL;
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
	// 清理数据
	if (m_uipFitsDataTmp) delete[] m_uipFitsDataTmp;
	m_uipFitsDataTmp = NULL;
	// 读取 FITS 文件
	m_FSCFitsX.OpenFitsFile(lpszPath);
	// 创建 FITS 数据临时存储空间
	long lDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight() * sizeof(int);
	m_uipFitsDataTmp = new int[m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight()];
	memcpy_s(m_uipFitsDataTmp, lDataSize, m_FSCFitsX.GetFitsDataPtr(), lDataSize);
	// 读取 FITS 文件 PixelCount 最大值和最小值
	m_iMinPixelCount = m_FSCFitsX.GetMinPixelCount();
	m_iMaxPixelCount = m_FSCFitsX.GetMaxPixelCount();
	// 创建 OpenCV 16位存储空间
	m_cvMat.release();
	m_cvMat.create(m_FSCFitsX.GetHeight(), m_FSCFitsX.GetWidth(), CV_16U);
	for (int i = 0; i < m_FSCFitsX.GetHeight() * m_FSCFitsX.GetWidth(); i++)
	{
		unsigned short usTmp = unsigned short(*(m_uipFitsDataTmp + i));
		m_cvMat.at<unsigned short>(i) = usTmp;
	}
	m_cvMat8U.release();
	m_cvMat.convertTo(m_cvMat8U, CV_8U, 1 / 255.0, 0.0);
	// 根据默认参数计算线性灰度阈值
	ComputeGrayLimit(0.432506, 0.977250);
	// 创建 FSC_DibX 位图对象
	CreateDibX();

	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	ListFitsHDU();
	//ListImgInfo();
	// 计数器+1
	m_iImgCount++;
	return true;
}


bool IPLDlg::CreateDibX()
{
	// 计算 BMP 文件数据区的长度(Byte)
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	long lBmpDataSize = iWidth * iHeight;
	// 创建 BMP 数据空间
	BYTE* pBmpData = (BYTE*) new BYTE[lBmpDataSize];
	memset(pBmpData, 0, lBmpDataSize);
	double dRate = 1.0f / (float)(m_iHighPixelCount - m_iLowPixelCount) * 255.;
	//double dRate = 1.0f / (float)(3444 - 2760) * 255.;
	// 复制 FITS 数据到 BMP 数据空间
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			//int iFValue = m_FSCFitsX.GetFitsData(j, i);
			int iFValue = *(m_uipFitsDataTmp + i * iWidth + j);
			// 使用自动灰度阈值进行线性灰度增强
			iFValue = ((float)iFValue - (float)m_iLowPixelCount) * dRate;
			//iFValue = 255 * double(iFValue) / 65535;
			if (iFValue < 0) iFValue = 0;
			if (iFValue > 255) iFValue = 255;
			BYTE tempValue;
			memset(&tempValue, 0, 1);
			tempValue = tempValue + BYTE(iFValue);
			// 复制到 BMP 文件数据区
			memcpy_s(pBmpData + (iHeight - i - 1) * iWidth + j, 1, &tempValue, 1);
		}
	}
	// 从外部数据生成 BMP 位图
	m_FSCDibX.LoadFromBuffer(pBmpData, iWidth, iHeight, 8);
	delete[] pBmpData;
	return true;
}


bool IPLDlg::CreateDibX_BinaryConv(int iBinThreshoud)
{
	// 计算 BMP 文件数据区的长度(Byte)
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	long lBmpDataSize = iWidth * iHeight;
	// 创建 BMP 数据空间
	BYTE* pBmpData = (BYTE*) new BYTE[lBmpDataSize];
	memset(pBmpData, 0, lBmpDataSize);
	// 复制 FITS 数据到 BMP 数据空间
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			int iFValue = *(m_uipFitsDataTmp + i * iWidth + j);
			if (iFValue >= iBinThreshoud) iFValue = 255;
			else iFValue = 0;
			BYTE tempValue;
			memset(&tempValue, 0, 1);
			tempValue = tempValue + BYTE(iFValue);
			// 复制到 BMP 文件数据区
			memcpy_s(pBmpData + (iHeight - i - 1) * iWidth + j, 1, &tempValue, 1);
		}
	}
	// 从外部数据生成 BMP 位图
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
	return;

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


void IPLDlg::UpdateUI()
{
	m_ProgressMain.SetRange32(0, m_iFitsCount - 1);
	m_ProgressMain.SetPos(m_iImgIndex);
	CString csLog;
	csLog.Format(_T("%d / %d   %s"), m_iImgIndex + 1, m_iFitsCount, m_vFitsName.at(m_iImgIndex));
	SetDlgItemText(IDC_Text_Proc, csLog);
	csLog.ReleaseBuffer();
}


void IPLDlg::OutputContoursFile(TCHAR * pszFileName, std::vector<std::vector<cv::Point>>& cont)
{
	CFile output;
	CFileException e;
	if (!output.Open(pszFileName, CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE(_T("Output file could not be opened %d\n"), e.m_cause);
	}
	CString csOutput = _T("");
	for (int i = 0; i < cont.size(); i++)
	{
		CString csTmp;
		for (int j = 0; j < cont[i].size(); j++)
		{
			csTmp.Format(_T("%s[%4d,%4d] "), csTmp, cont[i][j].x, cont[i][j].y);
		}
		csOutput.Format(_T("%s%3d %s\n"), csOutput, i, csTmp);
	}
	output.Write(csOutput, csOutput.GetLength());
	output.Flush();
	output.Close();
}


void IPLDlg::OutputAutoProcFile(CString & csFilePath, std::vector<cv::Point2f>* vpPixel, std::vector<cv::Point2f>* vpRD, std::vector<ObjIKMap>* pIKMap)
{
	CFile output;
	CFileException e;
	if (!output.Open(csFilePath, CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE(_T("Output file could not be opened %d\n"), e.m_cause);
	}
	CString csOutput = _T("");
	csOutput.Format(_T("LST: %8.04f\nCenterRA:  %8.04f  CenterDEC: %8.04f\nCenterAz:  %8.04f  CenterEl:  %8.04f\n\n"),
		m_CurLST, m_CurRA, m_CurDEC, m_CurAz * ToAngle, m_CurEl * ToAngle);
	csOutput.Format(_T("%s NO    [X, Y]               [RA, DEC]         K\n\n"), csOutput);
	for (int i = 0; i < pIKMap->size(); i++)
	{
		int index = pIKMap->at(i).Index;
		CString csTmp;
		csTmp.Format(_T(" %-3d   [%7.02f, %7.02f]   [%5.02f, %5.02f]   %7.04f\n"),
			i, vpPixel->at(index).x, vpPixel->at(index).y, vpRD->at(index).x, vpRD->at(index).y, pIKMap->at(i).KValue);
		csOutput += csTmp;
		/*csOutput.Format(_T("%s %-3d   [%7.02f, %7.02f]   [%5.02f, %5.02f]   %7.04f\n"),
			csOutput, i, vpPixel->at(index).x, vpPixel->at(index).y, vpRD->at(index).x, vpRD->at(index).y, pIKMap->at(i).KValue);*/
	}
	output.Write(csOutput, csOutput.GetLength());
	output.Flush();
	output.Close();
	csOutput.ReleaseBuffer();

	CString csLog;
	csLog.Format(_T("LST:%.03f  RA:%.03f  DEC:%.03f"), m_CurLST, m_CurRA, m_CurDEC);
	PrintLog(csLog);
	csLog.ReleaseBuffer();
}


void IPLDlg::ReleaseAndInit()
{
	m_csFitsDir.ReleaseBuffer();
	m_vFitsName.clear();
	vPixel[0].clear();
	vPixel[1].clear();
	vRaDec[0].clear();
	vRaDec[1].clear();
	m_iPreObjNID = 0;
	m_iCurObjNID = 0;
	m_iImgInQueue = 0;
	m_iImgIndex = 0;
	m_iFitsCount = 0;
	m_vpPixelPre = &(vPixel[1]);
	m_vpPixelCur = &(vPixel[0]);
	m_vpRDPre = &(vRaDec[1]);
	m_vpRDCur = &(vRaDec[0]);
	m_pcRA_Pre = &(cRA[1]);
	m_pcRA_Cur = &(cRA[0]);
	m_pcDEC_Pre = &(cDEC[1]);
	m_pcDEC_Cur = &(cDEC[0]);
	m_MarkXPre = -1;
	m_MarkYPre = -1;
	m_MarkXCur = -1;
	m_MarkYCur = -1;
	m_iProcErrCount = 0;
}


void IPLDlg::PrintLog(CString logStr)
{
	SYSTEMTIME TimeLocal;
	GetLocalTime(&TimeLocal);

	CString curTime;
	curTime.Format(_T("%02d:%02d:%02d"), TimeLocal.wHour, TimeLocal.wMinute, TimeLocal.wSecond);
	CString tempStr;
	tempStr.Format(_T("[%s] > %s\n"), curTime, logStr);

	int iLength = m_EditImgInfo.GetWindowTextLength();
	m_EditImgInfo.SetSel(iLength, iLength);
	m_EditImgInfo.ReplaceSel(tempStr);

	// Write logs to file
	if (m_EditImgInfo.GetLineCount() % 300 == 0 || logStr == _T("SYSTEM CALL: WRITE LOG!"))
	{
		CString csLog;
		m_EditImgInfo.GetWindowText(csLog);
		// Create data file directory
		CString csOutputPath = m_csFitsDir + _T("\\OBS_DATA");
		if (!PathIsDirectory(csOutputPath))
			::CreateDirectory(csOutputPath, NULL);
		// Open log file
		CString csFilePath;
		csFilePath.Format(_T("%s\\OBS_DATA\\OBS_Logs.txt"), m_csFitsDir);
		CFile output;
		CFileException e;
		if (!output.Open(csFilePath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, &e))
			TRACE(_T("Log file could not be opened %d\n"), e.m_cause);
		output.SeekToEnd();
		output.Write(csLog, csLog.GetLength());
		output.Flush();
		output.Close();
		// Empty log window
		m_EditImgInfo.SetSel(0, -1);
		m_EditImgInfo.ReplaceSel(_T(""));
	}
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
		pSampleData[i] = *(m_uipFitsDataTmp + int(i * dRate));
	}
	std::nth_element(pSampleData, pSampleData + iLowNum, pSampleData + iSampleNum);
	iLow = pSampleData[iLowNum];
	std::nth_element(pSampleData, pSampleData + iHighNum, pSampleData + iSampleNum);
	iHigh = pSampleData[iHighNum];
	delete[] pSampleData;
	m_iLowPixelCount = iLow;
	m_iHighPixelCount = iHigh;
}


void IPLDlg::Proc_LoadFile(LPCTSTR lpszPath)
{
	DWORD dTimeB = GetTickCount();

	// Release data
	if (m_uipFitsDataTmp) delete[] m_uipFitsDataTmp;
	m_uipFitsDataTmp = NULL;
	// Load FITS file
	m_FSCFitsX.OpenFitsFile(lpszPath);
	// Create FITS temp data
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	long lDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight() * sizeof(int);
	m_uipFitsDataTmp = new int[iWidth * iHeight];
	memcpy_s(m_uipFitsDataTmp, lDataSize, m_FSCFitsX.GetFitsDataPtr(), lDataSize);
	// Get FITS PixelCount Maximun and Minimum
	m_iMinPixelCount = m_FSCFitsX.GetMinPixelCount();
	m_iMaxPixelCount = m_FSCFitsX.GetMaxPixelCount();
	// Create cvMat16U
	m_cvMat.release();
	m_cvMat.create(iHeight, iWidth, CV_16U);
	for (int i = 0; i < iHeight * iWidth; i++)
	{
		m_cvMat.at<unsigned short>(i) = unsigned short(*(m_uipFitsDataTmp + i));
	}
	// Create cvMatShow
	m_cvMatShow.release();
	m_cvMat.convertTo(m_cvMatShow, CV_8U, 1 / 255.0, 0.0);
	// Test
	//cv::imwrite(_T("G:\\DuktoFiles\\Output\\1_Ori.bmp"), m_cvMatShow);
	// Create FSCDibX
	cv::flip(m_cvMatShow, m_cvMatShow, 0);
	m_FSCDibX.LoadFromBuffer(m_cvMatShow.data, iWidth, iHeight, 8);
	// Get scale value
	CWnd* cwImgMain = GetDlgItem(IDC_StaticImgMain);
	RECT rtImgMain;
	cwImgMain->GetClientRect(&rtImgMain);
	int Fw = rtImgMain.right - rtImgMain.left;
	int Fh = rtImgMain.bottom - rtImgMain.top;
	m_dScale = (double)Fw / iWidth < (double)Fh / iHeight ? (double)Fw / iWidth : (double)Fh / iHeight;
	// Show Image
	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(iWidth * m_dScale, iHeight * m_dScale));
	ListFitsHDU();
	ListImgInfo();

	// Log
	DWORD dTimeE = GetTickCount();
	CString csLog;
	csLog.Format(_T("Load FITS file success.   %dms"), dTimeE - dTimeB);
	PrintLog(csLog);
	csLog.ReleaseBuffer();
}


void IPLDlg::Proc_OutStand()
{
	DWORD dTimeB = GetTickCount();

	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();

	int iAve = m_FSCFitsX.GetAveragePixelCount();
	unsigned int iMin = UINT_MAX;
	unsigned int iMax = 0;
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			int *pValue = m_uipFitsDataTmp + i * iWidth + j;
			//unsigned int iValue = *pValue;
			/*int S1 = pow(iValue - iAve, 2);
			*pValue = S1;
			if (S1 < iMin) iMin = S1;
			if (S1 > iMax) iMax = S1;*/
			unsigned int S1 = pow(*pValue - iAve, 2);
			if (S1 > UINT_MAX)
				TRACE(_T("INT overflow.\n"));
			*pValue = S1;
			if (S1 < iMin) iMin = S1;
			if (S1 > iMax) iMax = S1;
		}
	}

	for (long i = 0; i < iHeight * iWidth; i++)
	{
		int *pValue = m_uipFitsDataTmp + i;
		int iTmp;
		iTmp = 65535 * double((*pValue - iMin)) / double((*pValue - iMax));
		*pValue = iTmp;
		//*pValue = 65535 * double(*pValue - iMax) / (*pValue - iMin);
		//if (*pValue < 0) *pValue = 0;
		//if (*pValue > 65535) *pValue = 65535;
	}
	// Output process info
	/*
	CString tempStr;
	tempStr.Format(_T("AvePixelCount = %d\n"), iAve);
	int iLength = m_EditImgInfo.GetWindowTextLength();
	m_EditImgInfo.SetSel(iLength, iLength);
	m_EditImgInfo.ReplaceSel(tempStr);
	return;
	*/

	DWORD dTimeE = GetTickCount();
	CString csLog;
	csLog.Format(_T("Proc Outstand success.   %dms"), dTimeE - dTimeB);
	PrintLog(csLog);
	csLog.ReleaseBuffer();
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
	// 根据自定义参数计算线性灰度阈值
	ComputeGrayLimit(dLowPercent, dHighPercent);
	// 创建 FSC_DibX 位图对象
	CreateDibX();

	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	ListFitsHDU();
	ListImgInfo();
}


void IPLDlg::OnBnClickedBtnBinaryConv()
{
	// TODO:
	int iBinThreshoud = GetDlgItemInt(IDC_EditBinThreshold);
	// 创建 FSC_DibX 位图对象
	CreateDibX_BinaryConv(iBinThreshoud);

	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	ListFitsHDU();
	ListImgInfo();
}


void IPLDlg::OnBnClickedBtnReset()
{
	// TODO:
	// 清理数据
	if (m_uipFitsDataTmp) delete[] m_uipFitsDataTmp;
	m_uipFitsDataTmp = NULL;
	// 创建 FITS 数据临时存储空间
	long lDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight() * sizeof(int);
	m_uipFitsDataTmp = new int[m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight()];
	memcpy_s(m_uipFitsDataTmp, lDataSize, m_FSCFitsX.GetFitsDataPtr(), lDataSize);
	// 读取 FITS 文件 PixelCount 最大值和最小值
	m_iMinPixelCount = m_FSCFitsX.GetMinPixelCount();
	m_iMaxPixelCount = m_FSCFitsX.GetMaxPixelCount();
	// 根据默认参数计算线性灰度阈值
	ComputeGrayLimit(0.432506, 0.977250);
	// 创建 FSC_DibX 位图对象
	CreateDibX();

	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	ListFitsHDU();
	ListImgInfo();
}


void IPLDlg::OnBnClickedBtnTEST()
{
	double A, a, H, Ra, Dec;
	double longitude = 126.331;
	double latitude = 43.823;
	double LST;
	GetLocalSiderealTime(longitude, &LST);
	Ra = 13.792;
	Dec = 49.313;
	H = LST - Ra;
	YXRange(&H, 24.0f);
	EquCoordToHorCoord(Dec, H, latitude, A, a);
	TRACE(_T("H = %.2f A = %.2f a = %.2f\n"), H, A * ToAngle, a * ToAngle);
	HorCoordToEquCoord(A, a, latitude, Dec, H);
	YXRange(&H, 24.0f);
	TRACE(_T("H = %.2f Dec = %.2f\n"), H, Dec);

	HWND hWnd = GetDlgItem(IDC_StaticImgMain)->GetSafeHwnd();
	Graphics GP(hWnd);
	Gdiplus::Rect rtMark(400 - 50, 400 - 50, 100, 100);
	Pen GreenPen(Color(255, 0, 0, 255), 5);
	GP.DrawRectangle(&GreenPen, rtMark);
}


void IPLDlg::OnMenu_File_Save()
{
	// TODO:
	CString csSavePath(_T(".\\SaveImg.bmp"));
	if (m_FSCDibX.IsValid())
		m_FSCDibX.SaveToFile(csSavePath);
}


void IPLDlg::OnMenu_File_Reset()
{
	this->OnBnClickedBtnReset();
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
	unsigned int iMin = UINT_MAX;
	unsigned int iMax = 0;
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			int *pValue = m_uipFitsDataTmp + i * iWidth + j;
			unsigned int S1 = pow(*pValue - iAve, 2);
			*pValue = S1;
			if (S1 < iMin) iMin = S1;
			if (S1 > iMax) iMax = S1;
		}
	}
	int iFSMin = INT_MAX;
	int iFSMax = INT_MIN;
	for (long i = 0; i < iHeight * iWidth; i++)
	{
		int *pValue = m_uipFitsDataTmp + i;
		int iTmp;
		iTmp = 65535 * double((*pValue - iMin)) / double((*pValue - iMax));
		*pValue = iTmp;
		//*pValue = 65535 * double(*pValue - iMax) / (*pValue - iMin);
		//if (*pValue < 0) *pValue = 0;
		//if (*pValue > 65535) *pValue = 65535;
	}

	// 计算 BMP 文件数据区的长度(Byte)
	long lBmpDataSize = iWidth * iHeight;
	// 创建 BMP 数据空间
	BYTE* pBmpData = (BYTE*) new BYTE[lBmpDataSize];
	memset(pBmpData, 0, lBmpDataSize);
	// 复制 FITS 数据到 BMP 数据空间
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			int FS1 = *(m_uipFitsDataTmp + i * iWidth + j);
			FS1 = 255 * double(FS1) / 65535;
			BYTE tempValue;
			memset(&tempValue, 0, 1);
			tempValue = tempValue + BYTE(FS1);
			// 复制到 BMP 文件数据区
			memcpy_s(pBmpData + (iHeight - i - 1) * iWidth + j, 1, &tempValue, 1);
		}
	}
	// 从外部数据生成 BMP 位图
	m_FSCDibX.LoadFromBuffer(pBmpData, iWidth, iHeight, 8);
	delete[] pBmpData;
	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(800, 800));
	return;
}


void IPLDlg::OnMenu_Analyse_LineGrayEnhance()
{
	OnBnClickedBtnLinearGE();
}


void IPLDlg::OnMenu_Analyse_Threshold()
{
	OnBnClickedBtnBinaryConv();
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
		// 过滤文件名
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
			// 清理数据
			if (m_uipFitsDataTmp) delete[] m_uipFitsDataTmp;
			m_uipFitsDataTmp = NULL;
			// 读取 FITS 文件
			m_FSCFitsX.OpenFitsFile(strFilePath);
			// 创建 FITS 数据临时存储空间
			long lDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight() * sizeof(int);
			m_uipFitsDataTmp = new int[m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight()];
			memcpy_s(m_uipFitsDataTmp, lDataSize, m_FSCFitsX.GetFitsDataPtr(), lDataSize);
			// 读取 FITS 文件 PixelCount 最大值和最小值
			m_iMinPixelCount = m_FSCFitsX.GetMinPixelCount();
			m_iMaxPixelCount = m_FSCFitsX.GetMaxPixelCount();
			cv::Mat cvMa(m_FSCFitsX.GetHeight(), m_FSCFitsX.GetWidth(), CV_16U);
			for (int i = 0; i < m_FSCFitsX.GetHeight() * m_FSCFitsX.GetWidth(); i++)
			{
				unsigned short usTmp = unsigned short(*(m_uipFitsDataTmp + i));
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
		unsigned short usTmp = unsigned short(*(m_uipFitsDataTmp + i));
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


void IPLDlg::OnMenu_Process_ManualProc()
{
	OnBnClickedBtnProc();
}


void IPLDlg::OnMenu_Process_Stop()
{
	if (m_iThreadStatus == 0) return;
	if (m_iThreadStatus == 1)
	{
		m_iThreadStatus = 0;
		// Avoid block when Waiting Msg
		while (TRUE)
		{
			DWORD result;
			MSG msg;
			result = MsgWaitForMultipleObjects(1, &m_hThread_Proc, FALSE, INFINITE, QS_ALLINPUT);
			if (result == (WAIT_OBJECT_0))
			{
				break;
			}
			else
			{
				PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
				DispatchMessage(&msg);
			}
		}
		//WaitForSingleObject(m_hThread_Proc, INFINITE);
	}
	else if (m_iThreadStatus == 2)
	{
		m_iThreadStatus = 0;
	}
	//Set BtnSTStart to START
	m_BtnSTStart.SetIcon(IDI_ICON_START, 48, 48);
	// Release and Init ProcThread data
	ReleaseAndInit();
	PrintLog(_T("Auto Process Stopped."));
	PrintLog(_T("SYSTEM CALL: WRITE LOG!"));
	MessageBoxEx(GetSafeHwnd(), _T("Auto Process Stopped."), _T("QwQ"), MB_OK | MB_ICONINFORMATION, 0);
	return;
}


void IPLDlg::OnBnClickedBtnProc()
{
	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	double dLongti = 126.331;
	double dLati = 43.817;
	double dRA, dDEC;
	SYSTEMTIME sysTime;
	m_FSCFitsX.GetOBSData(sysTime, dRA, dDEC);

	m_cvMat8U.release();
	m_cvMat8U.create(iHeight, iWidth, CV_8U);
	for (int i = 0; i < iHeight * iWidth; i++)
	{
		unsigned short usTmp = unsigned short(*(m_uipFitsDataTmp + i));
		m_cvMat8U.at<unsigned char>(i) = usTmp / 255.0;
	}
	cv::Mat tmpMat;
	cv::threshold(m_cvMat8U, tmpMat, 5, 255, CV_THRESH_BINARY);
	// 形态学滤波器膨胀图像
	//cv::Mat element10(10, 10, CV_8U, cv::Scalar(255));
	//cv::morphologyEx(tmpMat, tmpMat, cv::MORPH_CLOSE, element10);
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20));
	cv::dilate(tmpMat, tmpMat, element);

	if (m_iImgCount % 2 == 0)
	{
		cv::namedWindow("OpenCV Viewer", CV_WINDOW_NORMAL);
		cv::resizeWindow("OpenCV Viewer", iWidth / 4, iHeight / 4);
		cv::imshow("OpenCV Viewer", tmpMat);
		cv::imwrite("./ImgProc_Ori.bmp", tmpMat);
	}

	// 提取轮廓

	vector<vector<cv::Point>> cvContours(512);
	vector<Vec4i> hierarchy(512);
	//std::vector<std::vector<cv::Point>> cvContours(512);
	cv::findContours(tmpMat, cvContours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	// 过滤轮廓
	/*
	std::vector<std::vector<cv::Point>> contoursFinal;
	int iCMin = 10;
	int iCMax = 70;
	for (int i = 0; i < cvContours.size(); i++)
	{
		if (cvContours[i].size() > iCMin && cvContours[i].size() < iCMax)
			contoursFinal.push_back(cvContours[i]);
	}
	*/

	// 中心坐标
	/*
	std::vector<cv::Point> vCenterPoint;
	for (int i = 0; i < cvContours.size(); i++)
	{
		int ixm = INT_MAX, ixM = INT_MIN;
		int iym = INT_MAX, iyM = INT_MIN;
		for (int j = 0; j < cvContours[i].size(); j++)
		{
			if (cvContours[i][j].x < ixm) ixm = cvContours[i][j].x;
			if (cvContours[i][j].x > ixM) ixM = cvContours[i][j].x;
			if (cvContours[i][j].y < iym) iym = cvContours[i][j].y;
			if (cvContours[i][j].y > iyM) iyM = cvContours[i][j].y;
		}
		vCenterPoint.push_back(cv::Point((ixM - ixm) / 2, (iyM - iym) / 2));
	}
	*/

	// 输出轮廓数据
	TCHAR* pszFileName = _T("ContoursFile.txt");
	//OutputContoursFile(pszFileName, cvContours);

	cv::Mat showMat(m_cvMat8U.size(), CV_8U, cv::Scalar(255));
	cv::drawContours(showMat, cvContours, -1, 0, 2);
	// 标记所有目标，获得目标中心坐标
	for (int i = 0; i < cvContours.size(); i++)
	{
		//cv::Rect cvRec = cv::boundingRect(cvContours[i]);
		//cv::rectangle(showMat, cvRec, cv::Scalar(0), 2, 4);
		cv::RotatedRect rRect = cv::minAreaRect(cvContours[i]);
		cv::Point2f rPoint[4];
		rRect.points(rPoint);
		cv::line(showMat, rPoint[0], rPoint[1], 0, 2);
		cv::line(showMat, rPoint[1], rPoint[2], 0, 2);
		cv::line(showMat, rPoint[2], rPoint[3], 0, 2);
		cv::line(showMat, rPoint[3], rPoint[0], 0, 2);
		vPixel[m_iImgCount % 2].push_back(rRect.center);
	}
	// 计算恒星时
	double LST;
	GetLocalSiderealTime(dLongti, sysTime, &LST);
	// 计算视场中心的时角 （恒星的时角t、赤经α和当地的恒星时θ之间的关系为t=θ-α）
	double dHourAngle = LST - dRA;
	YXRange(&dHourAngle, 24.);
	// 计算视场中心的方位角、高度角
	double Az, El;
	EquCoordToHorCoord(dDEC, dHourAngle, dLati, Az, El);
	// 计算视场中心的赤经赤纬
	YXRange(&dRA, 24.);
	cRA[m_iImgCount % 2] = dRA;
	cDEC[m_iImgCount % 2] = dDEC;
	// 计算所有目标的赤经赤纬
	double A0 = Az;
	double E0 = El;
	double X0 = iWidth / 2.;
	double Y0 = iHeight / 2.;
	double SY = 6.3729159639520310744535933610831e-6;
	double SX = 6.3915866161901717904920316228831e-6;
	double AS, ES, YS, XS;
	for (int i = 0; i < vPixel[m_iImgCount % 2].size(); i++)
	{
		double RA, DEC, H;
		XS = vPixel[m_iImgCount % 2][i].x;
		YS = vPixel[m_iImgCount % 2][i].y;
		ES = E0 + (YS - Y0) * SY;
		AS = A0 + (XS - X0) * SX / cos(ES);
		HorCoordToEquCoord(AS, ES, dLati, DEC, H);
		RA = LST - H;
		YXRange(&RA, 24.);
		vRaDec[m_iImgCount % 2].push_back(cv::Point2f(RA, DEC));
	}
	// 输出所有目标的坐标和赤经赤纬
	pszFileName = _T("CentersFile.txt");
	CFile output;
	CFileException e;
	if (!output.Open(pszFileName, CFile::modeCreate | CFile::modeWrite, &e))
	{
		TRACE(_T("Output file could not be opened %d\n"), e.m_cause);
	}
	CString csOutput = _T("");
	csOutput.Format(_T("恒星时：%8.04f\n视场中心赤经：%8.04f  赤纬：%8.04f\n视场中心方位：%8.04f  高度：%8.04f\n\n"), 
					LST, dRA, dDEC, Az * ToAngle, El * ToAngle);
	for (int i = 0; i < vPixel[m_iImgCount % 2].size(); i++)
	{
		csOutput.Format(_T("%s %3d   [%7.02f, %7.02f]   [%5.02f, %5.02f]\n"), 
							csOutput, i, vPixel[m_iImgCount % 2][i].x, vPixel[m_iImgCount % 2][i].y, vRaDec[m_iImgCount % 2][i].x, vRaDec[m_iImgCount % 2][i].y);
	}
	output.Write(csOutput, csOutput.GetLength());
	output.Flush();
	output.Close();

	// 搜索目标
	int iTarIndex;
	if (m_iImgCount % 2 == 0)
	{
		double kx0, ky0;
		kx0 = cRA[0] - cRA[1];
		ky0 = cDEC[0] - cDEC[1];
		double kMin0 = 256;

		for (int iA = 0; iA < vRaDec[0].size(); iA++)
		{
			double kMin1 = 256;
			for (int iB = 0; iB < vRaDec[1].size(); iB++)
			{
				double xTmp = fabs(1 - (vRaDec[0][iA].x - vRaDec[1][iB].x) / kx0);
				double yTmp = fabs(1 - (vRaDec[0][iA].y - vRaDec[1][iB].y) / ky0);
				double xandy = xTmp + yTmp;
				if (xandy < kMin1) kMin1 = xandy;
			}
			if (kMin1 < kMin0)
			{
				kMin0 = kMin1;
				iTarIndex = iA;
			}
		}
	}
	// 标记目标
	if (m_iImgCount % 2 == 0)
	{
		cv::circle(showMat, cv::Point(vPixel[0][iTarIndex].x, vPixel[0][iTarIndex].y), 50, cv::Scalar(0), 1);
	}

	TRACE(_T("\nNumber of cvContours: %d\n"), cvContours.size());
	if (m_iImgCount % 2 == 0)
	{
		cv::namedWindow("OpenCV Viewer 2", CV_WINDOW_NORMAL);
		cv::resizeWindow("OpenCV Viewer 2", iWidth / 4, iHeight / 4);
		cv::imshow("OpenCV Viewer 2", showMat);
		cv::imwrite("./ImgProc_Show.bmp", showMat);
		cv::waitKey(0);
	}
	cvContours.clear();
	hierarchy.clear();
}


void IPLDlg::Proc_ExtractObject(int iIndex, std::vector<cv::Point2f>* vpPixel, std::vector<cv::Point2f>* vpRD, double* pcRA, double* pcDEC)
{
	DWORD dTimeB = GetTickCount();

	int iWidth = m_FSCFitsX.GetWidth();
	int iHeight = m_FSCFitsX.GetHeight();
	double dLongti = 126.331;
	double dLati = 43.817;
	double dRA, dDEC;
	SYSTEMTIME sysTime;
	// Get OBS Data
	m_FSCFitsX.GetOBSData(sysTime, dRA, dDEC);
	// Build cv::Mat in 8Bits
	m_cvMat8U.release();
	m_cvMat8U.create(iHeight, iWidth, CV_8U);
	for (int i = 0; i < iHeight * iWidth; i++)
	{
		//unsigned short usTmp = unsigned short(*(m_uipFitsDataTmp + i));
		//m_cvMat8U.at<unsigned char>(i) = usTmp / 255.0;
		unsigned short usTmp = unsigned short(*(m_uipFitsDataTmp + i));
		m_cvMat8U.at<unsigned char>(i) = usTmp / 255.0;
	}
	// Test
	//cv::imwrite(_T("G:\\DuktoFiles\\Output\\2_显著性增强.bmp"), m_cvMat8U);
	// OpenCV 二值化
	cv::Mat tmpMat;
	cv::threshold(m_cvMat8U, tmpMat, 5, 255, CV_THRESH_BINARY);
	// Test
	//cv::imwrite(_T("G:\\DuktoFiles\\Output\\3_二值化.bmp"), tmpMat);
	// OpenCV 形态学滤波器膨胀图像
	//cv::Mat element10(10, 10, CV_8U, cv::Scalar(255));
	//cv::morphologyEx(tmpMat, tmpMat, cv::MORPH_CLOSE, element10);
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20));
	cv::dilate(tmpMat, tmpMat, element);
	// Test
	//cv::imwrite(_T("G:\\DuktoFiles\\Output\\4_闭运算膨胀.bmp"), tmpMat);
	// OpenCV 提取轮廓
	vector<vector<cv::Point>> cvContours(512);
	vector<Vec4i> hierarchy(512);
	cv::findContours(tmpMat, cvContours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	
	// 输出轮廓点阵数据
	//const TCHAR* pszFileName = _T("ContoursFile.txt");
	//OutputContoursFile(pszFileName, cvContours);

	cv::Mat showMat(m_cvMat8U.size(), CV_8U, cv::Scalar(255));
	cv::drawContours(showMat, cvContours, -1, 0, 2);
	// 标记所有目标，获得目标中心坐标
	for (int i = 0; i < cvContours.size(); i++)
	{
		//cv::Rect cvRec = cv::boundingRect(cvContours[i]);
		//cv::rectangle(showMat, cvRec, cv::Scalar(0), 2, 4);
		cv::RotatedRect rRect = cv::minAreaRect(cvContours[i]);
		cv::Point2f rPoint[4];
		rRect.points(rPoint);
		cv::line(showMat, rPoint[0], rPoint[1], 0, 2);
		cv::line(showMat, rPoint[1], rPoint[2], 0, 2);
		cv::line(showMat, rPoint[2], rPoint[3], 0, 2);
		cv::line(showMat, rPoint[3], rPoint[0], 0, 2);
		vpPixel->push_back(rRect.center);
	}
	// Test
	//cv::imwrite(_T("G:\\DuktoFiles\\Output\\5_轮廓提取.bmp"), showMat);
	// 计算恒星时
	double LST;
	GetLocalSiderealTime(dLongti, sysTime, &LST);
	// 计算视场中心的时角 （恒星的时角t、赤经α和当地的恒星时θ之间的关系为t=θ-α）
	double dHourAngle = LST - dRA;
	YXRange(&dHourAngle, 24.);
	// 计算视场中心的方位角、高度角
	double Az, El;
	EquCoordToHorCoord(dDEC, dHourAngle, dLati, Az, El);
	// 计算视场中心的赤经赤纬
	YXRange(&dRA, 24.);
	*pcRA = dRA;
	*pcDEC = dDEC;
	// 计算所有目标的赤经赤纬
	double A0 = Az;
	double E0 = El;
	double X0 = iWidth / 2.;
	double Y0 = iHeight / 2.;
	double SY = 6.3729159639520310744535933610831e-6;
	double SX = 6.3915866161901717904920316228831e-6;
	double AS, ES, YS, XS;
	for (int i = 0; i < vpPixel->size(); i++)
	{
		double RA, DEC, H;
		XS = vpPixel->at(i).x;
		YS = vpPixel->at(i).y;
		ES = E0 + (YS - Y0) * SY;
		AS = A0 + (XS - X0) * SX / cos(ES);
		HorCoordToEquCoord(AS, ES, dLati, DEC, H);
		RA = LST - H;
		YXRange(&RA, 24.);
		vpRD->push_back(cv::Point2f(RA, DEC));
	}
	// Store Data
	m_CurLST = LST;
	m_CurAz = Az;
	m_CurEl = El;
	m_CurRA = dRA;
	m_CurDEC = dDEC;
	
	// Log
	DWORD dTimeE = GetTickCount();
	CString csLog;
	csLog.Format(_T("Proc ExtractObject success. N:%d   %dms"), cvContours.size(), dTimeE - dTimeB);
	PrintLog(csLog);
	csLog.ReleaseBuffer();

	cvContours.clear();
	hierarchy.clear();
}


void IPLDlg::Proc_SearchObject(int iIndex, double * pcRAPre, double * pcRACur, double * pcDECPre, double * pcDECCur,
	std::vector<cv::Point2f>* vpPixelPre, std::vector<cv::Point2f>* vpPixelCur, std::vector<cv::Point2f>* vpRDPre, std::vector<cv::Point2f>* vpRDCur)
{
	DWORD dTimeB = GetTickCount();

	m_vObjIK.clear();
	
	double kx0, ky0;
	kx0 = *pcRACur - *pcRAPre; //kx0: RA_k0
	ky0 = *pcDECCur - *pcDECPre; //ky0: DEC_k0

	for (int iCur = 0; iCur < vpRDCur->size(); iCur++)
	{
		double subMin = 1024.;
		for (int iPre = 0; iPre < vpRDPre->size(); iPre++)
		{
			double xTmp = fabs(1 - (vpRDCur->at(iCur).x - vpRDPre->at(iPre).x) / kx0);
			double yTmp = fabs(1 - (vpRDCur->at(iCur).y - vpRDPre->at(iPre).y) / ky0);
			double xandy = xTmp + yTmp;
			if (xandy < subMin) subMin = xandy;
		}
		ObjIKMap ik = { iCur, subMin };
		m_vObjIK.push_back(ik);
	}
	// 对IK列表排序
	std::sort(m_vObjIK.begin(), m_vObjIK.end(), &CompareIKMap);
	// 输出IK列表
	CString csFilePath;
	csFilePath.Format(_T("%s\\OBS_DATA\\%s.txt"), m_csFitsDir, m_vFitsName.at(iIndex).Left(35));
	OutputAutoProcFile(csFilePath, vpPixelCur, vpRDCur, &m_vObjIK);

	// 显示主窗口图像
	cv::flip(m_cvMat8U, m_cvMat8U, 0);
	m_FSCDibX.LoadFromBuffer(m_cvMat8U.data, m_cvMat8U.cols, m_cvMat8U.rows, 8);
	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(m_cvMat8U.cols * m_dScale, m_cvMat8U.rows * m_dScale));
	// 显示预览窗口图像
	cv::flip(m_cvMat8U, m_cvMat8U, 0);
	DrawSubImg();
	// 多目标标记
	for (int i = 0; i < 1; i++)
	{
		int iObjIndex = m_vObjIK.at(i).Index;
		cv::circle(m_cvMat8U, cv::Point((*vpPixelCur)[iObjIndex].x, (*vpPixelCur)[iObjIndex].y), 120, cv::Scalar(255), 3);
	}
	// Test
	//cv::imwrite(_T("G:\\DuktoFiles\\Output\\6_搜索目标.bmp"), m_cvMat8U);

	CString csOutputImgPath;
	csOutputImgPath.Format(_T("%s\\OBS_DATA\\%s.bmp"), m_csFitsDir, m_vFitsName.at(iIndex).Left(35));
	cv::imwrite(csOutputImgPath.GetString(), m_cvMat8U);

	// Log
	DWORD dTimeE = GetTickCount();
	CString csLog;
	csLog.Format(_T("Proc SearchObject success.   %dms"), dTimeE - dTimeB);
	PrintLog(csLog);
	csLog.ReleaseBuffer();
}


void IPLDlg::DrawMarkPre()
{
	if (m_iImgInQueue < 3)
		return;
	// Mark Frame Pre
	HWND hWnd = GetDlgItem(IDC_StaticImgMain)->GetSafeHwnd();
	Graphics GP(hWnd);
	GP.SetSmoothingMode(SmoothingModeHighQuality);
	Gdiplus::Rect rtMark(m_MarkXPre - 25, m_MarkYPre - 25, 50, 50);
	Pen GreenPen(Color(255, 0, 255, 0), 1);
	GreenPen.SetDashStyle(DashStyleDash);
	GP.DrawRectangle(&GreenPen, rtMark);
}


void IPLDlg::DrawMarkCur()
{
	// Get Mark coordinate in pixel (X, Y)
	int objIndex = m_vObjIK.at(0).Index;
	int Px = m_vpPixelCur->at(objIndex).x;
	int Py = m_vpPixelCur->at(objIndex).y;
	m_MarkXCur = (double)Px * m_dScale;
	m_MarkYCur = (double)Py * m_dScale;
	HWND hWnd = GetDlgItem(IDC_StaticImgMain)->GetSafeHwnd();
	Graphics GP(hWnd);
	GP.SetSmoothingMode(SmoothingModeHighQuality);
	Gdiplus::Rect rtMark(m_MarkXCur - 25, m_MarkYCur - 25, 50, 50);
	Pen GreenPen(Color(255, 0, 255, 0), 1);
	GP.DrawRectangle(&GreenPen, rtMark);
	// Check OBJECT Mark error
	int iPixelErr = 10;
	if ((m_iImgInQueue > 2) && (abs(m_MarkXCur - m_MarkXPre) > iPixelErr || abs(m_MarkYCur - m_MarkYPre) > iPixelErr))
	{
		m_iProcErrCount++;
		CString csErr;
		csErr.Format(_T("Proc Err: %s"), m_vFitsName.at(m_iImgIndex));
		PrintLog(csErr);
	}
	CString csErrText;
	csErrText.Format(_T("Proc Err: %d / %d"), m_iProcErrCount, m_iImgIndex + 1);
	SetDlgItemText(IDC_Text_ProcErr, csErrText);
	csErrText.ReleaseBuffer();
	// Store Mark coordinate in pixel (X, Y)
	m_MarkXPre = m_MarkXCur;
	m_MarkYPre = m_MarkYCur;
}


void IPLDlg::DrawSubImg()
{
	int objIndex = m_vObjIK.at(0).Index;
	int Px = m_vpPixelCur->at(objIndex).x;
	int Py = m_vpPixelCur->at(objIndex).y;
	int iImgW = m_iSubImgW;
	int iImgH = m_iSubImgH;
	Px = Px - iImgW / 2;
	Py = Py - iImgH / 2;

	// Build BMP Data cache
	INT iBitCount = 8;
	UINT uBmpLineByte = (iImgW * iBitCount + 31) / 32 * 4;
	DWORD dwBmpDataSize = uBmpLineByte * iImgH;
	LPBYTE lpSubImgData = new BYTE[dwBmpDataSize];
	for (int iRow = 0; iRow < iImgH; iRow++)
	{
		for (int iCol = 0; iCol < iImgW; iCol++)
		{
			unsigned char sTmp = 0;
			if ((Py + iRow >= 0 && Py + iRow <= m_cvMat8U.rows) && (Px + iCol >= 0 && Px + iCol <= m_cvMat8U.cols))
				sTmp = m_cvMat8U.at<unsigned char>(Py + iRow, Px + iCol);
				//sTmp = m_FSCFitsX.GetFitsData(Px + iCol, Py + iRow);
			else
				sTmp = 0;
			lpSubImgData[(iImgH - iRow - 1) * uBmpLineByte + iCol] = sTmp;
		}
	}
	m_FSCDibX_Sub.LoadFromBuffer(lpSubImgData, iImgW, iImgH, 8);
	m_FSCDibX_Sub.Draw(m_pCDCImgSub, CPoint(0, 0), CSize(iImgW, iImgH));
	delete[] lpSubImgData;
}


void IPLDlg::OnBnClickedBtnAutoProc()
{
	// If Process is already running, refuse to execute.
	if (m_iThreadStatus == 1) return;

	//typedef struct _browseinfo {
	//	HWND hwndOwner;            // 父窗口句柄  
	//	LPCITEMIDLIST pidlRoot;    // 要显示的文件目录对话框的根(Root)  
	//	LPTSTR pszDisplayName;     // 保存被选取的文件夹路径的缓冲区  
	//	LPCTSTR lpszTitle;         // 显示位于对话框左上部的标题  
	//	UINT ulFlags;              // 指定对话框的外观和功能的标志  
	//	BFFCALLBACK lpfn;          // 处理事件的回调函数  
	//	LPARAM lParam;             // 应用程序传给回调函数的参数  
	//	int iImage;                // 文件夹对话框的图片索引  
	//} BROWSEINFO, *PBROWSEINFO, *LPBROWSEINFO

	// Init Data
	m_iFitsCount = 0;
	m_vFitsName.clear();

	// Init BROWSEINFO
	TCHAR acDir[MAX_PATH] = { 0 };
	BROWSEINFO browInfo;
	LPITEMIDLIST lpidlBrowse;

	memset(&browInfo, 0, sizeof(BROWSEINFO));
	browInfo.hwndOwner = this->m_hWnd;
	browInfo.pidlRoot = NULL;
	browInfo.lpszTitle = _T("Please choose folder:");
	browInfo.pszDisplayName = acDir;
	browInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	browInfo.lpfn = NULL;
	browInfo.lParam = 0;
	browInfo.iImage = 0;

	lpidlBrowse = ::SHBrowseForFolder(&browInfo);
	if (lpidlBrowse == NULL) return;
	if (!::SHGetPathFromIDList(lpidlBrowse, acDir)) return;
	// Get fits folder path
	m_csFitsDir = acDir;
	// Get fits file count
	CFileFind fileFinder;
	bool bFound;
	int iFileCount = 0;

	bFound = fileFinder.FindFile(m_csFitsDir + _T("\\*.fit"));
	while (bFound)
	{
		bFound = fileFinder.FindNextFile();
		if (fileFinder.IsDots()) continue; //filter . and ..
		if (fileFinder.IsDirectory()) continue; //filter directory
		if (fileFinder.IsSystem()) continue; //filter system file
		if (fileFinder.IsHidden()) continue; //filter hidden file
		m_vFitsName.push_back(fileFinder.GetFileName());
		iFileCount++;
	}
	m_iFitsCount = iFileCount;
	fileFinder.Close();
	if (m_iFitsCount == 0) return; //no file

	// Sort fits file by OBS-TIME
	std::sort(m_vFitsName.begin(), m_vFitsName.end(), CompareFitsTime);
	
	TRACE(_T("m_csFitsDir = %s\n"), m_csFitsDir);
	TRACE(_T("m_iFitsCount = %d\n"), m_iFitsCount);

	// Start processing thread
	m_iThreadStatus = 1; //Set thread status Running.

	UINT uiThread_Proc;
	m_hThread_Proc = (HANDLE)_beginthreadex(NULL, 0, &uiFunc_Proc, this, 0, &uiThread_Proc);
	ASSERT(m_hThread_Proc);
	SetThreadPriority(m_hThread_Proc, THREAD_PRIORITY_HIGHEST);
}


void IPLDlg::OnBnClickedBtnProcStart()
{
	// If Process is running, Pause.
	if (m_iThreadStatus == 1)
	{
		m_iThreadStatus = 2;
		// Avoid block when Waiting Msg
		while (TRUE)
		{
			DWORD result;
			MSG msg;
			result = MsgWaitForMultipleObjects(1, &m_hThread_Proc, FALSE, INFINITE, QS_ALLINPUT);
			if (result == (WAIT_OBJECT_0))
			{
				break;
			}
			else
			{
				PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
				DispatchMessage(&msg);
			}
		}
		//WaitForSingleObject(m_hThread_Proc, INFINITE);
		//Set BtnSTStart to START
		m_BtnSTStart.SetIcon(IDI_ICON_START, 48, 48);
		PrintLog(_T("Auto Process Paused."));
		MessageBoxEx(GetSafeHwnd(), _T("Auto Process Paused."), _T("QwQ"), MB_OK | MB_ICONINFORMATION, 0);
		return;
	}

	//typedef struct _browseinfo {
	//	HWND hwndOwner;            // 父窗口句柄  
	//	LPCITEMIDLIST pidlRoot;    // 要显示的文件目录对话框的根(Root)  
	//	LPTSTR pszDisplayName;     // 保存被选取的文件夹路径的缓冲区  
	//	LPCTSTR lpszTitle;         // 显示位于对话框左上部的标题  
	//	UINT ulFlags;              // 指定对话框的外观和功能的标志  
	//	BFFCALLBACK lpfn;          // 处理事件的回调函数  
	//	LPARAM lParam;             // 应用程序传给回调函数的参数  
	//	int iImage;                // 文件夹对话框的图片索引  
	//} BROWSEINFO, *PBROWSEINFO, *LPBROWSEINFO

	// Init Data
	int iFitsCountPre = m_iFitsCount;
	//m_iFitsCount = 0;
	m_vFitsName.clear();

	// If Process be Stop, init m_csFitsDir, init MarkPos, reset ProcErr count.
	if (m_iThreadStatus == 0)
	{
		// Init BROWSEINFO
		TCHAR acDir[MAX_PATH] = { 0 };
		BROWSEINFO browInfo;
		LPITEMIDLIST lpidlBrowse;

		memset(&browInfo, 0, sizeof(BROWSEINFO));
		browInfo.hwndOwner = this->m_hWnd;
		browInfo.pidlRoot = NULL;
		browInfo.lpszTitle = _T("Please choose folder:");
		browInfo.pszDisplayName = acDir;
		browInfo.ulFlags = BIF_RETURNONLYFSDIRS;
		browInfo.lpfn = NULL;
		browInfo.lParam = 0;
		browInfo.iImage = 0;

		lpidlBrowse = ::SHBrowseForFolder(&browInfo);
		if (lpidlBrowse == NULL) return;
		if (!::SHGetPathFromIDList(lpidlBrowse, acDir)) return;
		// Get fits folder path
		m_csFitsDir = acDir;
		// Reset ProcErr count
		m_iProcErrCount = 0;
		// Init MarkPos
		m_MarkXPre = -1;
		m_MarkXCur = -1;
		m_MarkYPre = -1;
		m_MarkYCur = -1;
	}
	// Create Output file directory
	CString csOutputPath = m_csFitsDir + _T("\\OBS_DATA");
	if (!PathIsDirectory(csOutputPath))
		::CreateDirectory(csOutputPath, NULL);

	// If Process be Pause, reload fits directory.

	// Get fits file count
	CFileFind fileFinder;
	bool bFound;
	int iFileCount = 0;

	bFound = fileFinder.FindFile(m_csFitsDir + _T("\\*.fit"));
	while (bFound)
	{
		bFound = fileFinder.FindNextFile();
		if (fileFinder.IsDots()) continue; //filter . and ..
		if (fileFinder.IsDirectory()) continue; //filter directory
		if (fileFinder.IsSystem()) continue; //filter system file
		if (fileFinder.IsHidden()) continue; //filter hidden file
		m_vFitsName.push_back(fileFinder.GetFileName());
		iFileCount++;
	}
	fileFinder.Close();

	// Check fits directory
	if (iFileCount < iFitsCountPre) //Detected files become less. Error.
	{
		MessageBoxEx(GetSafeHwnd(), _T("Detected Fits files lost. Terminated."), _T("QAQ"), MB_OK | MB_ICONERROR, 0);
		return;
	}
	m_iFitsCount = iFileCount;
	if (m_iFitsCount == 0) return; //Find no file

	// Sort fits file by OBS-TIME
	std::sort(m_vFitsName.begin(), m_vFitsName.end(), CompareFitsTime);

	TRACE(_T("m_csFitsDir = %s\n"), m_csFitsDir);
	TRACE(_T("m_iFitsCount = %d\n"), m_iFitsCount);
	CString csTmp;
	csTmp.Format(_T("FITS Directory: %s"), m_csFitsDir);
	PrintLog(csTmp);

	// Start processing thread
	UINT uiThread_Proc;
	m_hThread_Proc = (HANDLE)_beginthreadex(NULL, 0, &uiFunc_Proc, this, 0, &uiThread_Proc);
	if (m_hThread_Proc != NULL)
	{
		m_iThreadStatus = 1; //Set thread status Running
		m_BtnSTStart.SetIcon(IDI_ICON_PAUSE, 48, 48); //Set BtnSTStart to PAUSE
		PrintLog(_T("Auto Process Start."));
	}
	ASSERT(m_hThread_Proc);
	SetThreadPriority(m_hThread_Proc, THREAD_PRIORITY_HIGHEST);
}


void IPLDlg::OnBnClickedBtnProcStop()
{
	OnMenu_Process_Stop();
}