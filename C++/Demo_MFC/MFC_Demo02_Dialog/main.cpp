#include <Windows.h>
#include <atlstr.h>
#include "resource.h"

//对话框消息回调函数
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
		{
			//响应菜单-退出
			if (LOWORD(wParam) == ID_MEXIT)
			{
				EndDialog(hwndDlg, ID_MEXIT);
				break;
			}
			if (LOWORD(wParam) == ID_MAbout)
			{
				MessageBox(hwndDlg, _T("About Me ?!"), _T("About"), MB_ICONINFORMATION);
				break;
			}
			if (LOWORD(wParam) == ID_40003)
			{
				DialogBox(0, (LPCTSTR)IDD_MAIN_DLG, hwndDlg, DialogProc);
				break;
			}
			//响应Exit按钮点击事件
			if (LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hwndDlg, IDCANCEL);
				break;
			}
			//响应Calculate按钮点击事件
			if (LOWORD(wParam) == IDOK)
			{
				int x1 = GetDlgItemInt(hwndDlg, IDC_EDIT_X1, NULL, TRUE);
				int x2 = GetDlgItemInt(hwndDlg, IDC_EDIT_X2, NULL, TRUE);
				int x3 = GetDlgItemInt(hwndDlg, IDC_EDIT_X3, NULL, TRUE);
				
				int y1 = GetDlgItemInt(hwndDlg, IDC_EDIT_Y1, NULL, TRUE);
				int y2 = GetDlgItemInt(hwndDlg, IDC_EDIT_Y2, NULL, TRUE);
				int y3 = GetDlgItemInt(hwndDlg, IDC_EDIT_Y3, NULL, TRUE);

				LPARAM r1 = MAKELPARAM(x1, y1);
				LPARAM r2 = MAKELPARAM(x2, y2);
				LPARAM r3 = MAKELPARAM(x3, y3);

				CString cStr1, cStr2, cStr3;
				cStr1.Format(_T("0x%.8X"), r1);
				cStr2.Format(_T("0x%.8X"), r2);
				cStr3.Format(_T("0x%.8X"), r3);

				SetDlgItemText(hwndDlg, IDC_EDIT_R1, cStr1);
				SetDlgItemText(hwndDlg, IDC_EDIT_R2, cStr2);
				SetDlgItemText(hwndDlg, IDC_EDIT_R3, cStr3);
				break;
			}
			break;
		}
		default:
			break;
	}
	return FALSE;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//调用资源IDD_MAIN_DLG显示对话框
	DialogBox(hInstance, (LPCTSTR)IDD_MAIN_DLG, NULL, DialogProc);
	return 0;
}