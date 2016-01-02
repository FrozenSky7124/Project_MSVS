/*
 * client.c
 *
 *  Created on: 2014-3-12
 *      Author: zhujy
 */

#include <stdio.h>
#include <stdlib.h>
#include "pub.h"

int main(int arg, char *args[])
{
	if (arg < 4)//如果参数小于3个，main函数退出
	{
		printf("usage:client host port filename\n");
		return EXIT_FAILURE;
	}

	int iport = atoi(args[2]);//将第二个参数转化为端口号
	if (iport == 0)//如果端口号为0，main函数退出
	{
		printf("port %d is invalid\n", iport);
		return EXIT_FAILURE;
	}

	printf("%s send begin\n", args[3]);
	if (send_work(args[1], iport, args[3]) == 1)//将第一个参数做为IP地址，第二个参数做为端口号，第三个参数做为要发送的文件名传递给send_work函数
		printf("%s send success\n", args[3]);
	else
		printf("%s send fail\n", args[3]);

	return EXIT_SUCCESS;
}

