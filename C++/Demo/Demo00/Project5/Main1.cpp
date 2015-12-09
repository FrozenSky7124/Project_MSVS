#include <iostream>

using namespace std;

void main()
{
	int a = 0, b = 0;
	int temp = 0;
	cin >> a;
	cin >> b;
	cout << a << " X " << b << " = ";
	while (a%b!=0)
	{
		temp = b;
		b = a%b;
		a = temp;
	}
	cout << b << endl;
	system("pause");
}