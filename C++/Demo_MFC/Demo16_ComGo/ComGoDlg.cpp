
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

	m_pCListData = (CListCtrl*)GetDlgItem(IDC_LIST_DATA);
	m_pCListData->InsertColumn(0, _T("序号"), LVCFMT_LEFT, 75);
	m_pCListData->InsertColumn(1, _T("数据"), LVCFMT_LEFT, 300);
	//m_pCListData->SetBkColor(RGB(205, 226, 252));
	//m_pCListData->SetTextBkColor(RGB(205, 226, 252));
	m_pCListData->SetExtendedStyle(m_pCListData->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); //设置CListCtrl控件的样式

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
	// 关闭串口
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	if (m_pData != NULL)
	{
		delete[] m_pData;
	}
	if (m_pByteData != NULL)
	{
		delete[] m_pByteData;
	}

	CDialog::OnCancel();
}


void ComGoDlg::OnBnClickedButtonLoad()
{
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
	TRACE(_T("FileLength: %d Bytes\n"), dwFileLength);


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
	if (m_hComm != NULL)
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("串口状态异常，无法打开！"));
		return;
	}
	m_hComm = CreateFile("COM4", //串口名称(COMx)
		GENERIC_READ | GENERIC_WRITE, //串口属性为可读写
		0, //串口设备必须被独占性的访问
		NULL, //无安全属性
		OPEN_EXISTING, //串口设备必须使用OPEN_EXISTING参数
		FILE_ATTRIBUTE_NORMAL, //同步式 I/O
		0); //对于串口设备而言此参数必须为0
	if (m_hComm == INVALID_HANDLE_VALUE) //如果该串口不存在或者正被另外一个应用程序使用，则打开失败
	{
		SetDlgItemText(IDC_STATIC_STATUS, _T("串口无法打开！"));
		m_hComm = NULL;
		return;
	}
	
	GetCommState(m_hComm, &m_dcb); //获取该端口的默认参数
	m_dcb.BaudRate = 115200; //修改波特率
	m_dcb.fParity = 1; //奇偶校验位
	m_dcb.Parity = NOPARITY; //奇偶校验方法，EVENPARITY：偶校验，NOPARITY：无校验，MARKPARITY：标记校验，ODDPARITY：奇校验
	m_dcb.ByteSize = 8; //通信字节位数
	m_dcb.StopBits = ONESTOPBIT; //停止位的位数，ONESTOPBIT：1位停止位，TWOSTOPBITS：2位停止位，ONE5STOPBITS，1.5位停止位
	UINT iResult = SetCommState(m_hComm, &m_dcb); //重新设置参数
	if (iResult)
		SetDlgItemText(IDC_STATIC_STATUS, _T("串口打开成功！"));
}

void ComGoDlg::OnBnClickedButtonSend()
{
	char lpBuffer[] = "FrozenSky's COMM!";
	DWORD nNumberOfBytesToWrite = strlen(lpBuffer);
	DWORD nBytesSent;
	WriteFile(m_hComm, lpBuffer, nNumberOfBytesToWrite, &nBytesSent, NULL);
	CString tempStr;
	tempStr.Format(_T("数据发送完成！ 共发送 %d 字节。"), nBytesSent);
	SetDlgItemText(IDC_STATIC_STATUS, tempStr);
}