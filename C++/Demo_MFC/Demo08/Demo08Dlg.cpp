#include "stdafx.h"
#include "Demo08.h"
#include "Demo08Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemo08Dlg 对话框



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
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CDemo08Dlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CDemo08Dlg::OnBnClickedButtonChange)
END_MESSAGE_MAP()


// CDemo08Dlg 消息处理程序

BOOL CDemo08Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CListCtrl *obsList = (CListCtrl *)GetDlgItem(IDC_LIST);
	obsList->InsertColumn(0, _T("观测员序号"), LVCFMT_LEFT, 75);
	obsList->InsertColumn(1, _T("观测计划"), LVCFMT_LEFT, 160);
	obsList->InsertColumn(2, _T("捕获目标数"), LVCFMT_LEFT, 100);
	obsList->InsertColumn(3, _T("日期"), LVCFMT_LEFT, 196);
	obsList->SetBkColor(RGB(205, 226, 252));
	obsList->SetTextBkColor(RGB(205, 226, 252));
	obsList->SetExtendedStyle(obsList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_bChanged = FALSE;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDemo08Dlg::OnPaint()
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
HCURSOR CDemo08Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemo08Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CDemo08Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bChanged)
	{
		if (IDYES == AfxMessageBox(_T("是否保存更改？"), MB_YESNO | MB_ICONQUESTION))
		{
			OnBnClickedButtonSave();
		}
	}
	CDialogEx::OnCancel();
}

void CDemo08Dlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
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
	m_bChanged = TRUE;
}


void CDemo08Dlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
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
			m_bChanged = TRUE;
		}
	}
}


void CDemo08Dlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile saveFile;
	CListCtrl *obsList = (CListCtrl *)GetDlgItem(IDC_LIST);
	if (!saveFile.Open(_T("./ObsData.TxT"), CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox(_T("无法打开ObsData.TxT文件！"));
		return;
	}
	int i = 0;
	int obsCount = obsList->GetItemCount();
	OBSInfo obsInfo;
	while (i < obsCount)
	{
		obsInfo.obsID = _tstoi(obsList->GetItemText(i, 0));
		_tcscpy_s(obsInfo.obsPlan, obsList->GetItemText(i, 1));
		obsInfo.tarCount = _tstoi(obsList->GetItemText(i, 2));
		obsInfo.obsDate.ParseDateTime(obsList->GetItemText(i, 3), VAR_DATEVALUEONLY);
		saveFile.Write(&obsInfo, sizeof(obsInfo));
		i++;
	}
	saveFile.Close();
}


void CDemo08Dlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile loadFile;
	CListCtrl *obsList = (CListCtrl *)GetDlgItem(IDC_LIST);
	if (!loadFile.Open(_T("./ObsData.TxT"), CFile::modeRead))
	{
		AfxMessageBox(_T("无法打开ObsData.TxT文件！"));
		return;
	}
	OBSInfo obsInfo;
	int i = 0;
	CString tempStr;
	while (loadFile.Read(&obsInfo, sizeof(obsInfo)) == sizeof(obsInfo))
	{
		tempStr.Format(_T("%d"), obsInfo.obsID);
		obsList->InsertItem(i, tempStr);
		obsList->SetItemText(i, 1, obsInfo.obsPlan);
		tempStr = _T("");
		tempStr.Format(_T("%d"), obsInfo.tarCount);
		obsList->SetItemText(i, 2, tempStr);
		//obsList->SetItemText(i, 3, obsInfo.obsDate.Format(VAR_DATEVALUEONLY));
		tempStr = obsInfo.obsDate.Format(_T("%Y年%m月%d日")); // 调用COleDataTime类Format函数对时间格式化
		obsList->SetItemText(i, 3, tempStr);
		i++;
	}
	loadFile.Close();
}

void CDemo08Dlg::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码
	CListCtrl *obsList = (CListCtrl *)GetDlgItem(IDC_LIST);
	UINT nSelected = obsList->GetSelectedCount();
	if (nSelected == 0)
	{
		AfxMessageBox(_T("请选择要修改的项目！"));
		return;
	}
	
	CString tempStr;
	POSITION pos = obsList->GetFirstSelectedItemPosition();

	while (nSelected != 0)
	{
		int index = obsList->GetNextSelectedItem(pos);
		GetDlgItemText(IDC_EDIT_OBSPLAN, tempStr);
		obsList->SetItemText(index, 1, tempStr);
		GetDlgItemText(IDC_EDIT_TARGET, tempStr);
		obsList->SetItemText(index, 2, tempStr);
		GetDlgItemText(IDC_DATETIMEPICKER_OBSDATE, tempStr);
		obsList->SetItemText(index, 3, tempStr);
		nSelected--;
	}
	m_bChanged = TRUE;
}
