#pragma once

#include "Dib_Ex.h"

struct SubWinParam //Ԥ���Ӵ�����ز������ݽṹ
{
	int iMainWinPosX;
	int iMainWinPosY;
	int iSubWidth;
	int iSubHeight;
	int iMouseX;
	int iMouseY;
	CDib* pOriginImage;
};

// SubWinDlg �Ի���

class SubWinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SubWinDlg)

public:
	SubWinDlg(CWnd* pParent = NULL);   // ��׼���캯��
	SubWinDlg(SubWinParam* pSubWinParam, CWnd* pParent = NULL); // ���ع��캯��
	void ShowViewImage();
	virtual ~SubWinDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUBWIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	SubWinParam* m_pSubWinParam;
	CDC* m_pCDC;
	CRect m_mainRect;
	CRect m_viewRect;
	CDib m_viewImage;
};
