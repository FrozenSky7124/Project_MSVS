
// Demo13_UDP.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemo13_UDPApp: 
// �йش����ʵ�֣������ Demo13_UDP.cpp
//

class CDemo13_UDPApp : public CWinApp
{
public:
	CDemo13_UDPApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDemo13_UDPApp theApp;