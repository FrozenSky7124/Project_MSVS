#pragma once


// SubWinDlg �Ի���

class SubWinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SubWinDlg)

public:
	SubWinDlg(CWnd* pParent = NULL);   // ��׼���캯��
	SubWinDlg(UINT uFlag, CWnd* pParent = NULL); // ���ع��캯��
	virtual ~SubWinDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUBWIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	UINT m_uFlag;

};
