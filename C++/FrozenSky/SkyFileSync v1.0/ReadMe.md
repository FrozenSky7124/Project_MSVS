**_finddata_t 文件信息结构体**

```cpp
#include <io.h>
struct _finddata_t
{
    unsigned attrib;     //文件属性
    time_t time_create;  //文件创建时间
    time_t time_access;  //文件上一次访问时间
    time_t time_write;   //文件上一次修改时间
    _fsize_t size;  //文件字节数
    char name[_MAX_FNAME]; //文件名
}; 
```

```
文件属性是无符号整数，取值为相应的宏：
_A_ARCH(存档)，_A_SUBDIR(文件夹)，_A_HIDDEN(隐藏)，
_A_SYSTEM(系统)，_A_NORMAL(正常)，_A_RDONLY(只读)。
```
```cpp
//按FileName命名规则匹配当前目录第一个文件
_findfirst(_In_ const char * FileName, _Out_ struct _finddata64i32_t * _FindData); 
//按FileName命名规则匹配当前目录下一个文件
_findnext(_In_ intptr_t _FindHandle, _Out_ struct _finddata64i32_t * _FindData);
//关闭_findfirst返回的文件句柄
_findclose(_In_ intptr_t _FindHandle);
```

**判断文件是否存在**

```cpp
#include <io.h>
_access(char *,int) //判断文件是否存在
//存在 返回0; 不存在 返回-1.
_access(const char *path,int mode)
//mode的值:
//00 是否存在
//02 写权限
//04 读权限
//06 读写权限
```

**创建文件夹**

```cpp
#include <direct.h>
int _mkdir(const char *dirname);
```

**删除文件夹**

```cpp
#include <direct.h>
int _rmdir(const char *dirname);
//返回值0 :删除成功
//返回值-1:删除失败
```

**删除文件**

```cpp
#include <stdio.h>
int remove(const char *filename);
//返回值0 :删除成功
//返回值-1:删除失败
```

