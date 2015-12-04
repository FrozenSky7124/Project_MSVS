#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "seqlist.h"

typedef struct _tag_SeqList
{
	int capacity;
	int length;
	int *node;
}TSeqList;

/*
SeqList* SeqList_Create(int capacity)
{
	TSeqList *ret = NULL;
	ret = (TSeqList *)malloc(sizeof(TSeqList));
	if (ret == NULL) { return NULL; }
	ret->capacity = capacity;
	ret->length = 0;
	ret->node = (int *)malloc(sizeof(int *)*capacity);
	if (ret->node == NULL) { return NULL; }
	return ret;
}
*/
SeqList* SeqList_Create(int capacity)
{
	TSeqList *ret = NULL;
	ret = (TSeqList *)malloc(sizeof(TSeqList) + sizeof(int *)*capacity);
	if (ret == NULL) { return NULL; }
	ret->capacity = capacity;
	ret->length = 0;
	ret->node = (int *)(ret + 1);
	return ret;
}

void SeqList_Destroy(SeqList* list)
{
	if (list == NULL)
	{
		return NULL;
	}
	free(list);
	return;
}

void SeqList_Clear(SeqList* list)
{
	if (list == NULL)
	{
		return NULL;
	}
	TSeqList * tmpList = (TSeqList *)list;
	tmpList->length = 0;
	return;
}

int SeqList_Length(SeqList* list)
{
	if (list == NULL)
	{
		return -1;
	}
	TSeqList * tmpList = (TSeqList *)list;
	return tmpList->length;
}

int SeqList_Capacity(SeqList* list)
{
	if (list == NULL)
	{
		return -1;
	}
	TSeqList * tmpList = (TSeqList *)list;
	return tmpList->capacity;
}

int SeqList_Insert(SeqList* list, SeqListNode* node, int pos)
{
	if (list == NULL || node == NULL)
	{
		return -1;
	}
	TSeqList * SList = (TSeqList *)list;
	if (pos < 0 || pos >= SList->capacity)
	{
		return -2;
	}
	if (SList->length >= SList->capacity)	//ÅÐ¶ÏÒÑÂú
	{
		return -3;
	}
	if (pos > SList->length)	//ÅÐ¶Ï¼ä¸ô²åÈë
	{
		pos = SList->length;
	}
	int i = 0;
	int *nodePtr = NULL;
	nodePtr = (int *)SList->node;
	for (i = SList->length; i > pos; i--)
	{
		nodePtr[i] = nodePtr[i - 1];
	}
	nodePtr[pos] = node;
	SList->length++;
	return 0;
}

SeqListNode* SeqList_Get(SeqList* list, int pos)
{
	TSeqList * SList = (TSeqList *)list;
	if (list == NULL || pos < 0 || pos >= SList->length)
	{
		return NULL;
	}
	return (SeqListNode *)SList->node[pos];
}

SeqListNode* SeqList_Delete(SeqList* list, int pos)
{
	int i = 0;
	SeqListNode *tmp = NULL;
	TSeqList *SList = (TSeqList *)list;
	if (list == NULL || pos <0 || pos >SList->length)
	{
		return NULL;
	}
	tmp = (SeqListNode *)SList->node[pos];
	for (i = pos;i < SList->length - 1;i++)
	{
		SList->node[i] = SList->node[i + 1];
	}
	SList->length--;
	return tmp;
}