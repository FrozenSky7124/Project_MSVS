#pragma once


// ModelDlg 对话框

class ModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ModelDlg)

public:
	ModelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ModelDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
