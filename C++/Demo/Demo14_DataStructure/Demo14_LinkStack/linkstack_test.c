#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "linkstack.h"

void main()
{
	int ret = 0;
	int i=0, a[10], tmp = 0;
	LinkStack* stack = NULL;
	stack = LinkStack_Create();

	for (i=0; i<2; i++)
	{
		a[i] = i+1;
		ret = LinkStack_Push(stack, (void *)&a[i]);
	}

	tmp = * ((int *)LinkStack_Top(stack) );
	printf("zhanÕ»¶¥£º%d\n", tmp);
	printf("length:%d \n", LinkStack_Size(stack));

	while(LinkStack_Size(stack) > 0)
	{
		tmp =  *((int *)LinkStack_Pop(stack) );
		printf("tmp:%d \n", tmp);
	}

	LinkStack_Destroy( stack);

	system("pause");
	return ;
}