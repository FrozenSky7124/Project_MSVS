#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "linklist.h"

typedef struct _tag_LinkList
{
	LinkListNode header;
	int length;
}TLinkList;

LinkList* LinkList_Create()
{
	TLinkList *tList = (TLinkList *)malloc(sizeof(TLinkList));
	if (tList == NULL)
	{
		return NULL;
	}
	tList->header.next = NULL;
	tList->length = 0;
	return tList;
}

void LinkList_Destroy(LinkList* list)
{
	if (list != NULL)
	{
		free(list);
	}
	return ;
}

void LinkList_Clear(LinkList* list)
{
	TLinkList *tList = list;
	if (tList == NULL)
	{
		return ;
	}
	tList->length = 0;
	tList->header.next = NULL;
	return ;
}

int LinkList_Length(LinkList* list)
{
	TLinkList *tList = list;
	if (tList == NULL)
	{
		return -1;
	}
	return tList->length;
}

int LinkList_Insert(LinkList* list, LinkListNode* node, int pos)
{
	int i = 0;
	TLinkList  *tList = (TLinkList *)list;
	LinkListNode  *current = NULL;

	if (tList == NULL || node == NULL || pos<0)
	{
		return -1;
	}
	current = &tList->header;
	//current = (LinkListNode *)list;

	for (i=0; (i<pos)&&current->next!=NULL; i++ )
	{
		current = current->next;
	}
	//新节点链接后续链表
	node->next = current->next;
	//前面链表链接node
	current->next = node;
	tList->length ++;

	return 0;
}

LinkListNode* LinkList_Get(LinkList* list, int pos)
{
	int i = 0;
	TLinkList  *tList = (TLinkList *)list;
	LinkListNode  *current = NULL;
	LinkListNode  *ret = NULL;

	if (list==NULL || pos<0 || pos>=tList->length)
	{
		return NULL;
	}

	current = &tList->header;
	for (i=0; i<pos; i++)
	{
		current = current->next;
	}
	ret = current->next;
	
	return ret;
}

LinkListNode* LinkList_Delete(LinkList* list, int pos)
{
	int i = 0;
	TLinkList  *tList = (TLinkList *)list;
	LinkListNode  *current = NULL;
	LinkListNode  *ret = NULL;

	if (list==NULL || pos<0 || pos>=tList->length)
	{
		return NULL;
	}
	//没有初始化环境
	current = &tList->header;
	for (i=0; i<pos; i++)
	{
		current = current->next;
	}
	ret = current->next;

	current->next = ret->next;
	tList->length --;
	return ret;
}