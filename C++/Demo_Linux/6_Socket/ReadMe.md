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

```c
	//获得远端sockaddr
	int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	//获得自身sockaddr
	int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

**非阻塞Socket**

非阻塞(nonblocking)调用，在调用后立即返回。

nonblocking状态下，accept函数，没有客户端连接请求返回-1，同时error值为EAGAIN或者EWOULDBLOCK（宏定义都为整数11）

nonblocking状态下，recv,recvfrom函数，如果没有接收到数据，返回-1，error值为11。如果socket关闭，函数返回0。

nonblocking状态下，对已经关闭的socket调用send函数，将触发SIGPIPE信号，如果不对SIGPIPE信号进行捕获，操作系统默认会将进程关闭。

```c
	//参数cmd：[F_GETFL]得到当前状态  [F_SETFL]设置状态
	//宏定义： O_NONBLOCK代表非阻塞状态  0代表阻塞状态
	//返回值表示描述符当前状态
	int fcntl(int fd, int cmd, ... /* arg */ );
```
**Linux下的多路复用：epoll**

```c
	#include <sys/epoll.h>
	int epoll_create(int size);
	int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
	int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

**Linux下的多路复用：select**

```c
	#include <sys/select.h>
	int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```