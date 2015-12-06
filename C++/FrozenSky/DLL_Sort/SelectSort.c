#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void printArray02(int array[], int len)
{
	int i = 0;

	for (i = 0; i<len; i++)
	{
		printf("%d ", array[i]);
	}

	printf("\n");
}
void InertionSort(int array[], int len) // O(n*n)
{
	int i = 0;
	int j = 0;
	int k = -1;
	int temp = -1;

	//{12, 5, 433, 253, 216, 7};
	for (i = 1; i<len; i++)
	{
		k = i; //������λ��
		temp = array[k];

		for (j = i - 1; (j >= 0) && (array[j]>temp); j--)
		{
			array[j + 1] = array[j]; //Ԫ�غ���
			k = j; //k��Ҫ�����λ��
		}

		array[k] = temp;//Ԫ�ز���
	}
}

int main()
{
	int array[] = { 12, 5, 433, 253, 216, 7 };
	int len = sizeof(array) / sizeof(*array);

	printArray02(array, len);
	InertionSort(array, len);
	printArray02(array, len);
	system("pause");
	return 0;
}