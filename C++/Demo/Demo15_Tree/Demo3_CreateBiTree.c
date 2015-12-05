#define _CRT_SECURE_NO_WARNINGS
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

typedef struct _BiTNode
{
	int data;
	struct _BiTNode *lc;
	struct _BiTNode *rc;
}BiTNode;

//先序遍历输出
void PreOrder(BiTNode *T)
{
	if (T != NULL)
	{
		printf("%c", T->data);
		PreOrder(T->lc);
		PreOrder(T->rc);
	}
}

void InOrder(BiTNode *T)
{
	if (T != NULL)
	{
		InOrder(T->lc);
		printf("%c", T->data);
		InOrder(T->rc);
	}
}

void BakOrder(BiTNode *T)
{
	if (T != NULL)
	{
		BakOrder(T->lc);
		BakOrder(T->rc);
		printf("%c", T->data);
	}
}

//先序遍历创建二叉树
BiTNode* PreCreate(char* x)
{
	static int i = 0;
	BiTNode *newnode = NULL;
	if (x[i] != '#' && x[i] != '\0')
	{
		newnode = malloc(sizeof(BiTNode));
		newnode->data = (int)(x[i]);
		i++;
		newnode->lc = PreCreate(x);
		newnode->rc = PreCreate(x);
		return newnode;
	}
	i++;
	return newnode;
}

//先序遍历创建二叉树(循环输入)
BiTNode* BiTree_Creat12()
{
	BiTNode *tmp = NULL;
	char ch;
	printf("\n请输入字符: ");
	scanf("%c", &ch);
	if (ch == ' ')
	{
		return NULL;
	}
	else
	{
		tmp = (BiTNode *)malloc(sizeof(BiTNode));
		if (tmp == NULL)
		{
			return NULL;
		}
		tmp->data = ch; //生成根结点
		tmp->lc = BiTree_Creat12();//构建左子树
		tmp->rc = BiTree_Creat12();//构建右子数
		return tmp;
	}
}

void main()
{
	/* TreeModel:
	PreOrder:ABDHKECFIGJ
	 InOrder:HKDBEAIFCGJ
	BakOrder:KHDEBIFJGCA
	*/
	char PreOrderList[100] = "ABDH#K###E##CFI###G#J##";
	BiTNode *Tree = NULL;
	Tree = PreCreate(PreOrderList);

	printf("PreOrder:");
	PreOrder(Tree);
	printf("\n");
	printf(" InOrder:");
	InOrder(Tree);
	printf("\n");
	printf("BakOrder:");
	BakOrder(Tree);
	printf("\n");

	system("pause");
}