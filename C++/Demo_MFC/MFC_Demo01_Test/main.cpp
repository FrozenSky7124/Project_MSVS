#include <Windows.h>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//使用SendMessage()函数来实现鼠标点击的坐标换算方法
	//LPARAM lparam = MAKELPARAM(50, 90);
	 
	HWND hWnd = FindWindow(NULL, "Calculator");
	int ret = MessageBox(hWnd, "This is a Demo : MFC_Demo01", "Demo01", MB_YESNO | MB_ICONINFORMATION);
	switch (ret)
	{
	case 0:
		//RunError
		break;
	case IDNO:
		MessageBox(hWnd, "You Press NO.", "Demo01", 0);
		break;
	case IDYES:
		MessageBox(hWnd, "You Press YES.", "Demo01", 0);
		break;
	default:
		break;
	}
	return 0;
}