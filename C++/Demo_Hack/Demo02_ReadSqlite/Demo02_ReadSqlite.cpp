// Demo02_ReadSqlite.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Demo02_ReadSqlite.h"
#include "MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CDemo02_ReadSqliteApp

BEGIN_MESSAGE_MAP(CDemo02_ReadSqliteApp, CWinApp)
END_MESSAGE_MAP()


// CDemo02_ReadSqliteApp ����

CDemo02_ReadSqliteApp::CDemo02_ReadSqliteApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDemo02_ReadSqliteApp ����

CDemo02_ReadSqliteApp theApp;


// CDemo02_ReadSqliteApp ��ʼ��
MainDialog *pMainDialog;

DWORD WINAPI showMainDialog(LPARAM lpData)
{
	pMainDialog = new MainDialog;
	//��ʾ����
	pMainDialog->DoModal();
	//ж��DLL
	delete pMainDialog;
	FreeLibraryAndExitThread(theApp.m_hInstance, 1);
	return true;
}

BOOL CDemo02_ReadSqliteApp::InitInstance()
{
	CWinApp::InitInstance();
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)showMainDialog, NULL, NULL, NULL);
	return TRUE;
}
