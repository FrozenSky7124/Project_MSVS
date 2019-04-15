
// U-TransDlg.h : ͷ�ļ�
//

#pragma once

#define BLOCKSIZE 32768 //data size in UDP package

// UTransDlg �Ի���
class UTransDlg : public CDialog
{
// ����
public:
	UTransDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO18_UTRANS_DIALOG };
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
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedQuit();
	virtual void OnCancel();
	BOOL InitSocket();
	BOOL LoadFile(CString csPath);

	// File info
	BYTE * m_pFileData; //store ptr for file data in bytes
	ULONGLONG m_lFileLength; //store file length in bytes

	// UDP Socket
	WSADATA m_wsaData;
	SOCKET m_socket;
	sockaddr_in m_addrSendto;

	// Send thread
	HANDLE m_hSendThread; //store handle of udp sender thread
	UINT m_iSendThread; //store id of udp sender thread
};
