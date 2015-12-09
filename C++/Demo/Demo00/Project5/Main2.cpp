#include <iostream>

using namespace std;

void main()
{
	int a, b;
	int temp;
	cin >> a;
	cin >> b;
	cout << a << " # " << b << "= ";
	temp = a > b ? a : b;
	while ((temp%a != 0) || (temp%b != 0))
	{
		temp++;
	}
	cout << temp << endl;
	system("pause");
}