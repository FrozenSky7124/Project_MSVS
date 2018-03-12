
// ImgViewerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo15_ImgViewer.h"
#include "ImgViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ImgViewerDlg �Ի���



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


// ImgViewerDlg ��Ϣ�������

BOOL ImgViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// ���ÿؼ�λ��
	this->GetClientRect(&m_mainRect);
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&m_viewRect);
	m_viewRect.left = m_mainRect.left + 10;
	m_viewRect.top = m_mainRect.top + 10;
	m_viewRect.right = m_mainRect.right - 10;
	m_viewRect.bottom = m_mainRect.bottom - 40;
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(m_viewRect);

	// ���ó�Ա����
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ImgViewerDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR ImgViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void ImgViewerDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


void ImgViewerDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}


void ImgViewerDlg::OnDropFiles(HDROP hDropInfo)
{
	// ��ȡ�����ļ�����
	UINT iFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount > 1) return;

	// ��ȡ�����ļ�·��
	TCHAR tcFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcFilePath, _countof(tcFilePath));
	TRACE(_T("FilePath: %s\n"), tcFilePath);

	// �ж��ļ�����
	// ......

	// ��������
	m_cDibImage.LoadFromFile(tcFilePath);
	m_cDibImage.Draw(m_pCDC, CPoint(0, 0), m_viewRect.Size());

	CDialogEx::OnDropFiles(hDropInfo);
}


void ImgViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if ((point.x >= m_viewRect.left + 32) && (point.x <= m_viewRect.right - 32) && 
		(point.y >= m_viewRect.top + 32) && (point.y <= m_viewRect.bottom - 32))
	{
		CString tempStr;
		tempStr.Format(_T("MouseX: %4d MouseY: %4d"), point.x, point.y);
		SetDlgItemText(IDC_STATIC_TIPS, tempStr);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
