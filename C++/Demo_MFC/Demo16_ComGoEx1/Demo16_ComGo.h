
// Demo16_ComGo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// ComGoApp: 
// �йش����ʵ�֣������ Demo16_ComGo.cpp
//

class ComGoApp : public CWinApp
{
public:
	ComGoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern ComGoApp theApp;