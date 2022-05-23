#pragma once


// FS_Button

class FS_Button : public CButton
{
	DECLARE_DYNAMIC(FS_Button)

public:
	FS_Button();
	virtual ~FS_Button();
	//设置按钮背景图片路径，父窗口背景图片路径
	void SetImagePath(CString strImgPath, CString strParentImgPath);
	//初始化按钮，主要是调整按钮的位置，处理透明色
	bool InitMyButton(int nX/*左上角X坐标*/, int nY/*左上角Y坐标*/, int nW/*图像宽*/, int nH/*图像高*/, bool bIsPng/*是否是PNG图片*/);
	//自绘制函数
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	DECLARE_MESSAGE_MAP()

public:
	CImage m_imgButton; //按钮背景图像
	CString m_strImgPath; //按钮png路径，包括焦点，正常，按下3个状态
	CString m_strImgParentPath; //父窗口背景图片背景路径，透明png需要使用

	BOOL m_bMouseMove; //鼠标状态指示
};


