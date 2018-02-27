
// CameraAlphaDlg.h : ͷ�ļ�
//

#pragma once

// �������SDK
#include "CameraApi.h"
#include "afxwin.h"

#ifdef _WIN64
#pragma comment(lib, "MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "MVCAMSDK.lib")
#endif

// CameraAlphaDlg �Ի���
class CameraAlphaDlg : public CDialogEx
{
// ����
public:
	CameraAlphaDlg(CWnd* pParent = NULL);	// ��׼���캯��

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
	afx_msg void OnBnClickedCaptStart();			//BUTTON:��ʼ�ɼ�
	afx_msg void OnBnClickedBtnCaptStop();		//BUTTON:ֹͣ�ɼ�
	afx_msg void OnBnClickedBtnOpenfile();		//BUTTON:���ļ�
	afx_msg void OnBnClickedBtnReviewStart();	//BUTTON:��ʼ�ط�
	afx_msg void OnBnClickedBtnReviewstop();	//BUTTON:ֹͣ�ط�
	afx_msg void OnBnClickedBtnQuit();			//BUTTON:�˳�����
	afx_msg void OnBnClickedBtnConfig();			//BUTTON:�������
	afx_msg void OnBnClickedRadioModeCapt();	//RADIO:�ɼ�ģʽ
	afx_msg void OnBnClickedRadioModeReview();	//RADIO:�ط�ģʽ
	afx_msg void OnBnClickedBtnBrowse();			//BUTTON:·��ѡ��
	afx_msg void OnSelchangeComboSavefile();	//COMBO-BOX:��Ϣ������
	afx_msg void OnStnClickedStaticSavepath();	//STATIC_LABEL:������Ӧ����
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
	
	struct SConfig
	{
		BOOL bSaveFile;
		TCHAR sSaveFolder[MAX_PATH];
	};

	void ConfigSave();							//�����ļ����溯��
	void ConfigRead();							//�����ļ���ȡ����

public:
	CStatic			m_cPreview;			//������ʾ���ͼ��Ĵ��ڿؼ�
	CDC*			m_pCDC;
	CameraHandle	m_hCamera;			//������豸���
	UINT			m_threadID;			//ͼ��ץȡ�̵߳�ID
	HANDLE			m_hDispThread;		//ͼ��ץȡ�̵߳ľ��
	UINT			m_tIDReviewThread;	//�ط��߳�ID
	HANDLE			m_hReviewThread;		//�ط��߳̾��
	BOOL			m_bExit;				//����֪ͨͼ��ץȡ�߳̽���
	BOOL			m_bReviewEnd;			//���ƻط��߳̽���

	tSdkFrameHead	m_sFrInfo;			//���ڱ��浱ǰͼ��֡��֡ͷ��Ϣ
	int				m_iDispFrameNum;		//���ڼ�¼��ǰ�Ѿ���ʾ��ͼ��֡������

	BYTE*			m_pFrameBuffer;		//���ڽ�ԭʼͼ������ת��ΪRGB�Ļ�����
	BOOL			m_bPause;				//�Ƿ���ͣͼ��
	BOOL			m_bSaveFile;			//�Ƿ񱣴�ͼ��
	CString			m_csSaveFolder;		//���ڴ洢ͼ�񱣴��ļ���λ��
	CString			m_csReviewFolder;		//���ڴ洢�ط��ļ���λ��
	LONG			m_lReviewFileCount; //���ڴ洢�ط��ļ�����
	LONG			m_lCurReview;		//���ڴ洢��ǰ�طŵ�ͼ����
	CString**		m_pReviewFileName;		//���ڴ洢�ط��ļ����ļ��������ָ��
	BOOL			InitCamera();			//�����ʼ������

	int				m_iMode;				//ģʽ���� 0���ɼ�ģʽ 1���ط�ģʽ
	CButton			m_BtnConfig;			//������ð�ť

};