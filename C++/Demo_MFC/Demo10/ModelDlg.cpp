// ModelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo10.h"
#include "ModelDlg.h"
#include "afxdialogex.h"


// ModelDlg 对话框

IMPLEMENT_DYNAMIC(ModelDlg, CDialogEx)

ModelDlg::ModelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

ModelDlg::~ModelDlg()
{
}

void ModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ModelDlg, CDialogEx)
END_MESSAGE_MAP()


// ModelDlg 消息处理程序
