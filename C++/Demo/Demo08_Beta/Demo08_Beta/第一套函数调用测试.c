#define _CRT_SECURE_NO_WARNINGS

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "socketclientdll.h"
#include "itcastlog.h"

void main()
{
	int		ret = 0;
	void	*handle = NULL;

	unsigned char buf[128];
	int buflen = 11; /*in*/

	unsigned char outbuf[128];
	int outbufLen; /*in*/

	strcpy(buf, "addddddddddddddddddddddddddddddddaaaaaaaa");

	ret = cltSocketInit(&handle /*out*/);
	if (ret != 0)
	{
		printf("cltSocketInit err:%d", ret);
		goto  End;
	}

	//客户端发报文
	ret = cltSocketSend(handle /*in*/, buf /*in*/, buflen /*in*/);
	if (ret != 0)
	{
		printf("func cltSocketSend() err:%d", ret);
		goto End;
	}


	//客户端收报文
	//底层库提供的是一种机制， 而不是具体的策略
	ret = cltSocketRev(handle /*in*/, outbuf, &outbufLen);
	if (ret != 0)
	{
		printf("func cltSocketSend() err:%d", ret);
		goto End;
	}
	printf("outbuf: %s \n", outbuf);


End:
	//客户端释放资源
	if (handle != NULL)
	{
		cltSocketDestory(handle/*in*/);
	}
	system("pause");
}