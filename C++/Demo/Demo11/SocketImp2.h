#pragma once
#include "socketprotocol.h"

class SocketImp2 :public SocketIF
{
public:
	SocketImp2(void);
	~SocketImp2(void);

public:
	int cltSocketInit();

	//客户端发报文
	int cltSocketSend( unsigned char *buf ,  int buflen );

	//客户端收报文
	int cltSocketRev( unsigned char *buf , int *buflen );

	//客户端释放资源
	int cltSocketDestory();

private:
	unsigned char *buf;
	int buflen;
};

