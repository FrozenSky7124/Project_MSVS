#pragma once


// ModelDlg �Ի���

class ModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ModelDlg)

public:
	ModelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ModelDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
