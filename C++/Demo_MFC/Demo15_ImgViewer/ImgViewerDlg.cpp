
// ImgViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo15_ImgViewer.h"
#include "ImgViewerDlg.h"
#include "afxdialogex.h"
#include "FSC_FitsX\FSC_FitsX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ImgViewerDlg 对话框



ImgViewerDlg::ImgViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO15_IMGVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ImgViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ImgViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// ImgViewerDlg 消息处理程序

BOOL ImgViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	// 设置控件位置
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	GetClientRect(&m_mainRect);
	m_mainRect.left = (iScreenWidth - (1024 + 20)) / 2;
	m_mainRect.top = (iScreenHeight - (768 + 20 + 30)) / 2;
	m_mainRect.right = m_mainRect.left + 1024 + 20;
	m_mainRect.bottom = m_mainRect.top + 768 + 20 + 30;
	CalcWindowRect(&m_mainRect); //根据客户区大小，计算窗体大小
	SetWindowPos(NULL, m_mainRect.left, m_mainRect.top, m_mainRect.Width(), m_mainRect.Height(), SWP_NOZORDER); //设置窗体位置

	GetClientRect(&m_mainRect);
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&m_viewRect);
	m_viewRect.left = m_mainRect.left + 10;
	m_viewRect.top = m_mainRect.top + 10;
	m_viewRect.right = m_mainRect.right - 10;
	m_viewRect.bottom = m_mainRect.bottom - 30 - 10;
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(m_viewRect);

	CRect rectTips;
	GetDlgItem(IDC_STATIC_TIPS)->GetClientRect(&rectTips);
	rectTips = CRect(m_mainRect.left + 10, m_mainRect.bottom - 30, m_mainRect.right - 10, m_mainRect.bottom);
	GetDlgItem(IDC_STATIC_TIPS)->MoveWindow(rectTips);

	// 设置成员变量
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();
	m_SubWin = NULL;
	m_lpCDibDataBuffer = NULL;

	GetClientRect(&m_mainRect);
	ClientToScreen(&m_mainRect);
	m_SubWinParam.iSubWidth = 256;
	m_SubWinParam.iSubHeight = 256;
	m_SubWinParam.iMainWinPosX = m_mainRect.left;
	m_SubWinParam.iMainWinPosY = m_mainRect.top;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void ImgViewerDlg::OnPaint()
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
HCURSOR ImgViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ImgViewerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();

	// FSC_FitsX Test
	FSC_FitsX testFX;
	testFX.OpenFitsFile(_T("F:\\FrozenSky_Data\\Astronomy\\TeleImages\\36585-000000-0000-20181023125044050.fit"));
}


void ImgViewerDlg::OnCancel()
{
	// TODO:
	
	// 释放预览图像数据缓存
	if (m_lpCDibDataBuffer)
		delete[] m_lpCDibDataBuffer;

	// 释放预览窗口
	if (m_SubWin != NULL)
	{
		m_SubWin->OnCancel();
		delete m_SubWin;
		m_SubWin = NULL;
	}
	
	CDialogEx::OnCancel();
}


void ImgViewerDlg::OnDropFiles(HDROP hDropInfo)
{
	// 获取拖入文件数量
	UINT iFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount > 1) return;

	// 获取拖入文件路径
	TCHAR tcFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcFilePath, _countof(tcFilePath));
	TRACE(_T("FilePath: %s\n"), tcFilePath);

	// 判断文件类型
	// ......
	
	// FSC_FitsX Test
	FSC_FitsX testFX;
	testFX.OpenFitsFile(tcFilePath);
	// 从 FITS 文件载入
	//m_cDibImage.LoadFromFitsFile(tcFilePath);
	//m_cDibImage.Draw(m_pCDC, CPoint(0, 0), CSize(1024, 768));

	/*
	// 载入位图数据
	m_cDibImage.LoadFromFile(tcFilePath);
	//m_cDibImage.RgbToGrade();
	m_cDibImage.Draw(m_pCDC, CPoint(0, 0), CSize(1024, 768));
	m_SubWinParam.pViewImage = NULL;

	// Test: 创建子窗口
	m_SubWin = new SubWinDlg(&m_SubWinParam, this);
	m_SubWin->Create(IDD_SUBWIN_DIALOG, this);
	//m_SubWin->ShowWindow(SW_SHOW);
	*/
	CDialogEx::OnDropFiles(hDropInfo);
}

// 计算并构建预览图像对象
void ImgViewerDlg::BuildTempCDibImage(int x, int y, int width, int height)
{
	int iTempx = x - 10 - width / 2;
	int iTempy = y - 10 - height / 2;
	int iBitCount = 24;

	// 计算位图每行的字节数
	//UINT uBmpLineByte = (width * iBitCount + 31) / 8;
	//uBmpLineByte = uBmpLineByte / 4 * 4;
	UINT uBmpLineByte = (width * iBitCount + 31) / 32 * 4;
	// 计算位图数据区字节数
	DWORD dwBmpDataSize = uBmpLineByte * height;

	// 创建预览图像数据区缓存空间
	if (m_lpCDibDataBuffer == NULL)
		m_lpCDibDataBuffer = (LPBYTE) new BYTE[dwBmpDataSize];

	// 写入预览图像数据
	LPBYTE lpData = m_cDibImage.GetData();
	int iLineByteNum = m_cDibImage.GetLineByte();
	for (int ci = 0; ci < height; ci++)
	{
		memcpy_s(m_lpCDibDataBuffer + ci * uBmpLineByte, uBmpLineByte, lpData + (768 - (iTempy + height - ci)) * iLineByteNum + iTempx * iBitCount / 8, uBmpLineByte);
	}
	
	// 创建预览图像Dib对象
	//m_cDibTemp.LoadFromBuffer(m_lpCDibDataBuffer, width, height, 8);
	m_cDibTemp.LoadFromBuffer(m_lpCDibDataBuffer, width, height, 24);
}

void ImgViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&m_viewRect);
	if ((m_SubWin) &&
		(point.x >= m_viewRect.left + 10 + m_SubWinParam.iSubWidth / 2) && (point.x <= m_viewRect.left + 10 + 1024 - m_SubWinParam.iSubWidth / 2) &&
		(point.y >= m_viewRect.top + 10 + m_SubWinParam.iSubHeight / 2) && (point.y <= m_viewRect.top +10 + 768 - m_SubWinParam.iSubHeight / 2))
	{
		// 计算并构建预览图像对象
		BuildTempCDibImage(point.x, point.y, m_SubWinParam.iSubWidth, m_SubWinParam.iSubHeight);
		m_SubWinParam.pViewImage = &m_cDibTemp;

		CString tempStr;
		tempStr.Format(_T("MouseX: %4d MouseY: %4d"), point.x, point.y);
		SetDlgItemText(IDC_STATIC_TIPS, tempStr);
		if (m_SubWin != NULL)
		{
			m_SubWin->ShowWindow(SW_SHOW);
			m_SubWin->ShowViewImage();
		}
	}
	else
	{
		;
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
