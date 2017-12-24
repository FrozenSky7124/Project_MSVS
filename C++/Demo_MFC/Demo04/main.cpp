#include <Windows.h>
#include <tchar.h>
#include <AtlBase.h>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//MessageBox(NULL, L"Unicode MessageBox", L"Demo04", MB_OK);

	// MultiByte 多字节字符集
	char *pChar1 = "FAST"; // 46 41 53 54
	char *pChar2 = "长征五号"; // b3 a4 d5 f7 ce e5 ba c5
	char *pChar3 = "長征五號"; // e9 4c d5 f7 ce e5 cc 96

	// UNICODE   宽字节字符集
	wchar_t *pW1 = L"FAST"; // 46 00 41 00 53 00 54 00
	wchar_t *pW2 = L"长征五号"; // 7f 95 81 5f 94 4e f7 53
	wchar_t *pW3 = L"長征五號"; // 77 95 81 5f 94 4e 5f 86

	// TCHAR & _TEXT() 自适应字符集
	TCHAR *pT1 = _T("FAST");

	// Convert Between MultiByte & UNICODE
	wchar_t wcConv[32] = { 0 };
	char cConv[32] = { 0 };
	//memset(cConv, 0, sizeof(cConv));
	MultiByteToWideChar(CP_ACP, 0, pChar2, strlen(pChar2)*sizeof(char), wcConv, sizeof(wcConv));
	WideCharToMultiByte(CP_ACP, 0, pW3, strlen((char*)pW3)*sizeof(wchar_t), cConv, sizeof(cConv), NULL, NULL);

	// T2A W2A
	USES_CONVERSION;
	LPSTR tempChar = W2A(pW2);
	LPWSTR tempWChar = A2W(pChar2);
	//MessageBox(NULL, tempWChar, _T("Demo04"), MB_OK);
	return 0;
}