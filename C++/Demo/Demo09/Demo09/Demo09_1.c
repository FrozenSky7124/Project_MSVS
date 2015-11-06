#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;			//������
	struct Node *next;	//ָ����
}SLIST;

//��������
int Creat_SLIST(SLIST **pHead);
//��ӡ����
int Print_SLIST(SLIST * const pHead);
//�ڽڵ�����x��ǰ�����y
int Insert_SLIST(SLIST * const pHead, int x, int y);
//ɾ���ڵ�����ΪX�Ľڵ�
int Delete_SLIST(SLIST * const pHead, int x);
//�ͷ�����
int Destory_SLIST(SLIST * pHead);

int Creat_SLIST(SLIST **p)
{
	//��ʼ����ͷ�ڵ�
	int ret = 0;
	int data = 0;
	SLIST *pHead = NULL, *pTmp = NULL, *pCur = NULL;
	pHead = (SLIST *)malloc(sizeof(SLIST));
	pHead->data = 0;
	pHead->next = NULL;
	//ѭ���������ڵ�
	printf("\nEnter Data of Node (-1:quit) ");
	scanf("%d", &data);
	pCur = pHead;
	while (data!= -1)
	{
		pTmp = (SLIST *)malloc(sizeof(SLIST));
		if (pTmp == NULL)
		{
			printf("\nError: pTmp = (SLIST *)malloc(sizeof(SLIST)); !");
			Destory_SLIST(pHead);
			ret = -1;
			return ret;
		}
		pTmp->data = data;
		pTmp->next = NULL;
		pCur->next = pTmp;
		pCur = pTmp;
		printf("\nEnter Data of Node (-1:quit) ");
		scanf("%d", &data);
	}
	*p = pHead;
	return 0;
}

int Print_SLIST(SLIST * const pHead)
{
	SLIST *pCur = NULL;
	pCur = pHead->next;
	printf("\nBegin ");
	while (pCur != NULL)
	{
		printf("%d ", pCur->data);
		pCur = pCur->next;
	}
	printf("End.\n");
	return 0;
}

int Insert_SLIST(SLIST * const pHead, int x, int y)
{
	int ret = 0;
	if (pHead == NULL) 
	{
		printf("\nError: Function Insert_SLIST pHeadֵΪNULL !");
		ret = -1;
		return ret;
	}
	SLIST *pCur = pHead->next;
	SLIST *pPre = pHead;
	while (pCur != NULL)
	{
		if (pCur->data == x)
		{
			SLIST *pTmp = NULL;
			pTmp = (SLIST *)malloc(sizeof(SLIST));
			if (pTmp == NULL)
			{
				printf("\nError : pTmp = (SLIST *)malloc(sizeof(SLIST)); !");
				ret = -1;
				return ret;
			}
			pTmp->data = y;
			pTmp->next = pCur;
			pPre->next = pTmp;
			return ret;
		}
		else
		{
			pPre = pCur;
			pCur = pCur->next;
		}
	}
	printf("\nError : �޷��ҵ�ֵ��Ϊx�Ľڵ� !");
	return ret;
}

int Delete_SLIST(SLIST * const pHead, int x)
{
	int ret = 0;
	if (pHead == NULL)
	{
		printf("\nError: Function Delete_SLIST pHeadֵΪNULL !");
		ret = -1;
		return ret;
	}
	SLIST *pCur = pHead->next;
	SLIST *pPre = pHead;
	while (pCur != NULL)
	{
		if (pCur->data == x)
		{
			pPre->next = pCur->next;
			free(pCur);
			pPre = NULL;
			pCur = NULL;
			printf("\nɾ���ڵ���ɣ�");
			return ret;
		}
		else
		{
			pPre = pCur;
			pCur = pCur->next;
		}
	}
	printf("\nError : �޷��ҵ�ֵ��Ϊx�Ľڵ� !");
	return ret;
}

int Destory_SLIST(SLIST * pHead)
{
	int ret = 0;
	if (pHead == NULL)
	{
		printf("\nError: Function Destory_SLIST pHeadֵΪNULL !");
		ret = -1;
		return ret;
	}
	SLIST *pPre = NULL, *pCur = NULL;
	pPre = pHead;
	pCur = pHead->next;
	while (pCur != NULL)
	{
		free(pPre);
		pPre = pCur;
		pCur = pCur->next;
	}
	pPre = NULL;
	pCur = NULL;
	printf("\n��Դ�ͷ���ɣ�");
	return ret;
}

void main()
{
	int ret = 0;
	SLIST *pHead = NULL;
	ret = Creat_SLIST(&pHead);
	ret = Print_SLIST(pHead);
	/**/
	printf("\n����x y ����ڵ㣺");
	int x = 0;
	int y = 0;
	scanf("%d %d", &x, &y);
	ret = Insert_SLIST(pHead, x, y);
	ret = Print_SLIST(pHead);
	/**/
	printf("\n����xɾ���ڵ㣺");
	scanf("%d", &x);
	ret = Delete_SLIST(pHead, x);
	ret = Print_SLIST(pHead);
	/**/
	ret = Destory_SLIST(pHead);
	system("pause");
}