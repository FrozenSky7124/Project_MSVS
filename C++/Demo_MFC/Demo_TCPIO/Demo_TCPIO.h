
// Demo_TCPIO.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// APP_TCPIO:
// See Demo_TCPIO.cpp for the implementation of this class
//

class APP_TCPIO : public CWinApp
{
public:
	APP_TCPIO();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern APP_TCPIO theApp;