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
	_getcwd(filepath, 200); //��ȡ��ǰ����Ŀ¼
	strcat(filepath, "\\Output.txt");
	FILE *fp = NULL;
	fp = fopen(filepath, "w");
	cout << "Ѱ��2-N����������..." << endl;
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
	cout << "��ɣ�����д�룺" << filepath << endl;
	system("pause");
	return 0;
}