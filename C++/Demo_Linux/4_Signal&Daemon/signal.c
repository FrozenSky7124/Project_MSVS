#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void catch_signal(int signal)
{
	switch (signal)
	{
	case SIGINT:
		printf("CATCH SIGINT!\n");
		break;
	default:
		break;
	}
}

int main(int argc, char const *argv[])
{
	signal(SIGINT, catch_signal);
	while (1)
	{
		printf("Hi~ Waiting...\n");
		sleep(1);
	}
	return 0;
}