// Demo01_MFCInject.h : Demo01_MFCInject DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemo01_MFCInjectApp
// �йش���ʵ�ֵ���Ϣ������� Demo01_MFCInject.cpp
//

class CDemo01_MFCInjectApp : public CWinApp
{
public:
	CDemo01_MFCInjectApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
