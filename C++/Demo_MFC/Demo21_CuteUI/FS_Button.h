#pragma once


// FS_Button

class FS_Button : public CButton
{
	DECLARE_DYNAMIC(FS_Button)

public:
	FS_Button();
	virtual ~FS_Button();
	//设置按钮背景图片路径，父窗口背景图片路径
	void SetBtnImage(CString strImgPath, CString strParentImgPath);
	void SetBtnImage(CString strImgPath, int R, int G, int B); //重载：设置背景颜色取代父窗口背景图片
	//初始化按钮，主要是调整按钮的位置，处理透明色
	bool InitButton(int nX/*左上角X坐标*/, int nY/*左上角Y坐标*/, int nW/*图像宽*/, int nH/*图像高*/, bool bIsPng/*是否是PNG图片*/);
	//自绘制函数
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	CImage m_imgButton; //按钮图像
	CString m_strImgPath; //按钮图像路径，包括焦点，正常，按下3个状态
	CString m_strImgParentPath; //父窗口背景图片背景路径，透明png需要使用
	int m_iBkColorR;
	int m_iBkColorG;
	int m_iBkColorB;
	int m_iBtnImageState; //按钮渲染方式 0=背景颜色 1=背景图片

	BOOL m_bMouseHover; //鼠标悬停状态指示
};


