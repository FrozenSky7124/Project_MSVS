// AboutDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo09.h"
#include "AboutDlg.h"
#include "afxdialogex.h"


// AboutDlg �Ի���

IMPLEMENT_DYNAMIC(AboutDlg, CDialogEx)

AboutDlg::AboutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ABOUT, pParent)
{

}

AboutDlg::~AboutDlg()
{
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AboutDlg, CDialogEx)
END_MESSAGE_MAP()


// AboutDlg ��Ϣ�������
