void printArray05(int array[], int len)
{
	int i = 0;

	for (i = 0; i < len; i++)
	{
		printf("%d ", array[i]);
	}

	printf("\n");
}

void swap5(int array[], int i, int j)
{
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}

int partition(int array[], int low, int high)
{
	int pv = array[low];

	while (low < high)
	{
		while ((low < high) && (array[high] >= pv))
		{
			high--; //比基准大，本来就在右边，所以high前移动
		}
		swap5(array, low, high);
		while ((low < high) && (array[low] <= pv))
		{
			low++;
		}
		swap5(array, low, high);
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
	int array[] = { 12, 5, 433, 253, 216, 7 };
	int len = sizeof(array) / sizeof(*array);
	printArray05(array, len);
	QuickSort(array, len);
	printArray05(array, len);
	system("pause");
	return 0;
}
