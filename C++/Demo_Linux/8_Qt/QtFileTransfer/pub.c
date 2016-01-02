/*
 * pub.c
 *
 *  Created on: 2014-3-10
 *      Author: zhujy
 */

#ifdef WIN
#include <WinSock2.h>
#else
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#define SOCKET int
#endif

#include <stdio.h>
#include "pub.h"

#define BUFSIZE 262144  //256kvoid getfilename(const char *filename, char *name)//从完整路径名中解析出文件名称，例如：/home/test/abc.txt,解析完成后为abc.txt{
	int len = strlen(filename);
	int i;
	for (i = (len - 1); i >= 0; i--)
	{
		if ((filename[i] == '\\') || (filename[i] == '/'))
		{
			break;
		}
	}
	strcpy(name, &filename[i + 1]);
	return;
}

SOCKET init_socket()//初始化socket
{
//如果是windows，执行如下代码
#ifdef WIN
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
#endif

	return 0;
}

SOCKET socket_connect(const char *hostname, int port)//连接到指定的主机和端口号
{
	if (init_socket() == -1)
		return 0;

	SOCKET st = socket(AF_INET, SOCK_STREAM, 0);//建立TCP socket
	if (st == 0)
		return 0;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);//指定port为要连接的端口号
	addr.sin_addr.s_addr = inet_addr(hostname);//指定hostname为要连接的IP地址
	if (connect(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("connect to %s:%d failed %s\n", hostname, port, strerror(errno));
		return 0;//连接失败，返回0
	} else
	{
		return st;//连接成功，返回socket描述符
	}
}

SOCKET socket_create(int port)//在port指定的端口上建立server端socket
{
	if (init_socket() == -1)
		return 0;

	SOCKET st = socket(AF_INET, SOCK_STREAM, 0);//建立TCP socket
	if (st == 0)
		return 0;//如果建立socket失败，返回0

#ifdef WIN
	const char on = 0;
#else
	int on = 0;
#endif

	if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		printf("setsockopt failed %s\n", strerror(errno));
		return 0;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)//server端socket，所以需要绑定IP地址
	{
		printf("bind failed %s\n", strerror(errno));
		return 0;
	}
	if (listen(st, 20) == -1)
	{
		printf("listen failed %s\n", strerror(errno));
		return 0;
	}
	printf("listen %d success\n", port);
	return st;//server端socket建立成功,返回server端socket描述符
}

SOCKET socket_accept(SOCKET listen_st)//server端socket开始accept的函数
{
	struct sockaddr_in client_addr;

#ifdef WIN
	int len = 0;
#else
	unsigned int len = 1;
#endif

	len = sizeof(client_addr);
	memset(&client_addr, 0, sizeof(client_addr));
	SOCKET client_st = accept(listen_st, (struct sockaddr *) &client_addr,
			&len);//accept阻塞，直到有client连接到server才返回
	if (client_st == -1)
	{
		printf("accept failed %s\n", strerror(errno));
		return 0;
	} else
	{
		printf("accept by %s\n", inet_ntoa(client_addr.sin_addr));
		return client_st;//有client连接到server，返回client的socket描述符
	}
}

int send_work(const char *hostname, int port, const char *filename)//向hostname指定的IP地址发送filename指定的文件
{
	SOCKET st = socket_connect(hostname, port);//连接到hostname指定的IP地址和port指定的端口号
	if (st == 0)//连接失败，函数返回
		return 0;

	FILE *fd = fopen(filename, "rb");//以只读方式打开filename指定的文件
	if (fd == NULL)//如果文件打开失败，函数返回
	{
		printf("open %s failed %s\n", filename, strerror(errno));
		return 0;
	}

	char *buf = malloc(BUFSIZE);//申请一个缓冲区，存放接收到的文件内容
	memset(buf, 0, BUFSIZE);
	getfilename(filename, buf);//从完整路径名中解析出文件名称，例如：/home/test/abc.txt,解析完成后为abc.txt
	size_t rc = send(st, buf, strlen(buf), 0);//客户端第一次给server端发送的数据为要传递的文件名称，将解析完成后的文件名通过socket发送给server端
	if (rc <= 0)
	{
		if (rc < 0)
			printf("send failed %s\n", strerror(errno));
		else
			printf("socket disconnect\n");

	} else
	{

		memset(buf, 0, BUFSIZE);
		if (recv(st, buf, BUFSIZE, 0) <= 0)//接收来自server端的回复
		{
			if (rc < 0)
				printf("recv failed %s\n", strerror(errno));
			else
				printf("socket disconnect\n");
		} else
		{
			if (strncmp(buf, "OK", 2) == 0)//如果收到来自服务端的回复，代表服务端认可，可以发送文件了
			{
				while (1)
				{
					memset(buf, 0, BUFSIZE);
					rc = fread(buf, 1, BUFSIZE, fd);//循环读取文件，直到读到文件尾，循环break
					if (rc <= 0)
					{
						if (rc < 0)
							printf("fread failed %s\n", strerror(errno));
						break;
					} else
					{
						rc = send(st, buf, rc, 0);//将从文件中读到的数据，通过socket发送到server端，其中rc为从文件中读到的数据大小
						if (rc <= 0)//如果发送失败，代表TCP连接出错，循环break
						{
							if (rc < 0)
								printf("send failed %s\n", strerror(errno));
							else
								printf("socket disconnect\n");
							break;
						}
					}
				}
			}
		}
	}

	fclose(fd);
	free(buf);

#ifdef WIN	
	closesocket(st);
	WSACleanup();
#else	
	close(st);
#endif
	return 1;
}

int recv_work(int port)//server端socket在port指定的端口上listen，接收来自client发送的文件
{
	SOCKET listen_st = socket_create(port);//建立server端socket，在port指定端口listen
	if (listen_st == 0)//如果创建服务端socket失败，函数返回0
		return 0;
	SOCKET st = socket_accept(listen_st);//如果有client连接到达，socket_accept函数返回client的socket描述符
	if (st == 0)
		return 0;

	char *buf = malloc(BUFSIZE);//建立接收文件数据缓冲区
	FILE *fd = NULL;

	memset(buf, 0, BUFSIZE);
	size_t rc = recv(st, buf, BUFSIZE, 0);//接收来自client的数据，客户端第一次要发送的文件名称
	if (rc <= 0)
	{
		if (rc < 0)
			printf("recv failed %s\n", strerror(errno));
		else
			printf("socket disconnect\n");

	} else
	{
		printf("receiving %s\n", buf);

		fd = fopen(buf, "wb");//以只写方式打开文件
		if (fd == NULL)
		{
			printf("open %s failed %s\n", buf, strerror(errno));
		} else
		{
			memset(buf, 0, BUFSIZE);
			strcpy(buf, "OK");
			rc = send(st, buf, strlen(buf), 0);//回复客户端，同意接收文件
			if (rc <= 0)
			{
				if (rc < 0)
					printf("send failed %s\n", strerror(errno));
				else
					printf("socket disconnect\n");
			}

			while (1)
			{
				memset(buf, 0, BUFSIZE);
				rc = recv(st, buf, BUFSIZE, 0);//循环接收来自client的数据，数据为发送文件的内容
				if (rc <= 0)//如果client连接断开，代表文件传递完成，或者网络意外中断,循环break
				{
					if (rc < 0)
						printf("recv failed %s\n", strerror(errno));
					else
						printf("socket disconnect\n");
					break;
				} else
				{
					fwrite(buf, 1, rc, fd);//将从client端收到的内容写入文件
				}
			}
		}
	}

	if (fd)
		fclose(fd);//关闭打开的文件
	free(buf);

#ifdef WIN	
	closesocket(st);
	closesocket(listen_st);
	WSACleanup();
#else	
	close(st);
	close(listen_st);
#endif
	return 1;
}

