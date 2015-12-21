Linux进程控制
============

**获得进程 PID、PPID**

```c

	#include <sys/types.h>
	
	#include <unistd.h>
	
	pid_t getpid(void);
	
	pid_t getppid(void);

```

**创建自身的子进程**

```c
	
	//返回值=0时，表示处于子进程

	//返回值>0时，表示处于父进程
	
	//返回值<0时，表示创建进程失败
	
	pid_t fork(void);

```

**创建指定进程**

```c
	//召唤指定程序，代替自身代码区，且不会改变原进程PID值等参数

	int execve(const char *filename, char *const argv[], char *const envp[]);

```

**获取子进程状态**

```c

	#include <sys/types.h>

	#include <sys/wait.h>

	//等待子进程退出，将子进程状态数据写入status

	pid_t wait(int *status);

	//通过pid指定特定的子进程

	pid_t waitpid(pid_t pid, int *status, int options);

```

**僵死进程 & 孤儿进程**

僵死进程简而言之就是：子进程退出时，父进程并未对其发出的SIGCHILD信号进行适当处理，导致子进程停留在僵死状态等待其父进程为其收尸，这个状态下的子进程就是僵死进程。

在fork()/execve()过程中，假设子进程结束时父进程仍存在，而父进程fork()之前既没安装SIGCHLD信号处理函数调用waitpid()等待子进程结束，又没有显式忽略该信号，则子进程成为僵死进程，无法正常结束，此时即使是root身份kill -9也不能杀死僵死进程。补救办法是杀死僵尸进程的父进程(僵死进程的父进程必然存在)，僵死进程成为"孤儿进程"，过继给1号进程init，init始终会负责清理僵死进程。

**KILL**

```c

	#include <sys/types.h>

	#include <signal.h>

	int kill(pid_t pid, int sig);

```