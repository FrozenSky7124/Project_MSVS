#include <stdio.h>
#include <stdlib.h>

void printArray(int array[], int len)
{
	int i = 0;
	//printf("Array Before : ");
	for (i = 0; i < len ; i++)
	{
		printf("%d ", array[i]);
	}
	printf("\n");
}

void swap(int array[], int x, int y)
{
	int tmp = array[x];
	array[x] = array[y];
	array[y] = tmp;
}

int partition(int array[], int low, int high)
{
	int pv = array[low];
	while (low < high)
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

		/*
		优化算法
		*/
		int tmpPivotPlus = pivot;
		int tmpPivotReduce = pivot;
		int plusN = 0;
		int reduceN = 0;
		while (array[tmpPivotReduce] == array[tmpPivotReduce - 1])
		{
			reduceN++;
			tmpPivotReduce --;
		}
		while (array[tmpPivotPlus] == array[tmpPivotPlus + 1])
		{
			plusN++;
			tmpPivotPlus ++;
		}

		QSort(array, low, pivot - reduceN - 1);
		QSort(array, pivot + plusN + 1, high);
	}
}

void QuickSort(int array[], int len)
{
	QSort(array, 0, len - 1);
}

int main(int argc, char const *argv[])
{
	int arrayDemo[] =
	{
		4, 9, 3, 2, 7, 6, 6, 8, 0, 11, 4, 10, 15, 7
	};
	int arrayLen = sizeof(arrayDemo) / sizeof(int);
	printArray(arrayDemo, arrayLen);
	QuickSort(arrayDemo, arrayLen);
	printArray(arrayDemo, arrayLen);
	return 0;
}