#ifndef _CLT_SOCKET_H__
#define _CLT_SOCKET_H__

//客户端初始化
int cltSocket_init(void **handle);

//客户端发送报文
int cltSocket_senddata(void *handle, unsigned char *buf, int buflen);

//客户端接收报文
int cltSocket_resvdata(void *handle, unsigned char *buf, int *buflen);

//客户端释放
int cltSocket_destroy(void *handle);

#endif
