#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//�������
int myPrintf(char **tmpArray, int iNum)
{
	int i = 0;
	for (i = 0;i < iNum; i++)
	{
		printf("%s\n", tmpArray[i]);
	}
	return 0;
}

//������
int sortMyArray(char **tmpArray, int iNum)
{
	int i = 0;
	int j = 0;
	char tmp[100];

	for (i = 0;i < iNum; i++)
	{
		for (j = i + 1;j < iNum; j++)
		{
			if (strcmp(tmpArray[i], tmpArray[j]) < 0)
			{
				strcpy(tmp, tmpArray[i]);
				strcpy(tmpArray[i], tmpArray[j]);
				strcpy(tmpArray[j], tmp);
			}
		}
	}
	return 0;
}

void main7_1()
{
	int i = 0;
	char **myArray = (char **)malloc(10 * sizeof(char*));
	for (i = 0;i < 10;i++)
	{
		myArray[i] = (char *)malloc(100 * sizeof(char));
		sprintf(myArray[i], "%d%d%d", i, i, i);
	}
	//���
	printf("����ǰ\n");
	myPrintf(myArray, 10);
	//����
	sortMyArray(myArray, 10);
	//���
	printf("�����\n");
	myPrintf(myArray, 10);

	//�ͷ�
	for (i = 0;i < 10;i++)
	{
		free(myArray[i]);
	}
	if (myArray != NULL)
	{
		free(myArray);
	}

	system("pause");
}