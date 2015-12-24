Linux网络编程
=============

**TCP连接的三次握手**

第一次握手：建立连接时，客户端发送syn包（syn=j）到服务器，并进入SYN_SENT状态，等待服务器确认；SYN：同步序列编号（Synchronize Sequence Numbers）。

第二次握手：服务器收到syn包，必须确认客户的SYN（ack=j+1），同时自己也发送一个SYN包（syn=k），即SYN+ACK包，此时服务器进入SYN_RECV状态；

第三次握手：客户端收到服务器的SYN+ACK包，向服务器发送确认包ACK(ack=k+1)，此包发送完毕，客户端和服务器进入ESTABLISHED（TCP连接成功）状态，完成三次握手。

**Socket**

```c
	#include <sys/types.h>
	#include <sys/socket.h>
	int socket(int domain, int type, int protocol);
```

**struct sockaddr_in**

```c
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	//设置结构类型为TCP/IP
	addr.sin_family = AF_INET;
	//设置端口号为8080 htons()函数将host字节类型转换为net short字节类型
	addr.sin_port = htons(7124);
	//设置IP地址
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
```