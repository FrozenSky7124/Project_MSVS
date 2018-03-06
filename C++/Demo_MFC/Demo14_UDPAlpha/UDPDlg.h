
// UDPDlg.h : 头文件
//

#pragma once
#define MAX_UDPDATA 512

// UDPDlg 对话框
class UDPDlg : public CDialogEx
{
// 构造
public:
	UDPDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO13_UDP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HANDLE m_hRecvThread; //用于存储UDP接收线程句柄
	UINT m_iRecvThreadId; //用于存储UDP接收线程线程ID

	BOOL SendEndToRecvThread();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	BYTE* m_pBmpData;
	CDC* m_pCDC;
	UINT m_viewWidth;
	UINT m_viewHeight;

	// 自定义传输协议
	// 数据包结构体
	struct UDP_PACKAGE
	{
		UINT flag; //定义标识位
		UINT width; //位图宽度
		UINT height; //位图高度
		UINT bitCount; //位图颜色位数
		UINT iNum; //数据包序号
		UINT iTotal; //数据包总数量
		UINT dataLength; //数据包数据大小
		DWORD dataTotal; //位图数据区大小
		BYTE data[MAX_UDPDATA]; //数据
	};
	// 确认包结构体
	struct UDP_CONFIRM
	{
		UINT flag; //标识位
		UINT iRecvNum; //已接收到的包序号
	};
};
