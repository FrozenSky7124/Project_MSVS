
// Demo_PMonDlg.h : ͷ�ļ�
//

#pragma once

#define LISTENPORT 17241 //������ն˿�
#define UDPPKGSIZE 32    //����UDP���ݰ���С

// CDemo_PMonDlg �Ի���
class CDemo_PMonDlg : public CDialogEx
{
// ����
public:
	CDemo_PMonDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_PROCESSMON_DIALOG };
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
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnIDCancel();
	void InitMonitor();
	BOOL InitSocket();
	void SearchProcess();

	// UDP Socket
	WSADATA m_wsaData;
	SOCKET m_socket;
	sockaddr_in m_addrSendto;
};
