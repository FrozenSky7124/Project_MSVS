
// GDI_MainDlg.h : ͷ�ļ�
//

#pragma once


// GDI_MainDlg �Ի���
class GDI_MainDlg : public CDialogEx
{
// ����
public:
	GDI_MainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO17_GDI_DIALOG };
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

	afx_msg void OnBnClickedBtnTest();

	DECLARE_MESSAGE_MAP()

public:
	HBITMAP m_hBmpBackground;
};
