#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>

using namespace std;

class Solution1 {
public:
	string reverseString(string s) {
		string outStr;
		for (int i = s.length() - 1; i >= 0; i--)
		{
			outStr += s.at(i);
		}
		outStr += '\0';
		return outStr;
	}
};

class Solution {
public:
	string reverseString(string s) {
		char tmp;
		char *c = new char[s.length() + 1];
		strcpy(c, s.c_str());
		int x = 0;
		int y = s.length() - 1;
		while (x < y)
		{
			tmp = c[x];
			c[x] = c[y];
			c[y] = tmp;
			x++;
			y--;
		}
		string outStr(c);
		delete[] c;
		return outStr;
	}
};


int main()
{
	Solution sol;
	string test = "hello";
	cout << sol.reverseString(test).c_str();
	system("pause");
	return 0;
}