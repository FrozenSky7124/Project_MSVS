#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cltsocket01.h"
#include "cltsocket02.h"

//�ͻ��˳�ʼ��
int cltSocket_init(void **handle)
{
	return 0;
}

//�ͻ��˷��ͱ���
int cltSocket_senddata(void *handle, unsigned char *buf, int buflen)
{
	return 0;
}

//�ͻ��˽��ձ���
int cltSocket_resvdata(void *handle, unsigned char *buf, int *buflen)
{
	return 0;
}

//�ͻ����ͷ�
int cltSocket_destroy(void *handle)
{
	return 0;
}
/*
void main()
{
	printf("Hello!\n");
	system("pause");
}
*/