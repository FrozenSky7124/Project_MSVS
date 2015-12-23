#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static int count = 0;

void *func(void *argv)
{
	int *a = (int *)argv;
	printf("     Thread%d start!\n", *a);
	int i;
	for (i = 0; i < 10; i++)
	{
		printf("     Thread%d running...%d\n", *a, count);
		sleep(1);
		pthread_mutex_lock(&mutex);
		count++;
		pthread_mutex_unlock(&mutex);
	}
	printf("     Thread%d end.\n", *a);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	printf("Main Start!\n");
	pthread_t thrd1, thrd2;
	int i[2];
	i[0] = 1;
	i[1] = 2;
	if (pthread_create(&thrd1, NULL, func, &i[0]) != 0)
	{
		printf("Error in pthread_create. %s\n", strerror(errno));
		return -1;
	}
	if (pthread_create(&thrd2, NULL, func, &i[1]) != 0)
	{
		printf("Error in pthread_create. %s\n", strerror(errno));
		return -1;
	}
	pthread_join(thrd1, NULL);
	pthread_join(thrd2, NULL);
	printf("Main End!\n");
	return 0;
}