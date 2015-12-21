信号与守护进程
=============

**信号捕获**

```c
	
	//官方定义

	typedef void (*sighandler_t)(int);

	sighandler_t signal(int signum, sighandler_t handler);

	//然而，并不需要这么麻烦

	void *signal(int signo, void (*func)(int));

	//signo : 捕获的信号标号 *func : 回调函数指针

```

**信号发送**

```c
	
	int kill(pid_t pid, int sig);

	int raise(int signo);//给自身发信号玩 (⊙﹏⊙)b

	unsigned int alarm(unsigned int second); //定时器，超时则发送SIGALRM信号

```

**增强的信号捕获机制**

```c
	
	int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

	//重写signal函数

	int mySignal(int signo, void (*func)(int))
	{
		struct sigaction act, oact;
		act.sa_handler = func;
		act.sa_flags = 0;
		sigemptyset(&act.sa_mask);
		return sigaction(signo, &act, &oact);
	}

```

守护进程
========

**创建守护进程**

-父进程执行fork()创建子进程后exit()退出

-在子进程中调用setsid()

```c
	pid_t setsid();
```

-使子进程工作目录变更为/

```c
	chdir("/");
```

-使子进程的umask设置为0

```c
	mode_t umask(mode_t mask);
```

-关闭父进程执行时创建的不需要的文件描述符

**守护进程Syslog日志**

守护进程可以使用syslog函数写入系统日志