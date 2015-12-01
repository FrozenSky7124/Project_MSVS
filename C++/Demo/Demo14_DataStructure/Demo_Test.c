#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "seqlist.h"


typedef struct _Teacher
{
	char name[64];
	int age;
	int buf;
}Teacher;

void main()
{
	int ret = 0, i = 0;
	Teacher t1, t2, t3;
	SeqList* list = NULL;

	t1.age = 10;
	t2.age = 20;
	t3.age = 30;

	list = SeqList_Create(10);

	
	ret = SeqList_Insert(list, (SeqListNode*)&t1, 0);
	ret = SeqList_Insert(list, (SeqListNode*)&t2, 0);
	ret = SeqList_Insert(list, (SeqListNode*)&t3, 0);
	

	//Ñ­»·±éÀú
	for (i = 0; i < SeqList_Length(list); i++)
	{
		Teacher *tmp = (Teacher *)SeqList_Get(list, i);
		printf("age:%d \n", tmp->age);
	}

	//Ñ­»·É¾³ı
	for (i = 0; i < SeqList_Length(list); i++)
	{
		SeqList_Delete(list, 0);
	}
	SeqList_Destroy(list);
	system("pause");
}