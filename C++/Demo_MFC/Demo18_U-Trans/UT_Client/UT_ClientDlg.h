
// UT_ClientDlg.h : ͷ�ļ�
//

#pragma once

#define BLOCKSIZE 32768 //data size in UDP package

// UT_ClientDlg �Ի���
class UT_ClientDlg : public CDialog
{
// ����
public:
	UT_ClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UT_CLIENT_DIALOG };
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
	BOOL InitSocket();
	virtual void OnCancel();

	// UDP Socket
	WSADATA m_wsaData;
	SOCKET m_socket;
	sockaddr_in m_addrRecvin;

	// Send thread
	HANDLE m_hRecvThread; //store handle of udp receive thread
	UINT m_iRecvThread; //store id of udp receive thread
};
