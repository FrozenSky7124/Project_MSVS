#include <iostream>

using namespace std;

class Solution {
public:
	int firstUniqChar(string s) {
		int sLen = s.length();
		for (int i = 0; i < sLen; i++)
		{
			bool flag = false;
			for (int j = 0; j < sLen; j++)
			{
				if (s[i] == s[j] && i != j)
				{
					flag = true;
					break;
				}
			}
			if (flag == false)
				return i;
			else
				continue;
		}
		return -1;
	}
};

class Solution1 {
public:
	int firstUniqChar(string s) {
		int alphabet[26] = { 0 };
		for (int i = 0; i < s.size(); ++i) { ++alphabet[s[i] - 'a']; }
		int i = 0;
		while (i < s.size() && alphabet[s[i] - 'a'] > 1) ++i;
		return i == s.size() ? -1 : i;
	}
};