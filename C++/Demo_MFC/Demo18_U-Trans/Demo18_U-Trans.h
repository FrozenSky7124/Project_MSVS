
// Demo18_U-Trans.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// UTransApp: 
// �йش����ʵ�֣������ Demo18_U-Trans.cpp
//

class UTransApp : public CWinApp
{
public:
	UTransApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern UTransApp theApp;