#include <stdio.h>
#include <stdlib.h>
#include "pub.h"

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		printf("Please enter: Port.\n");
		return EXIT_FAILURE;
	}

	int port = atoi(argv[1]);
	if (port == 0)
	{
		printf("Port %d is Invalid.\n", port);
		return EXIT_FAILURE;
	}

	printf("Receive File ...\n");
	if (recv_work(port) == 1)
		printf("Receive Success.\n");
	else
		printf("Receive Failure.\n");

	return 0;
}