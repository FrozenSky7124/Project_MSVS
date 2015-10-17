#ifndef _CLT_SOCKET2_H__
#define _CLT_SOCKET2_H__

//客户端初始化
int cltSocket_init2(void **handle);

//客户端发送报文
int cltSocket_senddata2(void *handle, unsigned char *buf, int buflen);

//客户端接收报文
int cltSocket_resvdata2(void *handle, unsigned char **buf, int *buflen);

int cltSocket_resvdata_Free2(unsigned char *buf);

//客户端释放
int cltSocket_destroy2(void **handle);

#endif
