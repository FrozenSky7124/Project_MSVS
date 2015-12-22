#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

void writeFifo()
{
	int len = 0;
	int fd = 0;
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	fd = open("/home/frozensky/LinuxDemo/FIFO1", O_WRONLY);
	if (fd < 0)
	{
		printf("Open FIFO Error! %s\n", strerror(errno));
		return ;
	}
	gets(buf);
	while (strcmp(buf, "quit") != 0)
	{
		write(fd, buf, sizeof(buf));
		gets(buf);
	}
	close(fd);
	return;
}

int main(int argc, char const *argv[])
{
	writeFifo();
	return 0;
}