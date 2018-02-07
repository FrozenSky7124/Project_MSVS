
// BasicCamDlg.h : 头文件
//

#pragma once

// 引用相机SDK
#include "CameraApi.h"

#ifdef _WIN64
#pragma comment(lib, "MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "MVCAMSDK.lib")
#endif

// BasicCamDlg 对话框
class BasicCamDlg : public CDialogEx
{
// 构造
public:
	BasicCamDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	CStatic	        m_cPreview;		//用于显示相机图像的窗口控件
	CameraHandle    m_hCamera;		//相机的设备句柄
	UINT            m_threadID;		//图像抓取线程的ID
	HANDLE          m_hDispThread;	//图像抓取线程的句柄
	BOOL            m_bExit;		//用来通知图像抓取线程结束

	tSdkFrameHead   m_sFrInfo;		//用于保存当前图像帧的帧头信息
	int	            m_iDispFrameNum;//用于记录当前已经显示的图像帧的数量

	BYTE*           m_pFrameBuffer;	//用于将原始图像数据转换为RGB的缓冲区
	BOOL	        m_bPause;		//是否暂停图像
	BOOL InitCamera();

};