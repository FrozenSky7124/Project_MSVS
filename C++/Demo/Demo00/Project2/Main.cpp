#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <direct.h>
#include <math.h>

using namespace std;

int writeToFile(FILE *p, int n)
{
	if (p == NULL)
	{
		return -1;
	}
	fprintf(p, "%d\t", n);
	return 0;
}

int main()
{
	int N = 0;
	int q = 0;
	int flag = 0;
	char filepath[200];
	_getcwd(filepath, 200); //获取当前工作目录
	strcat(filepath, "\\Output.txt");
	FILE *fp = NULL;
	fp = fopen(filepath, "w");
	cout << "寻找2-N的所有素数..." << endl;
	cin >> N;
	for (int i = 2;i <= N;i++)
	{
		flag = 0;
		q = (int)sqrt(i);
		for (int j = 2;j <= q;j++)
		{
			if (i%j == 0)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 0)
		{
			writeToFile(fp, i);
		}
	}
	fclose(fp);
	cout << "完成！数据写入：" << filepath << endl;
	system("pause");
	return 0;
}