#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>

void openBaidu()
{
	ShellExecuteA(0, "open", "http://www.baidu.com", 0, 0, 3);
}

void closeBaidu()
{
	system("taskkill /f /im Maxthon.exe");
}

void clickBaidu()
{
	SetCursorPos(40, 1060);
	Sleep(1000);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void main1()
{
	clickBaidu();
	/*
	while (1)
	{
		openBaidu();
		Sleep(5000);
		closeBaidu();
		Sleep(3000);
	}
	*/
}