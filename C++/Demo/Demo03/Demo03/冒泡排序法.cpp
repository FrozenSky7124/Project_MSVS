#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void main()
{
	int num[10];
	time_t tms;
	srand((unsigned int)time(&tms)); //以时间为随机数种子生成随机数
	printf("原始序列\n");
	for (int i = 0; i < 10; i++)
	{
		num[i] = rand() % 100 + 100; //随机数范围100~199
		printf("%-5d", num[i]);
	}
	printf("\n排序中...\n");
	for (int i = 0;i < 10 - 1;i++)
	{
		for (int j = 0;j < 10 - 1 - i;j++)
		{
			if (num[j] > num[j + 1])
			{
				int temp = num[j];
				num[j] = num[j + 1];
				num[j + 1] = temp;
			}
		}
		for (int i = 0;i < 10;i++)
		{
			printf("%-5d", num[i]);
		}
		printf("\n");
	}
	printf("排序完成\n");
	for (int i = 0;i < 10;i++)
	{
		printf("%-5d", num[i]);
	}
	printf("\n");
	getchar();
}