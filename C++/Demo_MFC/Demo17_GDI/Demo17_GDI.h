
// Demo17_GDI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemo17_GDIApp: 
// �йش����ʵ�֣������ Demo17_GDI.cpp
//

class CDemo17_GDIApp : public CWinApp
{
public:
	CDemo17_GDIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDemo17_GDIApp theApp;