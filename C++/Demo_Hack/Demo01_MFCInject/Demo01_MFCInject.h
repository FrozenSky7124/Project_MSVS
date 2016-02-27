// Demo01_MFCInject.h : Demo01_MFCInject DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CDemo01_MFCInjectApp
// 有关此类实现的信息，请参阅 Demo01_MFCInject.cpp
//

class CDemo01_MFCInjectApp : public CWinApp
{
public:
	CDemo01_MFCInjectApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
