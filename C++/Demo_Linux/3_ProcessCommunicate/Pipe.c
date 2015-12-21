#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int pd[2];//管道描述符
	char buf[100];//管道通信缓存
	int len;

	pipe(pd);//创建管道
	memset(buf, 0, sizeof(buf));
	pid_t pid;
	int status;
	pid = fork();
	if (pid == 0)
	{
		close(pd[1]);//关闭子进程写管道
		while ((len = read(pd[0], buf, sizeof(buf))) > 0)
		{
			write(STDOUT_FILENO, buf, len);

		}
		close(pd[0]);
	}
	else
	{
		close(pd[0]);//关闭父进程读管道
		strcpy(buf, "Hello! FrozenSky!\n");
		write(pd[1], buf, sizeof(buf));
		close(pd[1]);
		wait(&status);
	}
	return 0;
}