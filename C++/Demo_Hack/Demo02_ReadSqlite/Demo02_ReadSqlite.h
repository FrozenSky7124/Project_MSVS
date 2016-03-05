// Demo02_ReadSqlite.h : Demo02_ReadSqlite DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CDemo02_ReadSqliteApp
// 有关此类实现的信息，请参阅 Demo02_ReadSqlite.cpp
//

class CDemo02_ReadSqliteApp : public CWinApp
{
public:
	CDemo02_ReadSqliteApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

private:
	
};
