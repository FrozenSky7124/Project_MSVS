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
	if (arg < 2)
		return -1;

	int st = socket(AF_INET, SOCK_DGRAM, 0);
	if (st == -1)
	{
		printf("socket failed %s\n", strerror(errno));
		return 0;
	}

	int port = atoi(args[1]);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(st, (struct sockaddr *)&addr, sizeof(addr)) == -1)//UDP接收数据，也需要绑定IP
	{
		printf("bind failed %s\n", strerror(errno));
		return -1;
	}
	char buf[1024];
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	while(1)
	{
		memset(&client_addr, 0, sizeof(client_addr));
		memset(buf, 0, sizeof(buf));
		if(recvfrom(st, buf, sizeof(buf), 0,
				(struct sockaddr *)&client_addr, &len) == -1)
		{
			printf("recvfrom failed %s\n", strerror(errno));
			break;
		}else
		{

			printf("%s recv is %s\n", inet_ntoa(client_addr.sin_addr), buf);
		}
	}
	close(st);
	return 0;
}
