#pragma once


// fzsButton 对话框

class fzsButton : public CWnd
{
	DECLARE_DYNAMIC(fzsButton)

public:
	fzsButton(void);   // 标准构造函数
	virtual ~fzsButton();

protected:

	DECLARE_MESSAGE_MAP()
};
