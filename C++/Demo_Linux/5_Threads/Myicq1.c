#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//ssize_t read(int fd, void *buf, size_t count);
//ssize_t write(int fd, const void *buf, size_t count);
//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
void *listen(void *arg)
{
	int len = 0;
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int fd = open("/home/frozensky/LinuxDemo/fifo1", O_RDONLY);
	if (fd < 0)
	{
		printf("Error in open fifo1. %s\n", strerror(errno));
		exit(0);
	}
	while ((len = read(fd, buf, sizeof(buf))) > 0)
	{
		printf("%s\n", buf );
	}
	close(fd);
	return NULL;
}

void *send(void *arg)
{
	char *name = (char *)arg;
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int fd = open("/home/frozensky/LinuxDemo/fifo2", O_WRONLY);
	if (fd < 0)
	{
		printf("Error in open fifo2. %s\n", strerror(errno));
		exit(0);
	}
	gets(buf);
	while (strcmp(buf, "quit") != 0)
	{
		char sendbuf[1024];
		memset(sendbuf, 0, sizeof(sendbuf));
		strcat(sendbuf, name);
		strcat(sendbuf, ": ");
		strcat(sendbuf, buf);
		write(fd, sendbuf, sizeof(buf));
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(buf, 0, sizeof(buf));
		gets(buf);
	}
	close(fd);
	exit(0);
	return NULL;
}

int main(int argc, char const *argv[])
{
	printf("Myicq1 Online!\n");
	//printf("%s\n", argv[1]);
	if (argc < 2)
	{
		printf("Please enter the name.\n");
		return -1;
	}
	pthread_t thrd_listen;
	pthread_t thrd_send;
	if (pthread_create(&thrd_listen, NULL, listen, NULL) != 0)
	{
		printf("Error in pthread_create. %s\n", strerror(errno));
		return -1;
	}
	if (pthread_create(&thrd_send, NULL, send, (void *)argv[1]) != 0)
	{
		printf("Error in pthread_create. %s\n", strerror(errno));
		return -1;
	}
	pthread_join(thrd_listen, NULL);
	pthread_join(thrd_send, NULL);
	printf("Myicq1 Offline!\n");
	return 0;
}