#pragma once

// MainDialog �Ի���

class MainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MainDialog)

public:
	MainDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MainDialog();

	CFont m_editFont;
	virtual BOOL OnInitDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnChangeSearchEdit();
private:
	bool OpenDB();
};
