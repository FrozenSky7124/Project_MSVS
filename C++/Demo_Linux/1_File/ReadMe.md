Linux文件系统
============

##文件描述符
* 每个Linux进程启动时默认打开3个文件:

    Stdin     **STDIN_FILENO**
  
    Stdout    **STDOUT_FILENO**
    
    Stderr    **STDERR_FILENO**

##Linux系统调用
* 打开关闭文件描述符

```c

int open(const char *path, int flags);

int close(int fd);

```

* 获取详细错误信息

```c
    char *strerror(int errno);
```

* 读写用户输入，不回显信息

```c
    char *getpass( const char *prompt);
```

##库函数调用
* 二进制文件读取写入

```c
    size_t fread( void *ptr, size_t size, size_t nmemb, FILE *stream );

    size_t fwrite( void *ptr, size_t size, size_t nmemb, FILE *stream);
```

* 格式化文件读取写入

```c
    int fprintf( FILE *stream, const char *fromat, ...);

    int fscanf( FILE *stream, const char *fromat, ...);
```

* 获取当前目录

```c
    char *getcwd(char *cwdpath, size_t size);
```

* 目录操作

```c
    DIR *opendir( const char *path);
    struct dirent *readdir(DIR *dir);
    int closedir(DIR *dir);
```