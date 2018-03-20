
// ImgViewerDlg.h : ͷ�ļ�
//

#pragma once
#include "Dib_Ex.h"

// ImgViewerDlg �Ի���
class ImgViewerDlg : public CDialogEx
{
// ����
public:
	ImgViewerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO15_IMGVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
public:
	CRect m_mainRect;
	CRect m_viewRect;
	CDib m_cDibImage;
	CDC* m_pCDC;

protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};