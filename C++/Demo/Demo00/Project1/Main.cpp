#include <iostream>
#include <Windows.h>
#include <ctime>

using namespace std;

void main()
{
	int power = 0;
	int x = 0;
	cout << "捕获到一只比卡丘，POWER LEVEL未知（1-1000）。猜一猜~" << endl;
	srand((time_t)time(NULL)); //以当前时间为随机数种子
	power = rand() % 1000 + 1;
	cin >> x;
	while (x != power)
	{
		if (x > power)
		{
			cout << "A little High." << endl;
			cin >> x;
		}
		if (x < power)
		{
			cout << "A little Low." << endl;
			cin >> x;
		}
	}
	cout << "捕获成功！" << endl;
	system("pause");
}