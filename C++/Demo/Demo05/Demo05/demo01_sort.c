#include<stdio.h>
#include<stdlib.h>

void xuanze(int m[])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = i + 1;j < 10; j++)
		{
			if (m[i] > m[j])
			{
				int temp = m[i];
				m[i] = m[j];
				m[j] = temp;
			}
		}
	}
}

void maopao(int m[])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10-1-i; j++)
		{
			if (m[j] > m[j + 1])
			{
				int temp = m[j];
				m[j] = m[j + 1];
				m[j + 1] = temp;
			}
		}
	}
}
/*
void main()
{
	int x[10] = { 9, 4, 7, 3, 2, 6, 1, 10, 5, 8 };
	//xuanze(x);
	maopao(x);
	for (int i = 0;i < 10;i++)
	{
		printf("%d ", x[i]);
	}
	printf("\n");
	system("pause");
}
*/