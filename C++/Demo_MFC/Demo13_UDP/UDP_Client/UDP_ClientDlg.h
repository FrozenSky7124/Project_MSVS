
// UDP_ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "Dib_Ex.h"

#define MAX_UDPDATA 512

// CUDP_ClientDlg �Ի���
class CUDP_ClientDlg : public CDialogEx
{
// ����
public:
	CUDP_ClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UDP_CLIENT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CDC* m_pCDC;
	WSADATA m_wsaData;
	SOCKET m_socket;
	sockaddr_in m_addrSendto;
	CDib m_cDibImage;

	// �Զ��崫��Э��
	// ���ݰ��ṹ��
	struct UDP_PACKAGE
	{
		UINT flag; //�����ʶλ
		UINT width; //λͼ���
		UINT height; //λͼ�߶�
		UINT bitCount; //λͼ��ɫλ��
		UINT iNum; //���ݰ����
		UINT iTotal; //���ݰ�������
		UINT dataLength; //���ݰ����ݴ�С
		BYTE data[MAX_UDPDATA]; //����
	};

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonSend();

protected:
	BOOL InitSocket();
};
