#ifndef _CLT_SOCKET_H__
#define _CLT_SOCKET_H__

//�ͻ��˳�ʼ��
int cltSocket_init(void **handle);

//�ͻ��˷��ͱ���
int cltSocket_senddata(void *handle, unsigned char *buf, int buflen);

//�ͻ��˽��ձ���
int cltSocket_resvdata(void *handle, unsigned char *buf, int *buflen);

//�ͻ����ͷ�
int cltSocket_destroy(void *handle);

#endif
