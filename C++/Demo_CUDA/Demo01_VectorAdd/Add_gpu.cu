#include "stdio.h"
#include "time.h"
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"

// Defining number of elements in Array
#define N 10000000

// Defining Kernel function for vector addition
__global__ void gpuAdd(int *d_a, int *d_b, int *d_c)
{
	// Getting block index of current kernel
	int tid = blockIdx.x * 1000 + threadIdx.x;
	if (tid < N)
		d_c[tid] = d_a[tid] + d_b[tid];
}

int main(void)
{
	// Defining host arrays
	int *h_a, *h_b, *h_c;
	h_a = (int *)malloc(sizeof(int) * N);
	h_b = (int *)malloc(sizeof(int) * N);
	h_c = (int *)malloc(sizeof(int) * N);
	// Defining device pointers
	int *d_a, *d_b, *d_c;
	// Initializing two arrays for addition
	for (int i = 0; i < N; i++)
	{
		h_a[i] = i + 1;
		h_b[i] = i - 1;
	}
	// Allocate the memory
	cudaMalloc((void **)&d_a, sizeof(int) * N);
	cudaMalloc((void **)&d_b, sizeof(int) * N);
	cudaMalloc((void **)&d_c, sizeof(int) * N);
	// Copy input arrays from host to device memory
	cudaMemcpy(d_a, h_a, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, N * sizeof(int), cudaMemcpyHostToDevice);
	// Start Time
	clock_t start_h = clock();
	// Calling kernels with N blocks and one thread per block, passing device pointers as parameters
	gpuAdd <<<10000, 1000 >>>(d_a, d_b, d_c);
	cudaThreadSynchronize();
	// End Time
	clock_t end_h = clock();
	// Copy result back to host memory from device memory
	cudaMemcpy(h_c, d_c, N * sizeof(int), cudaMemcpyDeviceToHost);
	printf("Vector addition on GPU \n");
	// Execute Time
	double time_h = (double)(end_h - start_h) / CLOCKS_PER_SEC;
	printf("N = %d \n", N);
	printf("Execute time: %f seconds \n", time_h);
	// Printing result on console
	
	for (int i = 9999990; i < N; i++)
	{
		printf("The sum of %d element is %d + %d = %d\n", i, h_a[i], h_b[i], h_c[i]);
	}
	
	// Free up memory
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

	system("pause");
	return 0;
}
