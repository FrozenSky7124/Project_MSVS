
// Demo20_CuteDlg01.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CuteDlg01App:
// See Demo20_CuteDlg01.cpp for the implementation of this class
//

class CuteDlg01App : public CWinApp
{
public:
	CuteDlg01App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

	ULONG_PTR m_pGdiToken;
};

extern CuteDlg01App theApp;