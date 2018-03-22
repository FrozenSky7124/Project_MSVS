
// ComGoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo16_ComGo.h"
#include "ComGoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ComGoDlg 对话框

// CHAR2INT
UINT ConvChar2Int(CHAR _In_CHAR_)
{
	UINT _Out_UINT_;
	if (_In_CHAR_ >= '0' && _In_CHAR_ <= '9')
	{
		_Out_UINT_ = _In_CHAR_ - '0';
	}
	else
	{
		_Out_UINT_ = _In_CHAR_ - 'A' + 10;
	}
	return _Out_UINT_;
}

// 传输线程
UINT WINAPI uiTransThread(LPVOID lpParam)
{
	ComGoDlg* pThis = (ComGoDlg*)lpParam;
	CListCtrl* pListCtrl = (CListCtrl*)pThis->m_pCListData;
	BYTE* pByteData = pThis->m_pByteData;
	DWORD dwBytesSent;
	UINT iRow = pThis->m_iRow;
	BYTE bSend[ROWLENGTH_BYTE];
	memset(bSend, 0, ROWLENGTH_BYTE);

	DWORD iTime1 = GetTickCount(); //开始时间
	while (pThis->m_iCur < iRow)
	{
		memcpy_s(bSend, ROWLENGTH_BYTE, pByteData + pThis->m_iCur * ROWLENGTH_BYTE, ROWLENGTH_BYTE);
		WriteFile(pThis->m_hComm, bSend, ROWLENGTH_BYTE, &dwBytesSent, NULL);
		if (dwBytesSent != ROWLENGTH_BYTE) break; //检测传输异常
		if (pThis->m_bPause == TRUE) break;
		pListCtrl->SetItemText(pThis->m_iCur, 2, _T("SEND"));
		pThis->m_iCur++;
		Sleep(pThis->m_dwInterval);
	}
	DWORD iTime2 = GetTickCount(); //结束时间

	CString tempStr;
	if (dwBytesSent != ROWLENGTH_BYTE)
	{
		tempStr.Format(_T("检测到传输异常，终止传输！\n当前已传输数据 %d 本次耗时 %dms"), pThis->m_iCur, iTime2 - iTime1);
		MessageBox(pThis->GetSafeHwnd(), tempStr, _T("QAQ"), MB_OK | MB_ICONERROR);
	}
	else
	{
		if (pThis->m_bPause == TRUE)
			tempStr.Format(_T("传输暂停！ 已传输数据 %d 本次耗时 %dms"), pThis->m_iCur, iTime2 - iTime1);
		else
			tempStr.Format(_T("传输完成！ 已传输数据 %d 本次耗时 %dms"), pThis->m_iCur, iTime2 - iTime1);
	}
	pThis->SetDlgItemText(IDC_STATIC_STATUS, tempStr);

	// 数据重置
	if (pThis->m_iCur == iRow)
	{
		pThis->m_iCur = 0;
		pThis->m_bPause = TRUE;
		pThis->SetDlgItemText(IDC_BUTTON_SEND, _T("发送数据"));
	}

	return 0;
}

ComGoDlg::ComGoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DEMO16_COMGO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ComGoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ComGoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &ComGoDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_OPENCOM, &ComGoDlg::OnBnClickedButtonOpencom)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &ComGoDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// ComGoDlg 消息处理程序

BOOL ComGoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 成员变量设置
	m_pData = NULL;
	m_pByteData = NULL;
	m_hComm = NULL;
	m_hTransThread = NULL;
	m_uIDTransThread = NULL;
	m_iRow = 0;
	m_iCur = 0;
	m_bCommOpen = FALSE;
	m_bPause = TRUE;
	m_dwInterval = 20;
	m_BaudRate = 115200;
	m_ByteSize = 8;
	m_Parity = NOPARITY;
	m_StopBits = ONESTOPBIT;

	// 控件设置
	m_pCListData = (CListCtrl*)GetDlgItem(IDC_LIST_DATA);
	m_pCListData->InsertColumn(0, _T(" 序号"), LVCFMT_LEFT, 75);
	m_pCListData->InsertColumn(1, _T(" 数据"), LVCFMT_LEFT, 350);
	m_pCListData->InsertColumn(2, _T(" 状态"), LVCFMT_LEFT, 100);
	//m_pCListData->SetBkColor(RGB(205, 226, 252));
	//m_pCListData->SetTextBkColor(RGB(205, 226, 252));
	m_pCListData->SetExtendedStyle(m_pCListData->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); //设置CListCtrl控件的样式

	((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->SetCurSel(3);
	SetDlgItemInt(IDC_COMBO_BTL, m_BaudRate);
	SetDlgItemInt(IDC_COMBO_SJW, m_ByteSize);
	((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->SetCurSel(0);

	SetDlgItemInt(IDC_EDIT_TIME, m_dwInterval);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void ComGoDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR ComGoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void ComGoDlg::OnOK()
{

	//CDialog::OnOK();
}


void ComGoDlg::OnCancel()
{
	// 关闭端口
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	// 清空数据
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	if (m_pByteData != NULL)
	{
		delete[] m_pByteData;
		m_pByteData = NULL;
	}

	CDialog::OnCancel();
}


void ComGoDlg::OnBnClickedButtonLoad()
{
	// 清空数据
	m_pCListData->DeleteAllItems();
	m_iCur = 0;
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	if (m_pByteData != NULL)
	{
		delete[] m_pByteData;
		m_pByteData = NULL;
	}

	// 控件重置
	m_bPause = TRUE;
	SetDlgItemText(IDC_BUTTON_SEND, _T("发送数据"));

	// 获取当前工作目录
	TCHAR curDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curDir);

	// 调用CFileDialog打开文件
	CString filePath;
	CString defaultDir = curDir; //默认目录设置
	CString whiteList = _T("数据文件 (*.txt)|*.txt||"); //文件扩展名白名单
	CFileDialog openFileDlg(TRUE, curDir, NULL, OFN_HIDEREADONLY | OFN_READONLY, whiteList, this);
	if (IDOK == openFileDlg.DoModal())
	{
		filePath = openFileDlg.GetPathName(); //获取文件路径
		SetDlgItemText(IDC_STATIC_STATUS, filePath); //状态栏显示
	}
	else
	{
		return;
	}

	// 读取文件
	CFile loadFile;
	CFileException e_loadFile;
	if (!loadFile.Open(filePath, CFile::modeRead, &e_loadFile))
	{
		AfxMessageBox(_T("无法打开文件！ Error: %d"), e_loadFile.m_cause);
		return;
	}
	ULONGLONG dwFileLength = loadFile.GetLength();
	if (dwFileLength % ROWLENGTH != 0) //数据文件格式异常
	{
		AfxMessageBox(_T("检测到文件数据异常，请核对数据格式！"));
		loadFile.Close();
		return;
	}

	m_pData = new CHAR[dwFileLength]; //分配空间
	if (NULL == m_pData)
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("空间分配异常！ TextData"));
		return;
	}
	memset(m_pData, 0, dwFileLength);

	if (loadFile.Read(m_pData, dwFileLength) != dwFileLength) //读取数据
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("文件读取异常！"));
		delete[] m_pData;
		loadFile.Close();
		return;
	}
	loadFile.Close();

	// 转换文本数据为BYTE数据
	UINT iRow = dwFileLength / ROWLENGTH; //数据行数
	m_iRow = dwFileLength / ROWLENGTH;
	m_pByteData = new BYTE[iRow * ROWLENGTH_BYTE]; //分配空间
	if (NULL == m_pByteData)
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("空间分配异常！ ByteData"));
		return;
	}
	memset(m_pByteData, 0, iRow * ROWLENGTH_BYTE);
	UINT index = 0;
	for (UINT tRow = 0; tRow < iRow; tRow++ )
	{
		for (UINT tCol = 0; tCol < ROWLENGTH_BYTE; tCol++ )
		{
			UINT tempH = ConvChar2Int(m_pData[tRow * ROWLENGTH + 3 * tCol + 0]);
			UINT tempL = ConvChar2Int(m_pData[tRow * ROWLENGTH + 3 * tCol + 1]);
			m_pByteData[index] = tempH << 4 | tempL;
			index++;
		}
	}

	CString tempStr;
	CHAR tempChr[ROWLENGTH + 1]; //用于存储一行文本的临时变量，多1字节用于字符串结尾
	memset(tempChr, 0, ROWLENGTH + 1);
	for (UINT i = 0; i < iRow; i++)
	{
		tempStr.Format(_T("%4d"), i + 1);
		m_pCListData->InsertItem(i, tempStr);
		memcpy_s(tempChr, ROWLENGTH, m_pData + i * ROWLENGTH, ROWLENGTH);
		tempStr.Format(_T("%s"), tempChr);
		m_pCListData->SetItemText(i, 1, tempStr);
	}
	SetDlgItemText(IDC_STATIC_STATUS, _T("数据读取完成！"));
}


void ComGoDlg::OnBnClickedButtonOpencom()
{
	if ((m_hComm != NULL) && (m_bCommOpen == FALSE))
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("端口状态异常，无法打开！"));
		return;
	}
	if ((m_hComm != NULL) && (m_bCommOpen == TRUE)) //若端口状态为已打开，则关闭端口
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
		SetDlgItemText(IDC_BUTTON_OPENCOM, _T("开启端口"));
		((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(TRUE);
		return;
	}

	// 获取端口号
	((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->GetWindowText(m_PortNo);

	m_hComm = CreateFile(m_PortNo, //端口名称(COMx)
		GENERIC_READ | GENERIC_WRITE, //端口属性为可读写
		0, //端口设备必须被独占性的访问
		NULL, //无安全属性
		OPEN_EXISTING, //端口设备必须使用OPEN_EXISTING参数
		FILE_ATTRIBUTE_NORMAL, //同步式 I/O
		0); //对于端口设备而言此参数必须为0
	if (m_hComm == INVALID_HANDLE_VALUE) //如果该端口不存在或者正被另外一个应用程序使用，则打开失败
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("端口无法打开！该端口不存在或正被其他应用程序占用。"));
		MessageBoxEx(AfxGetMainWnd()->m_hWnd, _T("端口无法打开！\n该端口不存在或正被其他应用程序占用。"), _T("QAQ"), MB_OK | MB_ICONERROR, 0);
		m_hComm = NULL;
		return;
	}
	
	// 端口设置
	GetCommState(m_hComm, &m_dcb); //获取该端口的默认参数
	m_dcb.BaudRate = m_BaudRate = GetDlgItemInt(IDC_COMBO_BTL); //修改波特率
	m_dcb.ByteSize = m_ByteSize = GetDlgItemInt(IDC_COMBO_SJW); //通信字节位数
	m_dcb.Parity = m_Parity = ((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->GetCurSel(); //奇偶校验方法，EVENPARITY：偶校验，NOPARITY：无校验，MARKPARITY：标记校验，ODDPARITY：奇校验
	m_dcb.StopBits = m_StopBits = ((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->GetCurSel(); //停止位的位数，ONESTOPBIT：1位停止位，TWOSTOPBITS：2位停止位，ONE5STOPBITS，1.5位停止位
	m_dcb.fParity = 1; //奇偶校验位
	UINT iResult = SetCommState(m_hComm, &m_dcb); //重新设置参数
	if (iResult)
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("端口打开成功！"));
		m_bCommOpen = TRUE;
		SetDlgItemText(IDC_BUTTON_OPENCOM, _T("关闭端口"));
		((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(FALSE);
	}
	else
	{
		// 若端口设置异常，未被操作系统接受，则重设控件的值
		GetCommState(m_hComm, &m_dcb);
		SetDlgItemInt(IDC_COMBO_BTL, m_BaudRate = m_dcb.BaudRate);
		SetDlgItemInt(IDC_COMBO_SJW, m_ByteSize = m_dcb.ByteSize);
		((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->SetCurSel(m_Parity = m_dcb.Parity);
		((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->SetCurSel(m_StopBits = m_dcb.StopBits);
		m_fParity = m_dcb.fParity;

		CString tempStrRst;
		tempStrRst.Format(_T("端口打开成功！\n由于参数设置异常，已重置： \n[波特率] %d \n[数据位] %d \n[校验位] %d \n[停止位] %d"),
			m_dcb.BaudRate, m_dcb.ByteSize, m_dcb.Parity, m_dcb.StopBits);
		SetDlgItemText(IDC_STATIC_STATUS, tempStrRst);
		MessageBoxEx(AfxGetMainWnd()->m_hWnd, tempStrRst, _T("QwQ"), MB_OK | MB_ICONINFORMATION, 0);
		m_bCommOpen = TRUE;
		SetDlgItemText(IDC_BUTTON_OPENCOM, _T("关闭端口"));
		((CComboBox*)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(FALSE);
	}
}

void ComGoDlg::OnBnClickedButtonSend()
{
	if (NULL == m_hComm)
	{
		AfxMessageBox(_T("端口状态异常，无法传输！"));
		return;
	}
	if (NULL == m_pByteData)
	{
		AfxMessageBox(_T("数据状态异常，无法传输！"));
		return;
	}
	if (m_bPause == FALSE) //若正在传输，则...
	{
		m_bPause = TRUE;
		SetDlgItemText(IDC_BUTTON_SEND, _T("继续发送数据"));
		return;
	}

	UINT i = 0;
	while (i < m_iRow)
	{
		m_pCListData->SetItemText(i++, 2, _T("")); //传输前将列表状态列置为空
	}

	m_dwInterval = GetDlgItemInt(IDC_EDIT_TIME); //获取传输间隔

	GetCommState(m_hComm, &m_dcb); //获取该端口的默认参数
	m_dcb.BaudRate = m_BaudRate = GetDlgItemInt(IDC_COMBO_BTL); //修改波特率
	m_dcb.ByteSize = m_ByteSize = GetDlgItemInt(IDC_COMBO_SJW); //通信字节位数
	m_dcb.Parity = m_Parity = ((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->GetCurSel(); //奇偶校验方法
	m_dcb.StopBits = m_StopBits = ((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->GetCurSel(); //停止位的位数
	m_dcb.fParity = 1; //奇偶校验位
	UINT iResult = SetCommState(m_hComm, &m_dcb); //重新设置参数

	if ((!iResult) && (m_bCommOpen))
	{
		// 若端口设置异常，未被操作系统接受，则重设控件的值
		GetCommState(m_hComm, &m_dcb);
		SetDlgItemInt(IDC_COMBO_BTL, m_BaudRate = m_dcb.BaudRate);
		SetDlgItemInt(IDC_COMBO_SJW, m_ByteSize = m_dcb.ByteSize);
		((CComboBox*)GetDlgItem(IDC_COMBO_JYW))->SetCurSel(m_Parity = m_dcb.Parity);
		((CComboBox*)GetDlgItem(IDC_COMBO_TZW))->SetCurSel(m_StopBits = m_dcb.StopBits);
		m_fParity = m_dcb.fParity;

		CString tempStrRst;
		tempStrRst.Format(_T("由于参数设置异常，已重置： \n[波特率] %d \n[数据位] %d \n[校验位] %d \n[停止位] %d"),
			m_dcb.BaudRate, m_dcb.ByteSize, m_dcb.Parity, m_dcb.StopBits);
		SetDlgItemText(IDC_STATIC_STATUS, tempStrRst);
		MessageBoxEx(AfxGetMainWnd()->m_hWnd, tempStrRst, _T("QwQ"), MB_OK | MB_ICONINFORMATION, 0);
	}

	m_hTransThread = (HANDLE)_beginthreadex(NULL, 0, &uiTransThread, this, 0, &m_uIDTransThread);
	m_bPause = !m_bPause;
	SetDlgItemText(IDC_BUTTON_SEND, _T("暂停发送数据"));
}