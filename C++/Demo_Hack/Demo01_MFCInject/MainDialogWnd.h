#pragma once


// CMainDialogWnd �Ի���

class CMainDialogWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDialogWnd)

public:
	CMainDialogWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDialogWnd();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
