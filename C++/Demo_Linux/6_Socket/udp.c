/*
 ============================================================================
 Name        : udp.c
 Author      : zhujy
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int main(int arg, char *args[])
{
	if (arg < 3)
		return -1;

	int st = socket(AF_INET, SOCK_DGRAM, 0);//建立socket的时候第二个参数值为SOCK_DGRAM
	if (st == -1)
	{
		printf("socket failed %s\n", strerror(errno));
		return 0;
	}

	int port = atoi(args[2]);


	int on = 1;
	if (setsockopt(st, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) == -1)//设置UDP socket可以发送广播消息
	{
		printf("setsockopt failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}



	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(args[1]);
	char buf[1024];
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		read(STDIN_FILENO, buf, sizeof(buf));//读取用户键盘输入
		if (sendto(st, buf, strlen(buf), 0, (struct sockaddr *) &addr,
				sizeof(addr)) == -1)//udp使用sendto发送消息
		{
			printf("sendto failed %s\n", strerror(errno));
			break;
		}
	}
	close(st);
	return EXIT_SUCCESS;
}

