
// Demo10.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemo10App: 
// �йش����ʵ�֣������ Demo10.cpp
//

class CDemo10App : public CWinApp
{
public:
	CDemo10App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDemo10App theApp;