// InjectCMD.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <direct.h>
#include <string.h>

#define WndClassName "TL_YJ_WINDOW"
#define WndTitleName "多玩天刀"
#define DLLName "Demo01_MFCInject.dll"

char* m_char;
wchar_t* m_wchar;

//动态释放内存
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

void Inject()
{
	HWND gameHandle = NULL;
	DWORD pid = 0;
	HANDLE processHandle = NULL;
	HANDLE threadHandle = NULL;
	LPVOID memAddress = NULL;
	DWORD writeSize = 0;
	char *cwd = NULL;
	cwd = _getcwd(NULL, 0);
	char dllPath[256];
	memset(&dllPath, 0, sizeof(dllPath));
	sprintf(dllPath, "%s\\%s\0", cwd, DLLName);
	printf("动态链接库路径：%s\n", dllPath);

	EnumWindows(EnumWindowsProc, (LPARAM)&gameHandle);

	if (gameHandle == NULL)
	{
		printf("无法定位窗口句柄\n");
	}
	else
	{
		printf("窗口句柄 %0#10x\n", gameHandle);
		//获取进程PID
		GetWindowThreadProcessId(gameHandle, &pid);
		if (pid != NULL)
		{
			//获取进程句柄
			processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
			if (processHandle != NULL)
			{
				//在进程内分配内存空间
				memAddress = VirtualAllocEx(processHandle, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
				if (memAddress != NULL)
				{
					if(WriteProcessMemory(processHandle, memAddress, dllPath, strlen(dllPath)+1 , &writeSize))
					{
						printf("写入DLL路径成功！");
					}
					if (writeSize >= strlen(dllPath))
					{
						//LoadLibraryA(const char*);
						//LoardLibraryW(const wchar_t*);
						threadHandle = CreateRemoteThread(processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, memAddress, NULL, NULL);
						if (threadHandle == NULL)
						{
							printf("远端进程创建失败！");
						}
						else
						{
							//等待远端进程执行
							WaitForSingleObject(threadHandle, 0x0000FFFF);
							//释放在进程内分配的内存空间
							CloseHandle(threadHandle);
							VirtualFreeEx(processHandle, memAddress, 512, MEM_DECOMMIT);
						}
					}
					else
					{
						printf("写入DLL路径失败！");
					}
				}
				else
				{
					printf("进程内分配空间失败！");
				}
			}
			else
			{
				printf("获取进程句柄失败！");
			}
		}
	}
	free(cwd);
	if (processHandle != NULL)
	{
		CloseHandle(processHandle);
	}
}

int main()
{
	Inject();

	system("pause");
    return 0;
}

