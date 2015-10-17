#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

void mo(int num)
{
	if (num == 1)
	{
		printf("%d", 1);
	}
	else
	{
		mo(num / 2);
		printf("%d", num % 2);
	}
}
void main()
{
	while (1)
	{
		int num = 0;
		scanf("%d", &num);
		printf("%d\n", num);
		mo(num);
		printf("\n");
	}
}