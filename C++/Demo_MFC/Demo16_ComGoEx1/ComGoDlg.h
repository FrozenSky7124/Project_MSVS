
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
	BOOL m_bCommOpen;			//�˿�״̬

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
	UINT m_iRow;				//�洢��������
	UINT m_iCur;				//�洢��ǰ���͵��к�
	BOOL m_bPause;				//�Ƿ���ͣ
	HANDLE m_hComm;				//�洢�Ѵ򿪵Ĵ��ھ��
	DCB m_dcb;					//�洢�������ýṹ��

	CString m_PortNo;	//�˿ں�
	DWORD m_BaudRate;	//������
	DWORD m_fParity;	//��żУ��λ
	BYTE m_Parity;		//��żУ�鷽��
	BYTE m_ByteSize;	//ͨ���ֽ�λ��
	BYTE m_StopBits;	//ֹͣλλ��

	DWORD m_dwInterval;	//������

	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonOpencom();
	afx_msg void OnBnClickedButtonSend();
	HANDLE m_hTransThread;		//�洢�����߳̾��
	UINT m_uIDTransThread;		//�洢�����߳�ID
	HANDLE m_hListenThread;		//�洢�����߳̾��
	UINT m_uIDListenThread;		//�洢�����߳�ID
	BOOL m_bListenOver;			//�Ƿ�ر�

	// UI
	CFont m_GlobalFont;
	CFont m_ListHeaderFont;
};
