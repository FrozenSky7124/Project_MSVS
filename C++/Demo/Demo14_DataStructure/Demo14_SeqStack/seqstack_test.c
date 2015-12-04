#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "seqstack.h"

void main()
{
	int i = 0, tmp;
	int a[10];
	SeqStack* stack = NULL;

	stack = SeqStack_Create(10);

	for (i = 0; i<10; i++)
	{
		a[i] = i + 1;
		SeqStack_Push(stack, (void *)&a[i]);
		//SeqStack_Push(stack, (void *)(a + i) );
	}

	tmp = *((int *)SeqStack_Top(stack));
	printf("top element:%d \n", tmp);
	printf("stack len:%d \n", SeqStack_Size(stack));
	printf("stack captical:%d \n", SeqStack_Capacity(stack));

	while (SeqStack_Size(stack) > 0)
	{
		tmp = *((int *)SeqStack_Pop(stack));
		printf("%d \n", tmp);
	}

	SeqStack_Destroy(stack);

	system("pause");
	return;
}