#include <stdio.h>
#include <stdlib.h>

void printArray(int array[], int len)
{
	int i = 0;

	for (i = 0; i < len; i++)
	{
		printf("%d ", array[i]);
	}

	printf("\n");
}

void swap(int array[], int i, int j)
{
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}

int partition(int array[], int low, int high)
{
	int pv = array[low]; //定义基准值

	while (low < high) //整理数组，根据基准数重排
	{
		while ((low < high) && (array[high] >= pv))
		{
			high--;
		}
		swap(array, low, high);
		while ((low < high) && (array[low] <= pv))
		{
			low++;
		}
		swap(array, low, high);
	}

	return low;
}

void QSort(int array[], int low, int high)
{
	if (low < high)
	{
		int pivot = partition(array, low, high);

		QSort(array, low, pivot - 1);
		QSort(array, pivot + 1, high);
	}
}

void QuickSort(int array[], int len) // O(n*logn)
{
	QSort(array, 0, len - 1);
}

int main()
{
	int array[] = { 12, 5, 433, 7, 253, 216, 7, 101, 777, 5, 433, 216, 1024 };
	int len = sizeof(array) / sizeof(*array);
	printArray(array, len);
	QuickSort(array, len);
	printArray(array, len);
	return 0;
}
