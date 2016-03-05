// Demo02_ReadSqlite.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Demo02_ReadSqlite.h"
#include "MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CDemo02_ReadSqliteApp

BEGIN_MESSAGE_MAP(CDemo02_ReadSqliteApp, CWinApp)
END_MESSAGE_MAP()


// CDemo02_ReadSqliteApp 构造

CDemo02_ReadSqliteApp::CDemo02_ReadSqliteApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDemo02_ReadSqliteApp 对象

CDemo02_ReadSqliteApp theApp;


// CDemo02_ReadSqliteApp 初始化
MainDialog *pMainDialog;

DWORD WINAPI showMainDialog(LPARAM lpData)
{
	pMainDialog = new MainDialog;
	//显示窗体
	pMainDialog->DoModal();
	//卸载DLL
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
