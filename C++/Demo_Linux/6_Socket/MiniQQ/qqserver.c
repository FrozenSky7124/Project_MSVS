/*
 * qqserver.c
 *
 *  Created on: 2014年4月20日
 *      Author: zhujy
 */

#include "pub.h"

int main(int arg, char *args[])
{
	if (arg < 2)//如果没有参数，main函数退出
	{
		printf("usage:qqserver port\n");
		return EXIT_SUCCESS;
	}

	int iport = atoi(args[1]);//将第一个参数转化为端口号
	if (iport == 0)
	{
		printf("port %d is invalid\n", iport);
		return EXIT_SUCCESS;
	}

	printf("qqserver is begin\n");
	signal1(SIGINT, catch_Signal);//捕捉SIGINT消息
	init_socket_client();//初始化int socket_client[2]数组
	int st = socket_create(iport);//建立server端socket，在iport指定的端口号上listen
	if (st == 0)//如果建立失败，main函数退出
		return EXIT_FAILURE;
	socket_accept(st);//server端socket开始accept
	close(st);
	printf("qqserver is end\n");
	return EXIT_SUCCESS;
}

