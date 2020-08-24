#include "stdio.h"
#include "time.h"
#include <iostream>

// Defining Number of elements in Array
#define N	10000000

// Defining vector addition function for CPU
void cpuAdd(int *h_a, int *h_b, int *h_c)
{
	int tid = 0;
	while (tid < N)
	{
		h_c[tid] = h_a[tid] + h_b[tid];
		tid += 1;
	}
}

int main(void)
{
	int *h_a, *h_b, *h_c;
	h_a = (int *)malloc(sizeof(int) * N);
	h_b = (int *)malloc(sizeof(int) * N);
	h_c = (int *)malloc(sizeof(int) * N);
	// Initializing two arrays for addition
	for (int i = 0; i < N; i++)
	{
		h_a[i] = i + 1;
		h_b[i] = i - 1;
	}
	// Start Time
	clock_t start_h = clock();
	// Calling CPU function for vector addition
	cpuAdd(h_a, h_b, h_c);
	// End Time
	clock_t end_h = clock();
	// Printing Answer
	printf("Vector addition on CPU\n");
	// Execute Time
	double time_h = (double)(end_h - start_h) / CLOCKS_PER_SEC;
	printf("N = %d \n", N);
	printf("Execute time: %f seconds \n", time_h);
	
	for (int i = 9999990; i < N; i++)
	{
	printf("The sum of %d element is %d + %d = %d\n", i, h_a[i], h_b[i], h_c[i]);
	}
	
	free(h_a);
	free(h_b);
	free(h_c);
	system("pause");
	return 0;
}