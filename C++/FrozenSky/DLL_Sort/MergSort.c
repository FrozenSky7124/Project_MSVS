#include <stdio.h>
#include <malloc.h>

void printArray06(int array[], int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		printf("%d ", array[i]);
	}
	printf("\n");
}

void swap6(int array[], int i, int j)
{
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}

void Merge(int src[], int des[], int low, int mid, int high)
{
	int i = low;
	int j = mid + 1;
	int k = low;

	while ((i <= mid) && (j <= high)) //��С�ķŵ�Ŀ�ĵ���
	{
		if (src[i] < src[j])
		{
			des[k++] = src[i++];
		}
		else
		{
			des[k++] = src[j++];
		}
	}

	while (i <= mid)  //����ʣ����β��Ԫ��
	{
		des[k++] = src[i++];
	}

	while (j <= high) //����ʣ����β��Ԫ��
	{
		des[k++] = src[j++];
	}
}

//ÿ�η�Ϊ��· ��ֻʣ��һ��Ԫ��ʱ���Ͳ���Ҫ�ڻ���
void MSort(int src[], int des[], int low, int high, int max)
{
	if (low == high) //ֻ��һ��Ԫ�أ�����Ҫ�鲢���������des[low]
	{
		des[low] = src[low];
	}
	else //������Ԫ�أ�������·����
	{
		int mid = (low + high) / 2;
		int* space = (int*)malloc(sizeof(int) * max);

		//�ݹ������·����·�Ļ��� 
		//��ʣ��һ��Ԫ�ص�ʱ���ݹ黮�ֽ�����Ȼ��ʼmerge�鲢����
		if (space != NULL)
		{
			MSort(src, space, low, mid, max);
			MSort(src, space, mid + 1, high, max);
			Merge(space, des, low, mid, high); //���ù鲢�������й鲢
		}

		free(space);
	}
}

void MergeSort(int array[], int len) // O(n*logn)
{
	MSort(array, array, 0, len - 1, len);
}

int main()
{

	//int array[] = {21, 25, 49, 25, 16, 8};
	int array[] = { 21, 25 };
	int len = sizeof(array) / sizeof(*array);
	printArray06(array, len);
	MergeSort(array, len);
	printArray06(array, len);
	system("pause");
	return 0;
}
