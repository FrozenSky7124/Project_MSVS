#include <stdio.h>
#include <stdlib.h>
#include "pub.h"

int main(int argc, char const *argv[])
{
	if (argc < 4)
	{
		printf("Please enter IPHost Port Filename.\n");
		return EXIT_FAILURE;
	}
	int port = atoi(argv[2]);
	if (port == 0)
	{
		printf("Port %d is Invalid.\n", port);
		return EXIT_FAILURE;
	}
	printf("File Transfer Begin.\n");
	if (send_work(argv[1], port, argv[3]) == 1)
	{
		printf("File Transfer Success.\n");
	}
	else
	{
		printf("File Transfer Failure.\n");
	}
	return EXIT_SUCCESS;
}