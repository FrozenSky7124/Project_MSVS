
// Demo17_GDI.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CDemo17_GDIApp: 
// 有关此类的实现，请参阅 Demo17_GDI.cpp
//

class CDemo17_GDIApp : public CWinApp
{
public:
	CDemo17_GDIApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CDemo17_GDIApp theApp;