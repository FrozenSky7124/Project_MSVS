
// CameraAlphaDlg.h : 头文件
//

#pragma once

// 引用相机SDK
#include "CameraApi.h"
#include "afxwin.h"

#ifdef _WIN64
#pragma comment(lib, "MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "MVCAMSDK.lib")
#endif

// CameraAlphaDlg 对话框
class CameraAlphaDlg : public CDialogEx
{
// 构造
public:
	CameraAlphaDlg(CWnd* pParent = NULL);	// 标准构造函数

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
	afx_msg void OnBnClickedCaptStart();			//BUTTON:开始采集
	afx_msg void OnBnClickedBtnCaptStop();		//BUTTON:停止采集
	afx_msg void OnBnClickedBtnOpenfile();		//BUTTON:打开文件
	afx_msg void OnBnClickedBtnReviewStart();	//BUTTON:开始回放
	afx_msg void OnBnClickedBtnReviewstop();	//BUTTON:停止回放
	afx_msg void OnBnClickedBtnQuit();			//BUTTON:退出程序
	afx_msg void OnBnClickedBtnConfig();			//BUTTON:相机设置
	afx_msg void OnBnClickedRadioModeCapt();	//RADIO:采集模式
	afx_msg void OnBnClickedRadioModeReview();	//RADIO:回放模式
	afx_msg void OnBnClickedBtnBrowse();			//BUTTON:路径选择
	afx_msg void OnSelchangeComboSavefile();	//COMBO-BOX:消息处理函数
	afx_msg void OnStnClickedStaticSavepath();	//STATIC_LABEL:单击响应函数
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
	
	struct SConfig
	{
		BOOL bSaveFile;
		TCHAR sSaveFolder[MAX_PATH];
	};

	void ConfigSave();							//配置文件保存函数
	void ConfigRead();							//配置文件读取函数

public:
	CStatic			m_cPreview;			//用于显示相机图像的窗口控件
	CDC*			m_pCDC;
	CameraHandle	m_hCamera;			//相机的设备句柄
	UINT			m_threadID;			//图像抓取线程的ID
	HANDLE			m_hDispThread;		//图像抓取线程的句柄
	UINT			m_tIDReviewThread;	//回放线程ID
	HANDLE			m_hReviewThread;		//回放线程句柄
	BOOL			m_bExit;				//用来通知图像抓取线程结束
	BOOL			m_bReviewEnd;			//控制回放线程结束

	tSdkFrameHead	m_sFrInfo;			//用于保存当前图像帧的帧头信息
	int				m_iDispFrameNum;		//用于记录当前已经显示的图像帧的数量

	BYTE*			m_pFrameBuffer;		//用于将原始图像数据转换为RGB的缓冲区
	BOOL			m_bPause;				//是否暂停图像
	BOOL			m_bSaveFile;			//是否保存图像
	CString			m_csSaveFolder;		//用于存储图像保存文件夹位置
	CString			m_csReviewFolder;		//用于存储回放文件夹位置
	LONG			m_lReviewFileCount; //用于存储回放文件数量
	LONG			m_lCurReview;		//用于存储当前回放的图像编号
	CString**		m_pReviewFileName;		//用于存储回放文件的文件名数组的指针
	BOOL			InitCamera();			//相机初始化函数

	int				m_iMode;				//模式参数 0：采集模式 1：回放模式
	CButton			m_BtnConfig;			//相机设置按钮

};