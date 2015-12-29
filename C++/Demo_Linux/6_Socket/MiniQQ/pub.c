/*
 * pub.c
 *
 *  Created on: 2014年4月20日
 *      Author: zhujy
 */

#include "pub.h"


int socket_client[2]; //申明2个socket_client，管理2个client的socket连接

void catch_Signal(int Sign)
{
	switch (Sign)
	{
	case SIGINT: //捕捉SIGINT消息
		printf("signal SIGINT\n");
		break;
	}
}

int signal1(int signo, void (*func)(int))
{
	struct sigaction act, oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	return sigaction(signo, &act, &oact);
}

void init_socket_client() //初始化int socket_client[2]数组
{
	memset(socket_client, 0, sizeof(socket_client));
}

//接收到来自client发送的消息，index为0,代表接收到socket_client[0]消息，然后给socket_client[1]发送消息
//index为1,代表接收到socket_client[1]消息，然后给socket_client[0]发送消息
//buf为发送消息内容，len为发送消息长度
void deliver(int index, const char *buf, ssize_t len)
{
	ssize_t rc = 0;
	if (index == 0) //如果 index为0，代表0给1发消息
	{
		if (socket_client[1] == 0) //1不在线
		{
			printf("%d:user not online\n", index);
		} else
		{
			rc = send(socket_client[1], buf, len, 0); //将消息下发给1
			printf("send '%s'\nsend %u bytes\n", buf, rc);
			if (rc <= 0)
			{
				if (rc == 0)
					printf("send failed, disconn\n");
				else
					printf("send failed, %s\n", strerror(errno));
			}
		}
	}

	if (index == 1) //如果 index为1，代表1给0发消息
	{
		if (socket_client[0] == 0) ////0不在线
		{
			printf("%d:user not online\n", index);
		} else
		{
			rc = send(socket_client[0], buf, len, 0); //将消息下发给0
			printf("send '%s'\nsend %u byte\n", buf, rc);
			if (rc <= 0)
			{
				if (rc == 0)
					printf("send failed, disconn\n");
				else
					printf("send failed, %s\n", strerror(errno));
			}
		}
	}
}

//接收来自client端的连接,index为0代表接收来自socket_client[0]的消息
//index为1代表接收来自socket_client[1]的消息
void socket_work(int index)
{
	char buf[BUFSIZE];
	ssize_t rc = 0;
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		rc = recv(socket_client[index], buf, sizeof(buf), 0);
		if (rc <= 0) //client连接断开
		{
			if (rc == 0)
				printf("%d:recv disconn\n", index);
			else
				printf("%d:recv failed, %s\n", index, strerror(errno));
			close(socket_client[index]);
			socket_client[index] = 0; //如果client的socket已经断开，那么置socket_client[]数组中相应的元素为0
			break;
		} else
		{

			///////////////////////
			printf("%d:recv '%s'\nrecv %u bytes\n", index, buf, rc);
			//index为0,代表接收到socket_client[0]消息，然后给socket_client[1]发送消息
			//index为1,代表接收到socket_client[1]消息，然后给socket_client[0]发送消息
			//buf为发送消息内容
			deliver(index, buf, rc);
		}
	}
}

void *socket_contrl(void *arg)			//server端的线程入口函数
{
	int client_st = *(int *) arg;			//得到从accept函数返回的来自client端的socket描述符
	free((int *) arg);
	printf("contrl_thread is begin\n");
	int index = 0;

	if (socket_client[0] == 0)//如果socket_client[0]空闲，就将来自client端的socket赋给socket_client[0]
	{
		socket_client[0] = client_st;
	} else			//如果socket_client[1]空闲，就将来自client端的socket赋给socket_client[1]
	{
		if (socket_client[1] == 0)
		{
			socket_client[1] = client_st;
			index = 1;
		} else			//socket_client[0]socket_client[1]都不空闲
		{
			printf("满了，不能连了\n");
			close(client_st);		//socket_client[2]两个成员都已经在线了，拒绝其他client连接
			return NULL;
		}
	}
	//接收来自client端的连接,index为0代表接收来自socket_client[0]的消息
	//index为1代表接收来自socket_client[1]的消息
	socket_work(index);
	printf("contrl_thread is end\n");
	return NULL;
}

int socket_create(uint16_t port)	//创建server端socket在port指定端口listen
{
	int st = socket(AF_INET, SOCK_STREAM, 0);	//创建TUP socket描述符
	int on = 1;
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
	if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)	//在port端口上绑定
	{
		printf("bind failed %s\n", strerror(errno));
		return 0;
	}
	if (listen(st, 20) == -1)	//开始listen
	{
		printf("listen failed %s\n", strerror(errno));
		return 0;
	}
	printf("listen %d success\n", port);
	return st;	//返回listen的server端socket描述符
}

void sockaddr_toa(const struct sockaddr_in *addr, char *IPAddr)	//将addr转化为字符串IPAddr
{
	unsigned char *p = (unsigned char *) &(addr->sin_addr.s_addr);
	sprintf(IPAddr, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
}

void socket_accept(int st)	//server端socket的accept函数
{
	pthread_t thr_d;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //设置线程属性为可分离

	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	while (1)
	{
		memset(&client_addr, 0, sizeof(client_addr));
		int client_st = accept(st, (void *) &client_addr, &len); //accept函数阻塞，直到有client的socket连接到达
		if (client_st == -1) //accept失败，
		{
			printf("accept failed %s\n", strerror(errno));
			break;
		} else
		{
			char sIP[32];
			memset(sIP, 0, sizeof(sIP));
			sockaddr_toa(&client_addr, sIP); //将IP地址转化为字符串
			printf("accept by %s\n", sIP);

			int *tmp = malloc(sizeof(int));
			*tmp = client_st;
			//只要accept到来自client端的socket，就启动一个线程，线程入口函数为socket_contrl
			pthread_create(&thr_d, &attr, socket_contrl, tmp);
		}
	}
	pthread_attr_destroy(&attr);
}

