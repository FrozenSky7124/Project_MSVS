
// IPLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Demo_ImageProcLite.h"
#include "IPLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// IPLDlg 对话框


IPLDlg::IPLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO_IMAGEPROCLITE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void IPLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(IPLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// IPLDlg 消息处理程序

BOOL IPLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
	m_pCDCImgMain = GetDlgItem(IDC_StaticImgMain)->GetDC();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void IPLDlg::OnPaint()
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
HCURSOR IPLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void IPLDlg::OnOK()
{
	// Disable OnOK() Function.
	/*
	CDialogEx::OnOK();
	*/
}


void IPLDlg::OnDropFiles(HDROP hDropInfo)
{
	// Get count of files drop in
	UINT iFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount > 1) return;

	// Get file path
	TCHAR tcFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcFilePath, _countof(tcFilePath));
	TRACE(_T("FilePath: %s\n"), tcFilePath);

	// Filter file type
	// ......

	// FITS
	OpenFile_FITS(tcFilePath);

	CDialogEx::OnDropFiles(hDropInfo);
}


bool IPLDlg::OpenFile_FITS(LPCTSTR lpszPath)
{
	m_FSCFitsX.OpenFitsFile(lpszPath);

	// 计算 BMP 文件数据区的长度(Byte)
	long lBmpDataSize = m_FSCFitsX.GetWidth() * m_FSCFitsX.GetHeight();
	// 创建 BMP 数据空间
	BYTE* pBmpData = (BYTE*) new BYTE[lBmpDataSize];
	memset(pBmpData, 0, lBmpDataSize);
	// 复制 FITS 数据到 BMP 数据空间
	int minPixelCount = 65535;
	int maxPixelCount = 0;
	double dRate = 1.0f / 65535 * 255;
	for (int i = 0; i < m_FSCFitsX.GetHeight(); i++)
	{
		for (int j = 0; j < m_FSCFitsX.GetWidth(); j++)
		{
			int iFValue = m_FSCFitsX.GetFitsData(j, i);
			if (iFValue < minPixelCount) minPixelCount = iFValue;
			if (iFValue > maxPixelCount) maxPixelCount = iFValue;

			iFValue = (float)(iFValue - 0) * dRate;
			if (iFValue < 0) iFValue = 0;
			if (iFValue > 255) iFValue = 255;
			BYTE tempValue;
			memset(&tempValue, 0, 1);
			tempValue = tempValue + BYTE(iFValue);
			// 复制到 BMP 文件数据区
			memcpy_s(pBmpData + (m_FSCFitsX.GetHeight() - i - 1) * m_FSCFitsX.GetWidth() + j, 1, &tempValue, 1);
		}
	}
	TRACE(_T("\nminPixValue = %d\nmaxPixValue = %d\n"), minPixelCount, maxPixelCount);
	// 从外部数据生成 BMP 位图
	m_FSCDibX.LoadFromBuffer(pBmpData, m_FSCFitsX.GetWidth(), m_FSCFitsX.GetHeight(), 8);
	m_FSCDibX.Draw(m_pCDCImgMain, CPoint(0, 0), CSize(780, 780));
	delete[] pBmpData;
	return true;
}