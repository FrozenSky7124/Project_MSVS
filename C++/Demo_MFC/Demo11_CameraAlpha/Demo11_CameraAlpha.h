
// Demo11_CameraAlpha.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// BasicCamApp: 
// �йش����ʵ�֣������ Demo11_CameraAlpha.cpp
//

class BasicCamApp : public CWinApp
{
public:
	BasicCamApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern BasicCamApp theApp;