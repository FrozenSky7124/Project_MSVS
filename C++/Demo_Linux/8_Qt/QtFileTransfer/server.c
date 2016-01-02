/*
 * server.c
 *
 *  Created on: 2014-3-10
 *      Author: zhujy
 */

#include <stdio.h>
#include <stdlib.h>
#include "pub.h"

int main(int arg, char *args[])
{
	if (arg < 2)//如果没有参数，main函数退出
	{
		printf("usage:server port\n");
		return EXIT_FAILURE;
	}

	int iport = atoi(args[1]);//将第一个参数转化为端口号，server端socket要在这个端口号上listen
	if (iport == 0)
	{
		printf("port %d is invalid\n", iport);
		return EXIT_FAILURE;
	}

	printf("recv is begin\n");
	if (recv_work(iport) == 1)//server端socket在port指定的端口上listen，接收来自client发送的文件
		printf("recv success\n");
	else
		printf("recv fail\n");
	return EXIT_SUCCESS;
}
