
// Demo06.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemo06App: 
// �йش����ʵ�֣������ Demo06.cpp
//

class CDemo06App : public CWinApp
{
public:
	CDemo06App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDemo06App theApp;