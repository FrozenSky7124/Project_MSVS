
// UDP_ClientDlg.h : 头文件
//

#pragma once
#include "Dib_Ex.h"

#define MAX_UDPDATA 512

// CUDP_ClientDlg 对话框
class CUDP_ClientDlg : public CDialogEx
{
// 构造
public:
	CUDP_ClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UDP_CLIENT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CDC* m_pCDC;
	WSADATA m_wsaData;
	SOCKET m_socket;
	sockaddr_in m_addrSendto;
	CDib m_cDibImage;

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
		BYTE data[MAX_UDPDATA]; //数据
	};

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonSend();

protected:
	BOOL InitSocket();
};
