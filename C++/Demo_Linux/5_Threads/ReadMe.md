Linux多线程
===========

**多线程链接**

程序链接时需要链接libpthread.so库: -lpthread

线程拥有统一的代码区和堆区，但拥有不同的栈区。

**多线程创建**

```c
	#include <pthread.h>
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

	pthread_t thrd1, thrd2;
	int t1 = 1;
	int t2 = 2;
	if ((pthread_create(&thrd1, NULL, func, &t1)) != 0)
	{
		return -1;
	}
	if ((pthread_create(&thrd2, NULL, func, &t2)) != 0)
	{
		return -1;
	}
```

**线程终止**

任意线程调用exit函数，整个进程将被摧毁。

如果信号默认动作是终止进程，当该信号送达任意线程时，整个进程将被摧毁

**单个线程的退出**

线程从启动函数中返回

线程被进程中其他线程取消

线程调用pthread_exit函数

**线程挂起**

```c
	int pthread_join(pthread_t thread, void **retval);
	//挂起，等待线程Thread退出
```

**线程分离**

```c
	int pthread_detach(pthread_t thread);
```

**线程取消**

```c
	int pthread_cancel(pthread_t thread);
```

**线程同步**

```c
	//初始化MUTEX互斥锁
	//原子操作
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	//在进程调用的Function起始时加锁
	pthread_mutex_lock(&mutex);

	//在进程调用的Function返回前解锁
	pthread_mutex_unlock(&mutex);

	//判断锁
	pthread_mutex_unlock(&mutex);
```