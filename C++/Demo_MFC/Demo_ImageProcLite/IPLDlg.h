
// IPLDlg.h : 头文件
//

#pragma once

#include "Dib_Ex.h"
#include "FSC_FitsX.h"
#include "afxwin.h"

#define LinearGrayEnhancement 0x00000001 // 线性灰度增强

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
	afx_msg void OnBnClickedBtnLinearGE();
	afx_msg void OnBnClickedBtnBinaryConv();
	afx_msg void OnBnClickedBtnReset();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:

	bool OpenFile_FITS(LPCTSTR lpszPath);
	bool CreateDibX();
	bool CreateDibX_BinaryConv(int iBinThreshoud);
	void ComputeGrayLimit(double dLowPer, double dHighPer);
	void ListFitsHDU();
	void ListImgInfo();

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

	CFont m_FontStandard;
	CEdit m_EditFitsInfo;
	CEdit m_EditImgInfo;
};