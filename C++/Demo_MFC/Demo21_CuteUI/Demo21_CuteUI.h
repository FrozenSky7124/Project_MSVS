
// Demo21_CuteUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CuteUIApp:
// See Demo21_CuteUI.cpp for the implementation of this class
//

class CuteUIApp : public CWinApp
{
public:
	CuteUIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CuteUIApp theApp;