// Run.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <direct.h>
#include <string.h>

#define WndClassName "GEMAINWINDOWCLASS"
#define WndTitleName "�������µ�" //ģ������ ���ڱ��⺬��ָ���ִ�����
//#define WndTitleName "foobar2000 v1.3.9"
#define DLLName "Demo02_ReadSqlite.dll"

char* m_char;
wchar_t* m_wchar;

//��̬�ͷ��ڴ�
void Release()
{
	if (m_char)
	{
		delete m_char;
		m_char = NULL;
	}
	if (m_wchar)
	{
		delete m_wchar;
		m_wchar = NULL;
	}
}

//wchar_t TO char
char* WcharToChar(const wchar_t* wp)
{
	Release();
	int len = WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

//char TO wchar_t
wchar_t* CharToWchar(const char* c)
{
	Release();
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

//EnumWindows�ص�����
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	wchar_t wndTitle[256];
	memset(&wndTitle, 0, sizeof(wndTitle));
	GetWindowText(hwnd, wndTitle, 256);
	//printf("%s\n", WcharToChar(wndTitle));
	if (strstr(WcharToChar(wndTitle), WndTitleName) != NULL)
	{
		*((HWND*)lParam) = hwnd;
		return FALSE;
	}
	return TRUE;
}

//��������Ȩ��
bool enableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
						  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) {
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
		CloseHandle(hToken);
		return false;
	}

	return true;
}

void Inject(HWND wndHandle, char* dllPath)
{
	DWORD pid = 0;
	HANDLE processHandle = NULL;
	HANDLE threadHandle = NULL;
	LPVOID memAddress = NULL;
	DWORD writeSize = 0;

	if (wndHandle == NULL)
	{
		printf("�޷���λ���ھ��\n");
	}
	else
	{
		printf("���ھ�� %0#10x\n", (unsigned int)wndHandle);
		printf("���ӿ�·�� %s\n", dllPath);
		//��ȡ����PID
		GetWindowThreadProcessId(wndHandle, &pid);
		if (pid != NULL)
		{
			//��������Ȩ��
			enableDebugPriv();
			//��ȡ���̾��
			processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
			if (processHandle != NULL)
			{
				//�ڽ����ڷ����ڴ�ռ�
				memAddress = VirtualAllocEx(processHandle, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
				if (memAddress != NULL)
				{
					if (WriteProcessMemory(processHandle, memAddress, dllPath, strlen(dllPath) + 1, &writeSize))
					{
						printf("д��DLL·���ɹ���");
					}
					if (writeSize >= strlen(dllPath))
					{
						//LoadLibraryA(const char*);
						//LoardLibraryW(const wchar_t*);
						threadHandle = CreateRemoteThread(processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, memAddress, NULL, NULL);
						if (threadHandle == NULL)
						{
							printf("Զ�˽��̴���ʧ�ܣ�");
						}
						else
						{
							//�ȴ�Զ�˽���ִ��
							WaitForSingleObject(threadHandle, 0x0000FFFF);
							//�ͷ��ڽ����ڷ�����ڴ�ռ�
							CloseHandle(threadHandle);
							VirtualFreeEx(processHandle, memAddress, 512, MEM_DECOMMIT);
						}
					}
					else
					{
						printf("д��DLL·��ʧ�ܣ�");
					}
				}
				else
				{
					printf("�����ڷ���ռ�ʧ�ܣ�");
				}
			}
			else
			{
				printf("��ȡ���̾��ʧ�ܣ� ErrorCode:%d", GetLastError());
			}
		}
	}
	if (processHandle != NULL)
	{
		CloseHandle(processHandle);
	}
}

int main()
{
	HWND wndHandle = NULL;
	//DLL����·��
	char dllPath[256];
	char *cwd = NULL;
	cwd = _getcwd(NULL, 0);
	memset(&dllPath, 0, sizeof(dllPath));
	sprintf_s(dllPath, "%s\\%s\0", cwd, DLLName);
	//printf("DLL Path : %s\n", dllPath);
	free(cwd);
	//ʹ��FindWindow����ֱ�ӻ�ȡ׼ȷĿ�괰��
	//wndHandle = FindWindow(_T(WndTitleName), _T(WndClassName));
	//�������д���Ѱ��Ŀ�괰��
	EnumWindows(EnumWindowsProc, (LPARAM)&wndHandle);
	//ע��Ŀ��DLL
	Inject(wndHandle, dllPath);
	system("pause");
	return 0;
}