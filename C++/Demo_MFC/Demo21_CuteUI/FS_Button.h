#pragma once


// FS_Button

class FS_Button : public CButton
{
	DECLARE_DYNAMIC(FS_Button)

public:
	FS_Button();
	virtual ~FS_Button();
	//���ð�ť����ͼƬ·���������ڱ���ͼƬ·��
	void SetBtnImage(CString strImgPath, CString strParentImgPath);
	void SetBtnImage(CString strImgPath, int R, int G, int B); //���أ����ñ�����ɫȡ�������ڱ���ͼƬ
	//��ʼ����ť����Ҫ�ǵ�����ť��λ�ã�����͸��ɫ
	bool InitButton(int nX/*���Ͻ�X����*/, int nY/*���Ͻ�Y����*/, int nW/*ͼ���*/, int nH/*ͼ���*/, bool bIsPng/*�Ƿ���PNGͼƬ*/);
	//�Ի��ƺ���
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	CImage m_imgButton; //��ťͼ��
	CString m_strImgPath; //��ťͼ��·�����������㣬����������3��״̬
	CString m_strImgParentPath; //�����ڱ���ͼƬ����·����͸��png��Ҫʹ��
	int m_iBkColorR;
	int m_iBkColorG;
	int m_iBkColorB;
	int m_iBtnImageState; //��ť��Ⱦ��ʽ 0=������ɫ 1=����ͼƬ

	BOOL m_bMouseHover; //�����ͣ״ָ̬ʾ
};


