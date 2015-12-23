#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

void *func(void *argv)
{
	printf("     pthread start!\n");
	printf("     pthread end!\n");
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t thrd1;
	if ((pthread_create(&thrd1, NULL, func, NULL)) != 0)
	{
		printf("Error in pthread_create! %s\n", strerror(errno));
		return -1;
	}
	pthread_detach(thrd1);
	sleep(1);
	printf("main exit.\n");
	return 0;
}