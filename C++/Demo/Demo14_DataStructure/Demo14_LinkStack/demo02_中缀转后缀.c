/*
算法思路
从第一个字符开始扫描
当遇见普通字符时忽略，当遇见左符号时压入栈中
当遇见右符号时从栈中弹出栈顶符号，并进行匹配
匹配成功：继续读入下一个字符
匹配失败：立即停止，并报错
结束：
成功: 所有字符扫描完毕，且栈为空
失败：匹配失败或所有字符扫描完毕但栈非空
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
