
// CuteUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Demo21_CuteUI.h"
#include "CuteUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 通过资源文件加载图像（支持PNG）
BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, CImage& CImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), sTR);
	if (!hRsrc) return FALSE;
	// Load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc) return FALSE;
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy_s(pmem, len, lpRsrc, len);
	GlobalUnlock(m_hMem);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// Load from stream
	CImg.Load(pstm);
	// Free/Release stuff
	pstm->Release();
	FreeResource(lpRsrc);
	GlobalFree(m_hMem);
	return TRUE;
}

// CuteUIDlg dialog



CuteUIDlg::CuteUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEMO21_CUTEUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CuteUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CAPTION, m_UICaption);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_btnMin);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
}

BEGIN_MESSAGE_MAP(CuteUIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CuteUIDlg message handlers

BOOL CuteUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_UICaption.Init(0, 0, 1280, 25);

	SetClassLong(this->m_hWnd, GCL_STYLE, GetClassLong(this->m_hWnd, GCL_STYLE) | CS_DROPSHADOW);

	CImage img;
	ImageFromIDResource(IDB_PNG1, L"PNG", img);

	CImage imgBk;
	imgBk.Load(L".\\res\\imgBk.png");
	for (int i = 0; i < imgBk.GetWidth(); ++i)
	{
		for (int j = 0; j < imgBk.GetHeight(); ++j)
		{
			unsigned char* pucColor = (unsigned char*)imgBk.GetPixelAddress(i , j);
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
	CBitmap bmpTmp;
	bmpTmp.Attach(imgBk.Detach());
	m_bkBrush.CreatePatternBrush(&bmpTmp);

	m_btnMin.SetImagePath(_T(".\\res\\btn_minimize.png"), _T(".\\res\\Background.png"));
	m_btnMin.InitMyButton(1280 - 27 * 2, 2, 27, 21, true);
	m_btnClose.SetImagePath(_T(".\\res\\btn_close.png"), _T(".\\res/Background.png"));
	m_btnClose.InitMyButton(1280 - 27 * 1, 2, 27, 21, true);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CuteUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CuteUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CuteUIDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RECT rcCaption;
	m_UICaption.GetWindowRect(&rcCaption); //获取 m_UICaption 对象在屏幕中的区域
	if (point.y < rcCaption.bottom && point.x < rcCaption.right && point.x > rcCaption.left && point.y > rcCaption.top)
		return HTCAPTION; //判断鼠标在区域内时，返回 HTCAPTION，欺骗对话框这里是标题区

	return CDialog::OnNcHitTest(point);
}


HBRUSH CuteUIDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
		return m_bkBrush;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
