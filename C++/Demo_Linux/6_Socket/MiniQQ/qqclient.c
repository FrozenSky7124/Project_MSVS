/*
 * qqclient.c
 *
 *  Created on: 2014年4月20日
 *      Author: zhujy
 */

#include "pub.h"

int socket_connect(const char *hostname, uint16_t port)//client端socket连接到hostname指定的IP地址和port端口号
{
	int st = socket(AF_INET, SOCK_STREAM, 0);//建立一个TCP socket描述符
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(hostname);
	if (connect(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)//连接到指定IP
	{
		printf("connect to %s:%d failed %s\n", hostname, port, strerror(errno));
		return 0;//连接失败，返回0
	} else
	{
		printf("connect to %s:%d success\n", hostname, port);
		return st;//连接成功后返回socket描述符
	}
}

void *socket_read(void *arg)//读socket数据的线程入口函数
{
	int st = *(int *) arg;
	char buf[BUFSIZE];
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		ssize_t rc = recv(st, buf, sizeof(buf), 0);//循环从socket中读取数据
		if (rc <= 0)//如果recv失败，代表TCP连接中断，循环break
		{
			printf("recv failed, %s\n", strerror(errno));
			break;
		} else//将从socket读取到 的数据打印到屏幕
		{
			printf("recv '%s'\nrecv %u byte\n", buf, rc);
		}
	}
	return NULL ;
}

void *socket_write(void *arg)//写socket数据的线程入口函数
{
	int st = *(int *) arg;
	char buf[BUFSIZE];
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		read(STDIN_FILENO, buf, sizeof(buf));//从键盘读取数据，放入buf中
		int ilen = strlen(buf);
		if (buf[ilen - 1] == '\n')//如果读到的数据最后为回车键，那么把回车键替换为空字符'\0'
		{
			buf[ilen - 1] = 0;
		}
		ssize_t rc = send(st, buf, strlen(buf), 0);//将从键盘读取到的字符发送出去
		printf("send '%s'\nsend %u byte\n", buf, rc);
		if (rc <= 0)
		{
			printf("send failed, %s\n", strerror(errno));
		}
	}
	return NULL ;
}

int main(int arg, char *args[])
{
	if (arg < 3)//如果参数小于2个，main函数退出
	{
		printf("usage:qqclient hostname port\n");
		return EXIT_SUCCESS;
	}

	int iport = atoi(args[2]);//将第二个参数转化为整数
	if (iport == 0)
	{
		printf("port %d is invalid\n", iport);
		return EXIT_SUCCESS;
	}
	printf("qqclient is begin\n");
	int st = socket_connect(args[1], iport);//第一个参数为IP地址，连接到第一个参数指定的IP地址
	if (st == 0)//连接失败，main函数退出
		return EXIT_FAILURE;

	pthread_t thr_read, thr_write;
	pthread_create(&thr_read, NULL, socket_read, &st);//启动读socket数据线程
	pthread_create(&thr_write, NULL, socket_write, &st);//启动写socket数据线程
	pthread_join(thr_read, NULL);
	//pthread_join(thr_write, NULL);//如果等待thr_write退出，main函数可能被挂起
	close(st);
	printf("qqclient is end\n");
	return EXIT_SUCCESS;
}

