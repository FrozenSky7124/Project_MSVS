
// Demo10Dlg.h : ͷ�ļ�
//

#pragma once

#include "ModelDlg.h"

// CDemo10Dlg �Ի���
class CDemo10Dlg : public CDialogEx
{
// ����
public:
	CDemo10Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO10_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();

// FrozenSky
protected:
	ModelDlg m_modelDlg;
};
