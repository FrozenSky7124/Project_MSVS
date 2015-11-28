#pragma once
#include "socketprotocol.h"

class SocketImp2 :public SocketIF
{
public:
	SocketImp2(void);
	~SocketImp2(void);

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

