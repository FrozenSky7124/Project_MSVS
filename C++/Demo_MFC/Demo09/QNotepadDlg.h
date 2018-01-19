
// QNotepadDlg.h : ͷ�ļ�
//

#pragma once


// QNotepadDlg �Ի���
class QNotepadDlg : public CDialogEx
{
// ����
public:
	QNotepadDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO09_DIALOG };
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
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnAppExit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnHelpmenuAbout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnEditmenuSelectall();
	afx_msg void OnEditmenuDatetime();

protected:
	void FileRead_ANSI(CFile& file);
	void FileRead_UTF8(CFile& file);
	void FileRead_UTF16(CFile& file);
};
