
// Demo08Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo08.h"
#include "Demo08Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemo08Dlg �Ի���



CDemo08Dlg::CDemo08Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO08_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemo08Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDemo08Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDemo08Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDemo08Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDemo08Dlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDemo08Dlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDemo08Dlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CDemo08Dlg ��Ϣ�������

BOOL CDemo08Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CListCtrl *obsList = (CListCtrl *)GetDlgItem(IDC_LIST);
	obsList->InsertColumn(0, _T("�۲�Ա���"), LVCFMT_LEFT, 75);
	obsList->InsertColumn(1, _T("�۲�ƻ�"), LVCFMT_LEFT, 160);
	obsList->InsertColumn(2, _T("����Ŀ����"), LVCFMT_LEFT, 100);
	obsList->InsertColumn(3, _T("����"), LVCFMT_LEFT, 196);
	obsList->SetBkColor(RGB(205, 226, 252));
	obsList->SetTextBkColor(RGB(205, 226, 252));
	obsList->SetExtendedStyle(obsList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDemo08Dlg::OnPaint()
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
HCURSOR CDemo08Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemo08Dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}


void CDemo08Dlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

void CDemo08Dlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListCtrl *pList = (CListCtrl *)GetDlgItem(IDC_LIST);
	CString tempStr;
	UINT nCount = pList->GetItemCount();
	GetDlgItemText(IDC_EDIT_OBSID, tempStr);
	pList->InsertItem(nCount, tempStr);
	GetDlgItemText(IDC_EDIT_OBSPLAN, tempStr);
	pList->SetItemText(nCount, 1, tempStr);
	GetDlgItemText(IDC_EDIT_TARGET, tempStr);
	pList->SetItemText(nCount, 2, tempStr);
	GetDlgItemText(IDC_DATETIMEPICKER_OBSDATE, tempStr);
	pList->SetItemText(nCount, 3, tempStr);
}


void CDemo08Dlg::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListCtrl *obsList = (CListCtrl *)GetDlgItem(IDC_LIST);
	//int index = obsList->GetSelectedColumn();
	int selectNum = obsList->GetSelectedCount();
	if (selectNum != 0)
	{
		int index = -1;
		for (int i = 0; i < selectNum; i++)
		{
			index = obsList->GetNextItem(-1, LVNI_SELECTED);
			ASSERT(index != -1);
			TRACE(_T("Select: %d.\n"), index);
			obsList->DeleteItem(index);
		}
	}
}


void CDemo08Dlg::OnBnClickedButtonSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFile saveFile;
	saveFile.Open(_T("./ObsData.TxT"), CFile::modeCreate | CFile::modeWrite);
	const TCHAR sz[] = _T("I love CFile!");
	saveFile.Write(sz, sizeof(sz));
	saveFile.Close();
}
