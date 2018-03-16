
// ComGoDlg.h : ͷ�ļ�
//

#pragma once
#define ROWLENGTH 43
#define ROWLENGTH_BYTE 14

// ComGoDlg �Ի���
class ComGoDlg : public CDialog
{
// ����
public:
	ComGoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO16_COMGO_DIALOG };
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
	virtual void OnOK();
	virtual void OnCancel();

public:
	CListCtrl* m_pCListData;	//�б�ؼ�
	CHAR* m_pData;				//�洢�ı�����
	BYTE* m_pByteData;			//�洢�ֽ�����
	HANDLE m_hComm;				//�洢�Ѵ򿪵Ĵ��ھ��
	DCB m_dcb;					//�洢�������ýṹ��
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonOpencom();
	afx_msg void OnBnClickedButtonSend();
};
