// SubWinDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo15_ImgViewer.h"
#include "SubWinDlg.h"
#include "afxdialogex.h"


// SubWinDlg �Ի���

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


// SubWinDlg ��Ϣ�������


BOOL SubWinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
