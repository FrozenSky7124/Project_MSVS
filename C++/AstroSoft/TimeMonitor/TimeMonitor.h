
// TimeMonitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// TimeMonitorApp: 
// �йش����ʵ�֣������ TimeMonitor.cpp
//

class TimeMonitorApp : public CWinApp
{
public:
	TimeMonitorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern TimeMonitorApp theApp;