
// ComGoDlg.h : 头文件
//

#pragma once
#define ROWLENGTH 43
#define ROWLENGTH_BYTE 14

// ComGoDlg 对话框
class ComGoDlg : public CDialog
{
// 构造
public:
	ComGoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO16_COMGO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	BOOL m_bCommOpen;			//端口状态

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();

public:
	CListCtrl* m_pCListData;	//列表控件
	CHAR* m_pData;				//存储文本数据
	BYTE* m_pByteData;			//存储字节数据
	UINT m_iRow;				//存储数据行数
	UINT m_iCur;				//存储当前发送的行号
	BOOL m_bPause;				//是否暂停
	HANDLE m_hComm;				//存储已打开的串口句柄
	DCB m_dcb;					//存储串口配置结构体

	CString m_PortNo;	//端口号
	DWORD m_BaudRate;	//波特率
	DWORD m_fParity;	//奇偶校验位
	BYTE m_Parity;		//奇偶校验方法
	BYTE m_ByteSize;	//通信字节位数
	BYTE m_StopBits;	//停止位位数

	DWORD m_dwInterval;	//传输间隔

	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonOpencom();
	afx_msg void OnBnClickedButtonSend();
	HANDLE m_hTransThread;		//存储传输线程句柄
	UINT m_uIDTransThread;		//存储传输线程ID
	HANDLE m_hListenThread;		//存储监听线程句柄
	UINT m_uIDListenThread;		//存储监听线程ID
	BOOL m_bListenOver;			//是否关闭

	// UI
	CFont m_GlobalFont;
	CFont m_ListHeaderFont;
};
