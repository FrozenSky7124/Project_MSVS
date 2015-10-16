#include<stdio.h>
#include<stdlib.h>

void main()
{
	//system("explorer.exe C:");
	system("tasklist");
	system("taskkill /f /im QQ.EXE");
	printf("Running...");
	getchar();
}