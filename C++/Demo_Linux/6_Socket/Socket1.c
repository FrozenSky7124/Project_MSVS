#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	//创建套接字描述符
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		printf("Error in socket(). %s\n", strerror(errno));
		return -1;
	}
	//定义IP地址结构变量
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	//设置结构类型为TCP/IP
	addr.sin_family = AF_INET;
	//设置端口号为8080 htons()函数将host字节类型转换为net short字节类型
	addr.sin_port = htons(7124);
	//设置IP地址
	addr.sin_addr.s_addr = inet_addr("192.168.1.112");
	//调用connect函数建立连接
	if ((connect(sd, (struct sockaddr *)&addr, sizeof(addr))) == -1)
	{
		printf("Error in connect. %s\n", strerror(errno));
		return -1;
	}
	char s[1024];
	while (1)
	{
		memset(s, 0, sizeof(s));
		read(STDIN_FILENO, s, sizeof(s));
		if (send(sd, s, strlen(s), 0) == -1)
		{
			printf("Error in send. %s\n", strerror(errno));
			return -1;
		}
		memset(s, 0, sizeof(s));
		if (recv(sd, s, sizeof(s), 0) > 0)
		{
			printf("%s\n", s);
		}
		else
		{
			break;
		}
	}

	close(sd);
	return 0;
}