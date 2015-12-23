/*
* 编写一个孤儿进程，这个孤儿进程可以同时创建n个僵死进程。
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{

	pid_t pid;
	int n = atoi(argv[1]);
	if (argc < 1)
	{
		return -1;
	}
	pid = fork();
	if (pid < 0)
	{
		return -1;
	}
	if (pid > 0)
	{
		exit(0);
	}
	if (pid == 0)
	{
		printf("%d\n", getpid());
		printf("%d\n", n);
		static int _Total = 0;
		while (_Total < n)
		{
			int pidi = fork();
			if (pidi == 0)
			{
				return 0;
			}
			if (pidi > 0)
			{
				_Total++;
				printf("Total:%d   %d\n", _Total, getpid());
				//sleep(1);
			}
		}
	}

	sleep(60);

	return 0;
}
