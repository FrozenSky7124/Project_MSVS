
// IPLDlg.h : 头文件
//

#pragma once

#include "Dib_Ex.h"
#include "FSC_FitsX.h"
#include "afxwin.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

struct ObjIKMap
{
	int Index;
	double KValue;
};

// IPLDlg 对话框
class IPLDlg : public CDialog
{
// 构造
public:
	IPLDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_IMAGEPROCLITE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnMenu_File_Open();
	afx_msg void OnMenu_File_Save();
	afx_msg void OnMenu_File_Quit();
	afx_msg void OnMenu_Analyse_OutStand();
	afx_msg void OnMenu_OpenCV_OpenImg();
	afx_msg void OnMenu_OpenCV_NolinearGE();
	afx_msg void OnMenu_OpenCV_Threshold();
	afx_msg void OnMenu_OpenCV_EqualizeHist();
	afx_msg void OnBnClickedBtnLinearGE();
	afx_msg void OnBnClickedBtnBinaryConv();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnProc();
	afx_msg void OnBnClickedBtnAutoProc();
	afx_msg void OnBnClickedBtnTEST();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:

	bool OpenFile_FITS(LPCTSTR lpszPath);
	bool CreateDibX();
	bool CreateDibX_BinaryConv(int iBinThreshoud);
	void ComputeGrayLimit(double dLowPer, double dHighPer);
	void Proc_OutStand();
	void Proc_ExtractObject(int iIndex, std::vector<cv::Point2f>* pvCenter, std::vector<cv::Point2f>* pvRD, double* pcRA, double* pcDEC);
	void Proc_SearchObject(int iIndex, double * pcRAPre, double * pcRACur, double * pcDECPre, double * pcDECCur,
		std::vector<cv::Point2f>* pvCPre, std::vector<cv::Point2f>* pvCCur, std::vector<cv::Point2f>* pvRDPre, std::vector<cv::Point2f>* pvRDCur);
	void ListFitsHDU();
	void ListImgInfo();
	void OutputContoursFile(TCHAR* pszFileName, std::vector<std::vector<cv::Point>> & cont);
	void OutputAutoProcFile(CString & csFilePath, std::vector<cv::Point2f>* pvCenter, std::vector<cv::Point2f>* pvRD, std::vector<ObjIKMap>* pIKMap);

private:
	CDC* m_pCDCImgMain;
	FSC_FitsX m_FSCFitsX;
	FSC_DibX m_FSCDibX;
	int m_iProcConfig;
	int* m_ipFitsDataTmp;
	int m_iMinPixelCount;
	int m_iMaxPixelCount;
	int m_iLowPixelCount;
	int m_iHighPixelCount;
	int m_iImgCount;

	// UI
	CFont m_FontStandard;
	CEdit m_EditFitsInfo;
	CEdit m_EditImgInfo;

	// OpenCV support
	cv::Mat m_cvMat;
	cv::Mat m_cvMat8U;

public:
	// 以下变量不安全
	// AutoProc Data
	CString m_csFitsDir;
	int m_iFitsCount;
	std::vector<CString> m_vFitsName;
	std::vector<cv::Point2f> vCenter[2];
	std::vector<cv::Point2f> vRaDec[2];
	double cRA[2], cDEC[2];
	double m_CurLST;
	double m_CurRA, m_CurDEC, m_CurAz, m_CurEl;
};