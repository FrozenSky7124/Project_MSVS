//SevenQueensProblem 七女皇问题

#include <iostream>
#include <math.h>

using namespace std;

#define ARR_SIZE 10

//输出女皇矩阵
int printQueensArr(int *queensArr, int nLen)
{
	//cout << "QueenArray:" << endl;
	cout << endl;
	for (int i = 0; i < nLen; i++)
	{
		if (-1 == queensArr[i])
		{
			for (int n = 0; n < nLen; n++)
				cout << "○";
			cout << endl;
		}
		else
		{
			for (int n = 0; n < queensArr[i]; n++)
				cout << "○";
			cout << "●";
			for (int n = queensArr[i] + 1; n < nLen; n++)
				cout << "○";
			cout << endl;
		}
	}
	cout << endl;
	return 0;
}

//追溯法求解
int putQueen(int* queensArr, int nLen, int nRow, int & nSolution)
{	
	for (queensArr[nRow] = 0; queensArr[nRow] < nLen; queensArr[nRow]++)
	{
		bool isClash = false; //判断女皇位置是否产生冲突
		for (int iRow = 0; iRow < nRow; iRow++)
		{
			if (queensArr[nRow] == queensArr[iRow])
			{
				isClash = true;
				break;
			}
			if (abs((queensArr[nRow] - queensArr[iRow])) == abs(nRow - iRow))
			{
				isClash = true;
				break;
			}
		}
		if (!isClash)
		{
			if (nRow == nLen - 1)
			{
				nSolution++;
				cout << "Solution: " << nSolution << endl;
				printQueensArr(queensArr, nLen);
			}
			else
			{
				putQueen(queensArr, nLen, nRow + 1, nSolution);
			}
		}
	}
	return 0;
}

int main()
{
	int nSolution = 0;
	int queensArr[ARR_SIZE] = { 0 };
	putQueen(queensArr, ARR_SIZE, 0, nSolution);
	system("pause");
	return 0;
}

