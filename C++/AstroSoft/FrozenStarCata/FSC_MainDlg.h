
// FSC_MainDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// FSC_MainDlg �Ի���
class FSC_MainDlg : public CDialog
{
// ����
public:
	FSC_MainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainDlg };
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedBtnSelectFile();
	afx_msg void OnBnClickedBtnCreateDB();
	afx_msg void OnBnClickedBtnMake();

	//=================================================
	// Func: Init UI
	//=================================================
	void InitUI();

public:
	CFont m_FontYaHeiConsolas_S;
	CListCtrl m_MainList;
	CProgressCtrl m_MainProgBar;

	int m_iCurMakeNo;           //Cur Process

	HANDLE m_hMakeThread;       //MakeThread Handle
	UINT m_uiMakeThreadID;      //MakeThread ID
};
