
// IPLDlg.h : 头文件
//

#pragma once

#include "Dib_Ex.h"
#include "FSC_FitsX.h"
#include "afxwin.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Interface/BtnST.h"
#include "afxcmn.h"

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
	afx_msg void OnMenu_File_Reset();
	afx_msg void OnMenu_File_Quit();
	afx_msg void OnMenu_Analyse_OutStand();
	afx_msg void OnMenu_Analyse_LineGrayEnhance();
	afx_msg void OnMenu_Analyse_Threshold();
	afx_msg void OnMenu_OpenCV_OpenImg();
	afx_msg void OnMenu_OpenCV_NolinearGE();
	afx_msg void OnMenu_OpenCV_Threshold();
	afx_msg void OnMenu_OpenCV_EqualizeHist();
	afx_msg void OnMenu_Process_ManualProc();
	afx_msg void OnMenu_Process_Stop();
	afx_msg void OnBnClickedBtnLinearGE();
	afx_msg void OnBnClickedBtnBinaryConv();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnProc();
	afx_msg void OnBnClickedBtnAutoProc();
	afx_msg void OnBnClickedBtnProcStart();
	afx_msg void OnBnClickedBtnProcStop();
	afx_msg void OnBnClickedBtnTEST();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:

	bool OpenFile_FITS(LPCTSTR lpszPath);
	bool CreateDibX();
	bool CreateDibX_BinaryConv(int iBinThreshoud);
	void ComputeGrayLimit(double dLowPer, double dHighPer);
	void Proc_LoadFile(LPCTSTR lpszPath);
	void Proc_OutStand();
	void Proc_ExtractObject(int iIndex, std::vector<cv::Point2f>* vpPixel, std::vector<cv::Point2f>* vpRD, double* pcRA, double* pcDEC);
	void Proc_SearchObject(int iIndex, double * pcRAPre, double * pcRACur, double * pcDECPre, double * pcDECCur,
		std::vector<cv::Point2f>* vpPixelPre, std::vector<cv::Point2f>* vpPixelCur, std::vector<cv::Point2f>* vpRDPre, std::vector<cv::Point2f>* vpRDCur);
	void DrawMarkPre();
	void DrawMarkCur();
	void DrawSubImg();
	void ListFitsHDU();
	void ListImgInfo();
	void UpdateUI();
	void OutputContoursFile(TCHAR* pszFileName, std::vector<std::vector<cv::Point>> & cont);
	void OutputAutoProcFile(CString & csFilePath, std::vector<cv::Point2f>* vpPixel, std::vector<cv::Point2f>* vpRD, std::vector<ObjIKMap>* pIKMap);
	void ReleaseAndInit();
	void PrintLog(CString logStr);

private:
	CDC* m_pCDCImgMain;
	CDC* m_pCDCImgSub;
	FSC_FitsX m_FSCFitsX;
	FSC_DibX m_FSCDibX;
	FSC_DibX m_FSCDibX_Sub;
	int* m_uipFitsDataTmp;
	int m_iMinPixelCount;
	int m_iMaxPixelCount;
	int m_iLowPixelCount;
	int m_iHighPixelCount;
	int m_iImgCount;

	// UI
	CFont m_FontStandard;
	CEdit m_EditFitsInfo;
	CEdit m_EditImgInfo;
	int m_iSubImgW;
	int m_iSubImgH;

	// OpenCV support
	cv::Mat m_cvMat;
	cv::Mat m_cvMatShow;
	cv::Mat m_cvMat8U;

public:
	// AutoProc Data
	CString m_csFitsDir; //Record path of OBJECT directory
	int m_iFitsCount; //Record fits file count of OBJECT directory
	std::vector<CString> m_vFitsName; //Record array of fits file names
	std::vector<cv::Point2f> vPixel[2];
	std::vector<cv::Point2f> vRaDec[2];
	std::vector<cv::Point2f> *m_vpPixelPre, *m_vpPixelCur; //Record Ptr To vPixel[2]
	std::vector<cv::Point2f> *m_vpRDPre, *m_vpRDCur; //Record Ptr To vRaDec[2]
	std::vector<ObjIKMap> m_vObjIK; //Record I&K Value of OBJECT
	double cRA[2], cDEC[2];
	double *m_pcRA_Pre, *m_pcRA_Cur; //Record Ptr To cRA[2]
	double *m_pcDEC_Pre, *m_pcDEC_Cur; //Record Ptr To cDEC[2]
	int m_iPreObjNID;
	int m_iCurObjNID;
	int m_iImgInQueue;
	int m_iImgIndex;
	int m_MarkXPre, m_MarkXCur;
	int m_MarkYPre, m_MarkYCur;
	int m_iProcErrCount;
	double m_dScale;
	double m_CurLST;
	double m_CurRA, m_CurDEC, m_CurAz, m_CurEl;
	HANDLE m_hThread_Proc;
	int m_iThreadStatus; //Thread status. 0 Stop, 1 Running, 2 Pause.

	CButtonST m_BtnSTStart;
	CButtonST m_BtnSTStop;
	CProgressCtrl m_ProgressMain;
};