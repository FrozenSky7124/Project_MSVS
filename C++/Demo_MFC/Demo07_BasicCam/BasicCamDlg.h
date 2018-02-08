
// BasicCamDlg.h : ͷ�ļ�
//

#pragma once

// �������SDK
#include "CameraApi.h"

#ifdef _WIN64
#pragma comment(lib, "MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "MVCAMSDK.lib")
#endif

// BasicCamDlg �Ի���
class BasicCamDlg : public CDialogEx
{
// ����
public:
	BasicCamDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
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
	afx_msg void OnBnClickedCaptStart();	//BUTTON:��ʼ�ɼ�
	afx_msg void OnBnClickedBtnCaptStop();	//BUTTON:ֹͣ�ɼ�
	afx_msg void OnBnClickedBtnQuit();		//BUTTON:�˳�����
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	CStatic	        m_cPreview;		//������ʾ���ͼ��Ĵ��ڿؼ�
	CameraHandle    m_hCamera;		//������豸���
	UINT            m_threadID;		//ͼ��ץȡ�̵߳�ID
	HANDLE          m_hDispThread;	//ͼ��ץȡ�̵߳ľ��
	BOOL            m_bExit;		//����֪ͨͼ��ץȡ�߳̽���

	tSdkFrameHead   m_sFrInfo;		//���ڱ��浱ǰͼ��֡��֡ͷ��Ϣ
	int	            m_iDispFrameNum;//���ڼ�¼��ǰ�Ѿ���ʾ��ͼ��֡������

	BYTE*           m_pFrameBuffer;	//���ڽ�ԭʼͼ������ת��ΪRGB�Ļ�����
	BOOL	        m_bPause;		//�Ƿ���ͣͼ��
	BOOL            InitCamera();

	int				m_iMode;		//ģʽ���� 0���ɼ�ģʽ 1���ط�ģʽ
	afx_msg void OnBnClickedRadioModeCapt();
	afx_msg void OnBnClickedRadioModeReview();
};