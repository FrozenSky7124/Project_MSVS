
// UDPDlg.h : ͷ�ļ�
//

#pragma once
#define MAX_UDPDATA 512

// UDPDlg �Ի���
class UDPDlg : public CDialogEx
{
// ����
public:
	UDPDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO13_UDP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HANDLE m_hRecvThread; //���ڴ洢UDP�����߳̾��
	UINT m_iRecvThreadId; //���ڴ洢UDP�����߳��߳�ID

	BOOL SendEndToRecvThread();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	BYTE* m_pBmpData;
	CDC* m_pCDC;
	UINT m_viewWidth;
	UINT m_viewHeight;

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
		DWORD dataTotal; //λͼ��������С
		BYTE data[MAX_UDPDATA]; //����
	};
	// ȷ�ϰ��ṹ��
	struct UDP_CONFIRM
	{
		UINT flag; //��ʶλ
		UINT iRecvNum; //�ѽ��յ��İ����
	};
};
