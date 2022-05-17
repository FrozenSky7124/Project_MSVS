
// MUI_MessageDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMUI_MessageDemoApp:
// See MUI_MessageDemo.cpp for the implementation of this class
//

class CMUI_MessageDemoApp : public CWinApp
{
public:
	CMUI_MessageDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	ULONG_PTR m_pGdiToken;
};

extern CMUI_MessageDemoApp theApp;