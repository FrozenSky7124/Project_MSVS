// InjectCMD.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <direct.h>
#include <string.h>

#define WndClassName "TL_YJ_WINDOW"
#define WndTitleName "�����쵶"
#define DLLName "Demo01_MFCInject.dll"

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
	printf("��̬���ӿ�·����%s\n", dllPath);

	EnumWindows(EnumWindowsProc, (LPARAM)&gameHandle);

	if (gameHandle == NULL)
	{
		printf("�޷���λ���ھ��\n");
	}
	else
	{
		printf("���ھ�� %0#10x\n", gameHandle);
		//��ȡ����PID
		GetWindowThreadProcessId(gameHandle, &pid);
		if (pid != NULL)
		{
			//��ȡ���̾��
			processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
			if (processHandle != NULL)
			{
				//�ڽ����ڷ����ڴ�ռ�
				memAddress = VirtualAllocEx(processHandle, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
				if (memAddress != NULL)
				{
					if(WriteProcessMemory(processHandle, memAddress, dllPath, strlen(dllPath)+1 , &writeSize))
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
				printf("��ȡ���̾��ʧ�ܣ�");
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

