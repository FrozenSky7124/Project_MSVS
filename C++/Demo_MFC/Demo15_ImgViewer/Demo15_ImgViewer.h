
// Demo15_ImgViewer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// ImgViewerApp: 
// �йش����ʵ�֣������ Demo15_ImgViewer.cpp
//

class ImgViewerApp : public CWinApp
{
public:
	ImgViewerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern ImgViewerApp theApp;