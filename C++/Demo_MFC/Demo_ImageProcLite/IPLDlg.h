
// IPLDlg.h : 头文件
//

#pragma once

#include "Dib_Ex.h"
#include "FSC_FitsX.h"

// IPLDlg 对话框
class IPLDlg : public CDialogEx
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
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

public:

	bool OpenFile_FITS(LPCTSTR lpszPath);

private:
	CDC* m_pCDCImgMain;
	FSC_FitsX m_FSCFitsX;
	FSC_DibX m_FSCDibX;
};
