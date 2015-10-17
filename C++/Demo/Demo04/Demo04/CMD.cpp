#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int execmd(char *in, char *out)
{
	char buffer[128] = { 0 };
	FILE *pipe = _popen(in, "r");
	if (!pipe)
	{
		return 0;
	}
	while (!feof(pipe))
	{
		if (fgets(buffer, 128, pipe))
		{
			strcat(out, buffer);
		}
	}
	_pclose(pipe);
	return 1;
}

void main()
{
	char CMDin[20] = {0};
	char CMDout[4096*2] = {0};
	scanf("%s", &CMDin);
	execmd(CMDin, CMDout);
	printf("\n%s", CMDout);
	printf("\n×Ö·û´®³¤¶È=%d", strlen(CMDout));
	system("pause");
}