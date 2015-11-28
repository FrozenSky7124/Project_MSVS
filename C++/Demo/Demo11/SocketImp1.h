#pragma once
#include "socketprotocol.h"

class SocketImp1 :public SocketIF
{
public:
	SocketImp1(void);
	~SocketImp1(void);

public:
	 int cltSocketInit();

	//�ͻ��˷�����
	 int cltSocketSend( unsigned char *buf ,  int buflen );

	//�ͻ����ձ���
	 int cltSocketRev( unsigned char *buf , int *buflen );

	//�ͻ����ͷ���Դ
	 int cltSocketDestory();

private:
	unsigned char *buf;
	int buflen;
};

