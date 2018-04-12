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

SubWinDlg::SubWinDlg(UINT uFlag, CWnd* pParent)
	: CDialogEx(IDD_SUBWIN_DIALOG, pParent)
{
	this->m_uFlag = uFlag;
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

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
