
// Demo_ProcessMon.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemo_PMonApp: 
// �йش����ʵ�֣������ Demo_ProcessMon.cpp
//

class CDemo_PMonApp : public CWinApp
{
public:
	CDemo_PMonApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDemo_PMonApp theApp;