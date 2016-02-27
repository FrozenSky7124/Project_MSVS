// Demo01_MFCInject.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Demo01_MFCInject.h"
#include "MainDialogWnd.h"

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

// CDemo01_MFCInjectApp

BEGIN_MESSAGE_MAP(CDemo01_MFCInjectApp, CWinApp)
END_MESSAGE_MAP()


// CDemo01_MFCInjectApp ����

CDemo01_MFCInjectApp::CDemo01_MFCInjectApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDemo01_MFCInjectApp ����

CDemo01_MFCInjectApp theApp;


// CDemo01_MFCInjectApp ��ʼ��
CMainDialogWnd *pMainDialog;
DWORD WINAPI showDialog(LPARAM lpData)
{
	pMainDialog = new CMainDialogWnd;
	//��ʾ����
	pMainDialog->DoModal();
	//ж��DLL
	delete pMainDialog;
	FreeLibraryAndExitThread(theApp.m_hInstance, 1);
	return true;
}

BOOL CDemo01_MFCInjectApp::InitInstance()
{
	CWinApp::InitInstance();
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)showDialog, NULL, NULL, NULL);
	return TRUE;
}
