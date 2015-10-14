#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>

#define WaitTime 5

void press(char p)
{
	keybd_event(p, 0, 0, 0);
	keybd_event(p, 0, 2, 0);
}
void main()
{
	Sleep(WaitTime * 1000);
	SetCursorPos(75, 1064); //鼠标指向小娜图标
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(2000);
	press('X');press('I');press('A');press('O');
	press('B');press('I');press('N');press('G');
	Sleep(2000);
	press(VK_RETURN); //召唤小冰
	Sleep(3000);
	SetCursorPos(150, 1020);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	while (1)
	{
		keybd_event(VK_CONTROL, 0, 0, 0);
		press('V');
		keybd_event(VK_CONTROL, 0, 2, 0);
		press(VK_RETURN);
		Sleep(2000); //冰笑话
	}
}