#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
void main2()
{
	double a, b, c;
	scanf("%lf%lf%lf", &a, &b, &c);
	//ÅÅÐòa>b>c
	if (a < b)
	{
		double temp = a;
		a = b;
		b = temp;
	}
	if (a < c)
	{
		double temp = a;
		a = c;
		c = temp;
	}
	if (b < c)
	{
		double temp = b;
		b = c;
		c = temp;
	}
	printf("a=%f,b=%f,c=%f", a, b, c);
	system("pause");
}