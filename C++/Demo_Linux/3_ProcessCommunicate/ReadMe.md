Linux进程间通信
===============

**无名管道**

```c
	
	#include <unistd.h>

	int pipe(int pipefd[2]);

```

**有名管道**

```c
	
	//创建有名管道

	//mode_t mode : 用户组读写权限

	int mkfifo(const char *pathname, mode_t mode);

	//删除有名管道

	int unlink(const char *pathname);

```

**共享内存**

```c
	
	//ket_t key : 共享内存标识符，可由系统分配 IPC_PRIVATE

	//int shmflg : 共享内存读写权限
	
	int shmget(key_t key, size_t size, int shmflg);

```

**共享内存链接**

```c
	
	 void *shmat(int shmid, const void *shmaddr, int shmflg);

	 int shmdt(const void *shmaddr);
	 
```

**Linux共享库编译&链接**

-fPIC 使gcc编译时，产生与位置无关的代码

-shared 使gcc链接时，生成共享库文件

```

	gcc -fPIC -o test.o -c test.c

	gcc -shared -o libtest.so -c test.o

```

-L gcc链接时.o文件位置

-l gcc链接时.so共享库位置

```

	gcc -L[PATH] -l[PATH] -o test test.o

```

**C/C++混合编程**

```c

	#ifdef __cplusplus

	extern "C"
	
	{

	//如果使用G++编译，将执行extern "C"

	}
	
	#endif

```