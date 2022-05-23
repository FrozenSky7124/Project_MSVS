#pragma once


// FS_Button

class FS_Button : public CButton
{
	DECLARE_DYNAMIC(FS_Button)

public:
	FS_Button();
	virtual ~FS_Button();
	//���ð�ť����ͼƬ·���������ڱ���ͼƬ·��
	void SetImagePath(CString strImgPath, CString strParentImgPath);
	//��ʼ����ť����Ҫ�ǵ�����ť��λ�ã�����͸��ɫ
	bool InitMyButton(int nX/*���Ͻ�X����*/, int nY/*���Ͻ�Y����*/, int nW/*ͼ���*/, int nH/*ͼ���*/, bool bIsPng/*�Ƿ���PNGͼƬ*/);
	//�Ի��ƺ���
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	DECLARE_MESSAGE_MAP()

public:
	CImage m_imgButton; //��ť����ͼ��
	CString m_strImgPath; //��ťpng·�����������㣬����������3��״̬
	CString m_strImgParentPath; //�����ڱ���ͼƬ����·����͸��png��Ҫʹ��

	BOOL m_bMouseMove; //���״ָ̬ʾ
};


