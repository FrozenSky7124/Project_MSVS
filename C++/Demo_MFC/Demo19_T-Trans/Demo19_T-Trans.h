
// Demo19_T-Trans.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// TTransApp: 
// �йش����ʵ�֣������ Demo19_T-Trans.cpp
//

class TTransApp : public CWinApp
{
public:
	TTransApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern TTransApp theApp;