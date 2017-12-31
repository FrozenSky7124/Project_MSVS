
// BasicCamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo07_BasicCam.h"
#include "BasicCamDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// BasicCamDlg 对话框



BasicCamDlg::BasicCamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hCamera = -1;

}

void BasicCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_cPreview);
}

BEGIN_MESSAGE_MAP(BasicCamDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &BasicCamDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// BasicCamDlg 消息处理程序

BOOL BasicCamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	do
	{
		if (CameraSdkInit(1) != 0)
			TRACE(_T("初始化SDK失败！\n"));
		else
			TRACE(_T("初始化SDK成功！\n"));
		
	} while (0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void BasicCamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR BasicCamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void BasicCamDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


void BasicCamDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	TRACE(_T("反初始化相机\n"));
	if (m_hCamera > 0)
	{
		if (NULL != m_hDispThread)
		{
			//等待采集线程结束
			m_bExit = TRUE;
			::WaitForSingleObject(m_hDispThread, INFINITE);
			CloseHandle(m_hDispThread);
			m_hDispThread = NULL;
		}

		//反初始化相机。
		CameraUnInit(m_hCamera);
		m_hCamera = 0;
	}

	if (m_pFrameBuffer)
	{
		CameraAlignFree(m_pFrameBuffer);
		m_pFrameBuffer = NULL;
	}
	CDialogEx::OnCancel();
}
