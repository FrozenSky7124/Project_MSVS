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