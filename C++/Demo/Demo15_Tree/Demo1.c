#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//Sturcture:BinaryTreeLink
typedef struct _BiTNode
{
	int data;
	struct _BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

//Sturcture:TriTreeLink
typedef struct _TriTNode
{
	int data;
	struct _TriTNode *lchild, *rchild;
	struct _TriTNode *parent;
}TriTNode, *TriTree;

//PreOrder
void PreOrder(BiTNode *T)
{
	if (T != NULL)
	{
		printf("%d ", T->data);
		PreOrder(T->lchild);
		PreOrder(T->rchild);
	}
}

//InOrder
void InOrder(BiTNode *T)
{
	if (T != NULL)
	{
		InOrder(T->lchild);
		printf("%d ", T->data);
		InOrder(T->rchild);
	}
}

//PostOrder
void PostOrder(BiTNode *T)
{
	if (T != NULL)
	{
		PostOrder(T->lchild);
		PostOrder(T->rchild);
		printf("%d ", T->data);
	}
}

//CountLeaf
void CountLeaf(BiTNode *T, int *ncount)
{
	if (T != NULL)
	{
		if (T->lchild == NULL && T->rchild == NULL)
		{
			(*ncount)++;
		}
		CountLeaf(T->lchild, ncount);
		CountLeaf(T->rchild, ncount);
	}
}

//Depth
int Depth(BiTNode *T)
{
	int depthvalue = 0, ldepth = 0, rdepth = 0;
	if (T == NULL)
	{
		depthvalue = 0;
		return depthvalue;
	}
	ldepth = Depth(T->lchild);
	rdepth = Depth(T->rchild);
	depthvalue = 1 + (ldepth > rdepth) ? ldepth : rdepth;
	return depthvalue;
}

void main()
{
	//Init
	BiTNode b1, b2, b3, b4, b5;
	memset(&b1, 0, sizeof(BiTNode));
	memset(&b2, 0, sizeof(BiTNode));
	memset(&b3, 0, sizeof(BiTNode));
	memset(&b4, 0, sizeof(BiTNode));
	memset(&b5, 0, sizeof(BiTNode));
	b1.data = 1;
	b2.data = 2;
	b3.data = 3;
	b4.data = 4;
	b5.data = 5;
	//Relationship
	b1.lchild = &b2;
	b1.rchild = &b3;
	b2.lchild = &b4;
	b3.lchild = &b5;

	//Depth
	Depth(&b1);
	system("pause");
}