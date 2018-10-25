// SubWinDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo15_ImgViewer.h"
#include "SubWinDlg.h"
#include "afxdialogex.h"


// SubWinDlg 对话框

IMPLEMENT_DYNAMIC(SubWinDlg, CDialogEx)

SubWinDlg::SubWinDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SUBWIN_DIALOG, pParent)
{

}

SubWinDlg::SubWinDlg(SubWinParam* pSubWinParam, CWnd* pParent)
	: CDialogEx(IDD_SUBWIN_DIALOG, pParent)
{
	m_pSubWinParam = pSubWinParam;
}

SubWinDlg::~SubWinDlg()
{

}

void SubWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SubWinDlg, CDialogEx)
END_MESSAGE_MAP()


// SubWinDlg 消息处理程序


BOOL SubWinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 设置预览子窗体的位置
	GetClientRect(&m_mainRect);
	m_mainRect.left = m_pSubWinParam->iMainWinPosX + 1024 + 20 + 5;
	m_mainRect.top = m_pSubWinParam->iMainWinPosY;
	m_mainRect.right = m_mainRect.left + m_pSubWinParam->iSubWidth;
	m_mainRect.bottom = m_mainRect.top + m_pSubWinParam->iSubHeight;
	CalcWindowRect(&m_mainRect);
	SetWindowPos(NULL, m_mainRect.left, m_mainRect.top, m_mainRect.Width(), m_mainRect.Height(), SWP_NOZORDER); //设置窗体位置
	// 
	GetClientRect(&m_mainRect);
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&m_viewRect);
	m_viewRect.left = m_mainRect.left;
	m_viewRect.top = m_mainRect.top;
	m_viewRect.right = m_mainRect.right;
	m_viewRect.bottom = m_mainRect.bottom;
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(m_viewRect);

	// 获取绘制DC
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();

	// 初始化预览位图对象

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SubWinDlg::ShowViewImage()
{
	(m_pSubWinParam->pViewImage)->Draw(m_pCDC, CPoint(0, 0), CSize(256, 256));
}


void SubWinDlg::OnCancel()
{
	TRACE(_T("OnCancel!\n"));
	DestroyWindow();

	//CDialogEx::OnCancel();
}