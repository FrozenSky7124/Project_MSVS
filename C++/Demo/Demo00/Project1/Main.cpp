#include <iostream>
#include <Windows.h>
#include <ctime>

using namespace std;

void main()
{
	int power = 0;
	int x = 0;
	cout << "����һֻ�ȿ���POWER LEVELδ֪��1-1000������һ��~" << endl;
	srand((time_t)time(NULL)); //�Ե�ǰʱ��Ϊ���������
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
	cout << "����ɹ���" << endl;
	system("pause");
}