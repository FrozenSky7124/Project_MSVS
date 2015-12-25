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

#define BUFFSIZE 262144 //缓存空间256KB

void getfilename(const char *filename, char *name)
{
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
	return ;
}

int init_socket()//初始化Socket
{
//如果在Windows系统下编译，执行如下代码:
#ifdef WIN
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);//启动Socket
	if (err != 0)
	{
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)//判断SOCKET版本
	{
		WSACleanup();
		return -1;
	}
#endif
	return 0;
}

SOCKET socket_connect(const char *hostname, int port)
{
	if (init_socket() == -1)
		return 0;

	SOCKET st = socket(AF_INET, SOCK_STREAM, 0);
	if (st == 0)
		return 0;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(hostname);//inet_addr()将char*类型的IP转换为uint32_t类型的IP
	if (connect(st, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("Error in Connect to %s:%d. %s\n", hostname, port, strerror(errno));
		return 0;//连接失败 返回0
	}
	else
	{
		return st;//连接成功 返回socket描述符
	}
}

int send_work(const char *hostname, int port, const char*filename)
{
	SOCKET st = socket_connect(hostname, port);
	if (st == 0) //连接失败socket_connect函数返回0
		return 0;

	FILE *fd = fopen(filename, "rb");
	if (fd == NULL)
	{
		printf("Error in open file:%s.  %s\n", hostname, strerror(errno));
		return 0;
	}

	char *buf = malloc(BUFFSIZE);
	memset(buf, 0, BUFFSIZE);
	getfilename(filename, buf); //从路径字符串中解析出文件名
	size_t rc = send(st, buf, strlen(buf), 0); //传送文件名
	if (rc <= 0)
	{
		if (rc < 0)
			printf("Error in send filename. %s\n", strerror(errno));
		else
			printf("Socket Disconnect.\n");
	}
	else
	{
		memset(buf, 0, BUFFSIZE);
		if ((rc = recv(st, buf, BUFFSIZE, 0)) <= 0)
		{
			if (rc < 0)
				printf("Error in receive OK. %s\n", strerror(errno));
			else
				printf("Socket Disconnect.\n");
		}
		else
		{
			if (strncmp(buf, "OK", 2) == 0) //收到服务端回复，可以发送文件数据
			{
				while (1)
				{
					memset(buf, 0, BUFFSIZE);
					rc = fread(buf, 1, BUFFSIZE, fd);
					if (rc <= 0)
					{
						if (rc < 0)
							printf("Error in Readfile. %s\n", strerror(errno));
						break;
					}
					else
					{
						rc = send(st, buf, rc, 0);
						if (rc <= 0)
						{
							if (rc < 0)
								printf("Error in Send. %s\n", strerror(errno));
							else
								printf("Socket Disconnect.\n");
							break;
						}
					}
				}
			}
			else
			{
				printf("Error in Waiting 'OK'.\n");
			}
		}
	}
	fclose(fd);
	free(buf);
#ifdef WIN
	closesocket(st);//Windows系统下调用closesocket()关闭套接字连接
	WSACleanup();//Windows系统下调用WSACleanup关闭SOCKET
#else
	close(st);
#endif
	return 1;
}

SOCKET socket_create(int port)
{
	if (init_socket() == -1)
		return 0;

	SOCKET st = socket(AF_INET, SOCK_STREAM, 0);
	if (st == 0)
		return 0;

#ifdef WIN
	const char on = 0;
#else
	int on = 0;
#endif

	if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		printf("Error in setsockopt. %s\n", strerror(errno));
		return 0;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(st, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("Error in bind. %s\n", strerror(errno));
		return 0;
	}
	if (listen(st, 20) == -1)
	{
		printf("Error in listen. %s\n", strerror(errno));
		return 0;
	}
	printf("Listen %d Success.\n", port);
	return st;
}

SOCKET socket_accept(SOCKET listen_st)
{
	struct sockaddr_in client_addr;

#ifdef WIN
	int len = 0;
#else
	unsigned int len = 1;
#endif

	len = sizeof(client_addr);
	memset(&client_addr, 0, sizeof(client_addr));
	SOCKET client_st = accept(listen_st, (struct sockaddr *)&client_addr, &len);
	if (client_st == -1)
	{
		printf("Error in Accept. %s\n", strerror(errno));
		return 0;
	}
	else
	{
		printf("Accept client: %s\n", inet_ntoa(client_addr.sin_addr));
		return client_st;
	}
}

int recv_work(int port)
{
	SOCKET listen_st = socket_create(port);
	if (listen_st == 0)
		return 0;
	SOCKET st = socket_accept(listen_st);//如果有client连接，socket_accept函数返回client的socket
	if (st == 0)
		return 0;

	FILE *fd = NULL;
	char *buf = malloc(BUFFSIZE);
	memset(buf, 0, BUFFSIZE);

	size_t rc = recv(st, buf, BUFFSIZE, 0);
	if (rc <= 0)
	{
		if (rc < 0)
		{
			printf("Error in recv file. %s\n", strerror(errno));
		}
		else
		{
			printf("Socket Disconnect.\n");
		}
	}
	else
	{
		printf("Receiving File: %s\n", buf);

		fd = fopen(buf, "wb");
		if (fd == NULL)
		{
			printf("Error in fopen file: %s  %s\n", buf, strerror(errno));
		}
		else
		{
			memset(buf, 0, BUFFSIZE);
			strcpy(buf, "OK");
			rc = send(st, buf, strlen(buf), 0); //回复客户端OK
			if (rc <= 0)
			{
				if (rc < 0)
					printf("Error in send OK. %s\n", strerror(errno));
				else
					printf("Socket Disconnect\n");
			}
			else
			{
				while (1)
				{
					memset(buf, 0, BUFFSIZE);
					rc = recv(st, buf, BUFFSIZE, 0);
					if (rc <= 0)
					{
						if (rc < 0)
							printf("Error in recv. %s\n", strerror(errno));
						else
							printf("Socket Disconnect\n");
						break;
					}
					else
					{
						fwrite(buf, 1, rc, fd);
					}
				}
			}
		}
	}
	if (fd)
		fclose(fd);
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