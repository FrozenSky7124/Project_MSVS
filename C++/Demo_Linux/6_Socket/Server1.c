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

	int on = 1;
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		printf("Error in setsockopt. %s\n", strerror(errno));
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
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//Server与IP地址绑定
	if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("Error in bind. %s\n", strerror(errno));
		return -1;
	}

	//Server开始监听
	//定义最大连接数20
	if (listen(sd, 20) == -1)
	{
		printf("Error in Listen. %s\n", strerror(errno));
		return -1;
	}

	char s[1024];
	memset(s, 0, sizeof(s));
	int client_sd = 0;
	socklen_t len = 0 ;
	struct sockaddr_in client_addr;
	int i;
	for (i = 0; i < 5; i++)
	{
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t len = sizeof(client_addr);
		//accept阻塞，直到客户端连接，accept返回client的套接字描述符
		client_sd = accept(sd, (struct sockaddr *)&client_addr, &len);
		if (client_sd == -1)
		{
			printf("Error in accept. %s\n", strerror(errno));
			return -1;
		}
		printf("Get connect from:%s\n", inet_ntoa(client_addr.sin_addr));

		while (1)
		{
			memset(s, 0, sizeof(s));
			int rec = recv(client_sd, s, sizeof(s), 0);
			if ( rec > 0)
			{
				printf("Recv: %s\n", s);
				send(client_sd, "-------Confirm.\n", strlen("-------Confirm.\n"), 0);
			}
			else if (rec == 0)
			{
				printf("Client Close.\n");
				close(client_sd);
				break;
			}
			else
			{
				printf("Error in recv. %s\n", strerror(errno));
				close(client_sd);
				return -1;
			}
		}
	}
	close(sd);
	return 0;
}