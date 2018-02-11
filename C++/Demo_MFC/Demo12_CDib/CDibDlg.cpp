
// CDibDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo12_CDib.h"
#include "CDibDlg.h"
#include "afxdialogex.h"
#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDibDlg �Ի���



CDibDlg::CDibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO12_CDIB_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDibDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CDibDlg::OnBnClickedButtonShow)
END_MESSAGE_MAP()


// CDibDlg ��Ϣ�������

BOOL CDibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDibDlg::OnPaint()
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
HCURSOR CDibDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDibDlg::OnBnClickedButtonShow()
{
	CDib tDib;
	CRect tRect;
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&tRect);
	tDib.LoadFromFile(_T("X:\\CZ-5.bmp"));
	tDib.Draw(m_pCDC, CPoint(0, 0), CSize(tRect.Width(), tRect.Height()));
	return;
}
