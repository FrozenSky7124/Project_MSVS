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

	while ((i <= mid) && (j <= high)) //将小的放到目的地中
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

	while (i <= mid)  //若还剩几个尾部元素
	{
		des[k++] = src[i++];
	}

	while (j <= high) //若还剩几个尾部元素
	{
		des[k++] = src[j++];
	}
}

//每次分为两路 当只剩下一个元素时，就不需要在划分
void MSort(int src[], int des[], int low, int high, int max)
{
	if (low == high) //只有一个元素，不需要归并，结果赋给des[low]
	{
		des[low] = src[low];
	}
	else //如果多个元素，进行两路划分
	{
		int mid = (low + high) / 2;
		int* space = (int*)malloc(sizeof(int) * max);

		//递归进行两路，两路的划分 
		//当剩下一个元素的时，递归划分结束，然后开始merge归并操作
		if (space != NULL)
		{
			MSort(src, space, low, mid, max);
			MSort(src, space, mid + 1, high, max);
			Merge(space, des, low, mid, high); //调用归并函数进行归并
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
