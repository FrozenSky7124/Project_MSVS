#ifndef _CLT_SOCKET2_H__
#define _CLT_SOCKET2_H__

//�ͻ��˳�ʼ��
int cltSocket_init2(void **handle);

//�ͻ��˷��ͱ���
int cltSocket_senddata2(void *handle, unsigned char *buf, int buflen);

//�ͻ��˽��ձ���
int cltSocket_resvdata2(void *handle, unsigned char **buf, int *buflen);

int cltSocket_resvdata_Free2(unsigned char *buf);

//�ͻ����ͷ�
int cltSocket_destroy2(void **handle);

#endif
