// MainDialogWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo01_MFCInject.h"
#include "MainDialogWnd.h"
#include "afxdialogex.h"


// CMainDialogWnd 对话框

IMPLEMENT_DYNAMIC(CMainDialogWnd, CDialogEx)

CMainDialogWnd::CMainDialogWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMainDialogWnd::~CMainDialogWnd()
{
}

void CMainDialogWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDialogWnd, CDialogEx)
END_MESSAGE_MAP()


// CMainDialogWnd 消息处理程序
