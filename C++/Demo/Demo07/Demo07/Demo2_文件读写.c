#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main7_3()
{
	FILE *fp = NULL;
	char readc;
	//char *filename = "\config.ini";
	char *filename = "D:/Project_MSVS/C++/Demo/Demo07/Debug/config.ini";
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("Function: fopen() Error!\n");
		return;
	}
	while (!feof(fp))
	{
		readc = fgetc(fp);
		printf("%c", readc);
	}
	
	fclose(fp);
	system("pause");
}