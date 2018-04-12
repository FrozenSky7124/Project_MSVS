#pragma once


// SubWinDlg 对话框

class SubWinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SubWinDlg)

public:
	SubWinDlg(CWnd* pParent = NULL);   // 标准构造函数
	SubWinDlg(UINT uFlag, CWnd* pParent = NULL); // 重载构造函数
	virtual ~SubWinDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUBWIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	UINT m_uFlag;

};
