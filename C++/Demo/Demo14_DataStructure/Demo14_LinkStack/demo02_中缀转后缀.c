/*
�㷨˼·
�ӵ�һ���ַ���ʼɨ��
��������ͨ�ַ�ʱ���ԣ������������ʱѹ��ջ��
�������ҷ���ʱ��ջ�е���ջ�����ţ�������ƥ��
ƥ��ɹ�������������һ���ַ�
ƥ��ʧ�ܣ�����ֹͣ��������
������
�ɹ�: �����ַ�ɨ����ϣ���ջΪ��
ʧ�ܣ�ƥ��ʧ�ܻ������ַ�ɨ����ϵ�ջ�ǿ�
*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "linkstack.h"

int isNumber(char c)
{
	return ('0' <= c) && (c <= '9');
}

int isOperator(char c)
{
	return (c == '+') || (c == '-') || (c == '*') || (c == '/');
}

int isLeft(char c)
{
	return (c == '(');
}

int isRight(char c)
{
	return (c == ')');
}

int priority(char c)
{
	int ret = 0;

	if( (c == '+') || (c == '-') )
	{
		ret = 1;
	}

	if( (c == '*') || (c == '/') )
	{
		ret = 2;
	}

	return ret;
}

void output(char c)
{
	if( c != '\0' )
	{
		printf("%c", c);
	}
}

void transform(const char* exp)
{
	int i = 0;
	LinkStack* stack = LinkStack_Create();

	while( exp[i] != '\0' )
	{
		if( isNumber(exp[i]) )
		{
			output(exp[i]);
		}
		else if( isOperator(exp[i]) )
		{
			while( priority(exp[i]) <= priority((char)(int)LinkStack_Top(stack)) )
			{
				output((char)(int)LinkStack_Pop(stack));
			}

			LinkStack_Push(stack, (void*)(int)exp[i]);
		} 
		else if( isLeft(exp[i]) )
		{
			LinkStack_Push(stack, (void*)(int)exp[i]);
		} 
		else if( isRight(exp[i]) )
		{
			//char c = '\0';
			while( !isLeft((char)(int)LinkStack_Top(stack)) )
			{
				output((char)(int)LinkStack_Pop(stack));
			}

			LinkStack_Pop(stack);
		}
		else
		{
			printf("Invalid expression!");
			break;
		}

		i++;
	}

	while( (LinkStack_Size(stack) > 0) && (exp[i] == '\0') )
	{
		output((char)(int)LinkStack_Pop(stack));
	}

	LinkStack_Destroy(stack);
}

int main()
{
	transform("8+(3-1)*5");

	printf("\n");
	system("pause");
	return 0;
}
