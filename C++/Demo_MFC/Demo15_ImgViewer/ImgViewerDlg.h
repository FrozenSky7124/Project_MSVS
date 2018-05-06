
// ImgViewerDlg.h : 头文件
//

#pragma once

#include "Dib_Ex.h"
#include "SubWinDlg.h"

// ImgViewerDlg 对话框
class ImgViewerDlg : public CDialogEx
{
// 构造
public:
	ImgViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO15_IMGVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
public:
	CRect m_mainRect;
	CRect m_viewRect;
	CDib m_cDibImage;
	CDib m_cDibTemp;
	CDC* m_pCDC;

	SubWinDlg* m_SubWin;
	SubWinParam m_SubWinParam;

	LPBYTE m_lpCDibDataBuffer;
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void BuildTempCDibImage(int x, int y, int width, int height);
};
