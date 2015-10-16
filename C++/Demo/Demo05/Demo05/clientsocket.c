#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cltsocket01.h"
#include "cltsocket02.h"

//客户端初始化
int cltSocket_init(void **handle)
{
	return 0;
}

//客户端发送报文
int cltSocket_senddata(void *handle, unsigned char *buf, int buflen)
{
	return 0;
}

//客户端接收报文
int cltSocket_resvdata(void *handle, unsigned char *buf, int *buflen)
{
	return 0;
}

//客户端释放
int cltSocket_destroy(void *handle)
{
	return 0;
}
/*
void main()
{
	printf("Hello!\n");
	system("pause");
}
*/