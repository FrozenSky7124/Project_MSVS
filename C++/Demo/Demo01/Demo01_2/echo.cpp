#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
void main()
{
	char s_char = 'A';
	short num1 = 7;
	int num2 = 7;
	double num3 = 3.1415;
	printf("%x\n", &s_char);
	printf("%x\n", &num1);
	printf("%x\n", &num2);
	printf("%x\n", &num3);

	getchar();
}
void main1()
{
	const int frozensky = 7124;
	int num;
	char str[50];
	scanf("%d", &num);
	sprintf(str, "for /l %%i in (1,1,%d) do @echo China!", num);
	system(str);
	system("pause");
}