#pragma once


// MUI_Message �Ի���

class MUI_Message : public CDialogEx
{
	DECLARE_DYNAMIC(MUI_Message)

public:
	MUI_Message(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MUI_Message();

//// �Ի�������
//#ifdef AFX_DESIGN_TIME
//		enum { IDD = IDD_MUI_MESSAGE };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	Gdiplus::Image * m_pImgBk;
	UINT m_iBkWidth;
	UINT m_iBkHeight;
	BLENDFUNCTION m_Blend;
	HDC m_hdcMem;
};
