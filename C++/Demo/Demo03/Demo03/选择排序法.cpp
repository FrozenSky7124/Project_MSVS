#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void xuanze()
{
	int num[10];
	time_t tms;
	srand((unsigned int)time(&tms)); //��ʱ��Ϊ������������������
	printf("ԭʼ����\n");
	for (int i = 0; i < 10; i++)
	{
		num[i] = rand() % 100 + 100; //�������Χ100~199
		printf("%-5d",num[i]);
	}
	printf("\n������...\n");
	int MIN = 0;
	for (int i = 0;i < 10 - 1;i++)
	{
		MIN = i;
		for (int j = i + 1;j < 10;j++)
		{
			if (num[MIN] > num[j]) MIN = j;
		}
		int temp = num[i];
		num[i] = num[MIN];
		num[MIN] = temp;
		for (int i = 0;i < 10;i++)
		{
			printf("%-5d", num[i]);
		}
		printf("\n");
	}
	printf("�������\n");
	for (int i = 0;i < 10;i++)
	{
		printf("%-5d", num[i]);
	}
	printf("\n");
	getchar();
}