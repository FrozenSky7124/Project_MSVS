
// UT_Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// UT_ClientApp: 
// �йش����ʵ�֣������ UT_Client.cpp
//

class UT_ClientApp : public CWinApp
{
public:
	UT_ClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern UT_ClientApp theApp;