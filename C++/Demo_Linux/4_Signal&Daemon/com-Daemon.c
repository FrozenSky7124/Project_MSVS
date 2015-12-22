#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

void readFifo()
{
	int fd = 0;
	int len = 0;
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	fd = open("/home/frozensky/LinuxDemo/FIFO1", O_RDONLY);
	if (fd < 0)
	{
		printf("Open FIFO Error! %s\n", strerror(errno));
		return;
	}
	while ((len = read(fd, buf, sizeof(buf))) > 0)
	{
		printf("%s\n", buf );
	}
	close(fd);
	return;
}

void setDaemon()
{
	pid_t pid = fork();
	if (pid < 0)
	{
		printf("Fork Error!\n");
	}
	if (pid == 0)
	{
		setsid();
		/*
		chdir("/");
		umask(0);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		*/
	}
	if (pid > 0)
	{
		printf("Parent Process Exit!\n");
		exit(0);
	}
}

int main(int argc, char const *argv[])
{
	setDaemon();
	//while (1)
	{
		readFifo();
	}
	return 0;
}