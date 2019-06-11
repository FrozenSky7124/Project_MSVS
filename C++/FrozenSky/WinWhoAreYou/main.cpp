#include <Windows.h> 
#include <wchar.h> 
#include <Psapi.h> 

#pragma comment(lib, "Psapi.lib") 

int main()
{
	HWND last_fore = NULL;
	wchar_t name[MAX_PATH];
	DWORD size = MAX_PATH;

	while (true)
	{
		HWND fore = GetForegroundWindow();
		if (fore != last_fore)
		{
			last_fore = fore;

			DWORD processid;
			GetWindowThreadProcessId(fore, &processid);

			HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, NULL, processid);


			GetProcessImageFileNameW(process, name, size);

			wprintf(L"%s\n", name);
		}
		Sleep(10);
	}
	return 0;
}