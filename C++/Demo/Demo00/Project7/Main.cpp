#include <iostream>
#include "String.h"

using namespace std;

int main()
{
	char *c1 = "abcdeg";
	char *c2 = "abcdef";
	String _s1(c1), _s2(c2);
	if (_s1 == _s2)
	{
		cout << "s1 == s2" << endl;
	}
	if (_s1 > _s2)
	{
		cout << "s1 > s2" << endl;

	}
	if (_s1 < _s2)
	{
		cout << "s1 < s2" << endl;

	}
	_s1.show();
	_s2.show();
	system("pause");
	return 0;
}