// Demo02_ReadSqlite.h : Demo02_ReadSqlite DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemo02_ReadSqliteApp
// �йش���ʵ�ֵ���Ϣ������� Demo02_ReadSqlite.cpp
//

class CDemo02_ReadSqliteApp : public CWinApp
{
public:
	CDemo02_ReadSqliteApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

private:
	
};
