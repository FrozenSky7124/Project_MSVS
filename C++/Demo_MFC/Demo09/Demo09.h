
// Demo09.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// QNotepad: 
// �йش����ʵ�֣������ Demo09.cpp
//

class QNotepad : public CWinApp
{
public:
	QNotepad();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern QNotepad theApp;