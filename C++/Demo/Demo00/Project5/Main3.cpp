#include <iostream>

using namespace std;

int feib(int i)
{
	static int num = 0;
	if (i == 1 || i == 2)
	{
		return 1;
	}
	else
	{
		num++;
		cout << "Ext: Feib-" << i - 1 << " Feib-" << i - 2 << "  Num:" << num << endl;
		return feib(i - 1) + feib(i - 2);
	}	
}
void main()
{
	int n;
	cin >> n;
	while (n!=-1)
	{
		cout << feib(n) << endl;
		cin >> n;
	}
	system("pause");
}