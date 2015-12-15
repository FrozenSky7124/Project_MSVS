#include <iostream>
#include "String.h"

using namespace std;

String::String(char *sin)
{
	int length = 0;
	if (sin == NULL)
	{
		return;
	}
	length = strlen(sin);
	cout << length << endl;
	this->s = new char[length + 1];
	memset(s, 0, length + 1);
	memcpy(s, sin, length);
}

String::~String()
{
	if (this->s != NULL)
	{
		delete[] this->s;
	}
}

bool operator==(String &a, String &b)
{
	int ret;
	ret = strcmp(a.s, b.s);
	if (ret == 0) return true;
	return false;
}

bool operator>(String &a, String &b)
{
	int ret;
	ret = strcmp(a.s, b.s);
	if (ret > 0) return true;
	return false;
}

bool operator<(String &a, String &b)
{
	int ret;
	ret = strcmp(a.s, b.s);
	if (ret < 0) return true;
	return false;
}

void String::show()
{
	cout << s << endl;
}