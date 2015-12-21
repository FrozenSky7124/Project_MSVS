#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/klog.h>
#include <syslog.h>

int main(int argc, char const *argv[])
{
	pid_t pid = fork();
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
		setsid();//与控制台终端断开关联
		chdir("/");
		umask(0);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}

	while (1)
	{
		sleep(1);
		syslog(LOG_INFO, "FrozenSky's Daemon Process is Online.");
	}
	return 0;
}