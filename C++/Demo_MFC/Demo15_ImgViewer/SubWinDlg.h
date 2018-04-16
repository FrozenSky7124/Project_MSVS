#pragma once

#include "Dib_Ex.h"

struct SubWinParam //预览子窗体相关参数数据结构
{
	int iMainWinPosX;
	int iMainWinPosY;
	int iSubWidth;
	int iSubHeight;
	int iMouseX;
	int iMouseY;
	CDib* pOriginImage;
};

// SubWinDlg 对话框

class SubWinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SubWinDlg)

public:
	SubWinDlg(CWnd* pParent = NULL);   // 标准构造函数
	SubWinDlg(SubWinParam* pSubWinParam, CWnd* pParent = NULL); // 重载构造函数
	void ShowViewImage();
	virtual ~SubWinDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUBWIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	SubWinParam* m_pSubWinParam;
	CDC* m_pCDC;
	CRect m_mainRect;
	CRect m_viewRect;
	CDib m_viewImage;
};
