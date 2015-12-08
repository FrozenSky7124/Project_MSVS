#include <iostream>
using namespace std;

void main()
{
	int i = 0, j = 0, k = 0;
	for (i = 1;i < 10;i++)
	{
		for (int p = 1;p < i;p++)
		{
			printf("       ");
		}
		for (j = i;j < 10;j++)
		{
			k = i*j;
			printf("%d*%d=%2d ", i, j, k);
		}
		printf("\n");
	}
	system("pause");
}