#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;			//数据域
	struct Node *next;	//指针域
}SLIST;

//创建链表
int Creat_SLIST(SLIST **pHead);
//打印链表
int Print_SLIST(SLIST * const pHead);
//在节点数据x的前面插入y
int Insert_SLIST(SLIST * const pHead, int x, int y);
//删除节点数据为X的节点
int Delete_SLIST(SLIST * const pHead, int x);
//释放链表
int Destory_SLIST(SLIST * pHead);

int Creat_SLIST(SLIST **p)
{
	//初始化表头节点
	int ret = 0;
	int data = 0;
	SLIST *pHead = NULL, *pTmp = NULL, *pCur = NULL;
	pHead = (SLIST *)malloc(sizeof(SLIST));
	pHead->data = 0;
	pHead->next = NULL;
	//循环创建各节点
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
		printf("\nError: Function Insert_SLIST pHead值为NULL !");
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
	printf("\nError : 无法找到值域为x的节点 !");
	return ret;
}

int Delete_SLIST(SLIST * const pHead, int x)
{
	int ret = 0;
	if (pHead == NULL)
	{
		printf("\nError: Function Delete_SLIST pHead值为NULL !");
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
			printf("\n删除节点完成！");
			return ret;
		}
		else
		{
			pPre = pCur;
			pCur = pCur->next;
		}
	}
	printf("\nError : 无法找到值域为x的节点 !");
	return ret;
}

int Destory_SLIST(SLIST * pHead)
{
	int ret = 0;
	if (pHead == NULL)
	{
		printf("\nError: Function Destory_SLIST pHead值为NULL !");
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
	printf("\n资源释放完成！");
	return ret;
}

void main()
{
	int ret = 0;
	SLIST *pHead = NULL;
	ret = Creat_SLIST(&pHead);
	ret = Print_SLIST(pHead);
	/**/
	printf("\n输入x y 插入节点：");
	int x = 0;
	int y = 0;
	scanf("%d %d", &x, &y);
	ret = Insert_SLIST(pHead, x, y);
	ret = Print_SLIST(pHead);
	/**/
	printf("\n输入x删除节点：");
	scanf("%d", &x);
	ret = Delete_SLIST(pHead, x);
	ret = Print_SLIST(pHead);
	/**/
	ret = Destory_SLIST(pHead);
	system("pause");
}