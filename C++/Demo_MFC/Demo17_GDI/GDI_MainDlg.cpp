
// GDI_MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo17_GDI.h"
#include "GDI_MainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// GDI_MainDlg �Ի���



GDI_MainDlg::GDI_MainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO17_GDI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void GDI_MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(GDI_MainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnTest, &GDI_MainDlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// GDI_MainDlg ��Ϣ�������

BOOL GDI_MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_hBmpBackground = NULL;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void GDI_MainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		HWND hMainWnd = GetSafeHwnd();
		HDC hMainDC = ::GetDC(hMainWnd);
		// ����ͼƬ
		HDC hMemDC = ::CreateCompatibleDC(hMainDC);
		m_hBmpBackground = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BmpYYS));
		HBITMAP hOldBmp = static_cast<HBITMAP>(::SelectObject(hMemDC, m_hBmpBackground));
		BITMAP bmp;
		::GetObject(m_hBmpBackground, sizeof(BITMAP), &bmp);
		RECT rcClient;
		::GetClientRect(hMainWnd, &rcClient);
		int iWidth = rcClient.right - rcClient.left;
		int iHeight = rcClient.bottom - rcClient.top;
		//::BitBlt(hMainDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
		StretchBlt(hMainDC, 0, 0, iWidth, iHeight, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		::SelectObject(hMemDC, hOldBmp);
		::DeleteDC(hMemDC);
		// �ı�����
		LOGFONT logFont;
		memset(&logFont, 0, sizeof(LOGFONT));
		logFont.lfWidth = 20;
		logFont.lfHeight = 40;
		logFont.lfCharSet = GB2312_CHARSET;
		_tcscpy_s(logFont.lfFaceName, _T("YaHei Consolas Hybrid"));
		HFONT hFont = CreateFontIndirect(&logFont);
		HFONT hOldFont = (HFONT)::SelectObject(hMainDC, hFont);
		COLORREF clrOldText = ::SetTextColor(hMainDC, RGB(0, 0, 128));
		::SetBkMode(hMainDC, TRANSPARENT);
		CString csTitle = _T("FrozenSky7124.GitHub.io");
		DrawText(hMainDC, csTitle, csTitle.GetLength(), &rcClient, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
		::SetTextColor(hMainDC, clrOldText);
		::SelectObject(hMainDC, hOldFont);
		::DeleteObject(hFont);

		//����
		HPEN hPen = CreatePen(PS_DOT, 1, RGB(42, 178, 203));
		HPEN hOldPen = (HPEN)::SelectObject(hMainDC, hPen);
		MoveToEx(hMainDC, 50, 50, NULL);
		LineTo(hMainDC, 200, 300);
		::SelectObject(hMainDC, hOldPen);
		::DeleteObject(hPen);
		
		::ReleaseDC(hMainWnd, hMainDC);
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR GDI_MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void GDI_MainDlg::OnBnClickedBtnTest()
{
	HWND hMainWnd = GetSafeHwnd();
	//HDC hMainDC = ::GetDC(hMainWnd);
	//// ������ͼ��������
	//// ......
	//::ReleaseDC(hMainWnd, hMainDC);
	
	//PAINTSTRUCT ps;
	//HDC hMainDC = ::BeginPaint(hMainWnd, &ps);
	//// ������ͼ��������
	//// ......
	//::EndPaint(hMainWnd, &ps);

	//HDC hMemDC = ::CreateCompatibleDC(hDest);
	//// ������ͼ��������
	//// ......
	//::BitBlt(hDest, x, y, cx, cy, hMemDC, 0, 0, SRCCOPY); //���ڴ�DC������Ŀ��DC
	//::DeleteDC(hMemDC);

	HDC hMainDC = ::GetDC(hMainWnd);
	LOGFONT lf = { 0 };
	lf.lfWidth = 16;
	lf.lfHeight = 40;
	lf.lfCharSet = GB2312_CHARSET;
	HFONT hFont = ::CreateFontIndirect(&lf);
	HFONT hOldFont = static_cast<HFONT>(::SelectObject(hMainDC, (HGDIOBJ)hFont));
	RECT rect;
	::GetClientRect(hMainWnd, &rect);
	DrawText(hMainDC, _T("ABC"), 3, &rect, DT_LEFT);

	::SelectObject(hMainDC, hOldFont);
	::DeleteObject(hFont);
	::ReleaseDC(hMainWnd, hMainDC);
}
