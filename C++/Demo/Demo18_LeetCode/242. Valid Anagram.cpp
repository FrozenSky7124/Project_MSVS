#include <iostream>

using namespace std;

class Solution {
public:
	bool isAnagram(string s, string t) {
		int sLen = s.size();
		int tLen = t.size();
		if (sLen != tLen) return false;
		char * ps = (char *)s.c_str();
		char * pt = (char *)t.c_str();
		for (int i = 0; i < sLen; i++)
		{
			bool flag = false;
			for (int j = i; j < tLen; j++)
			{
				if (pt[i] == pt[j])
				{
					char temp = pt[i];
					pt[i] = pt[j];
					pt[j] = temp;
					flag = true;
					break;
				}
			}
			if (flag == true)
				continue;
			else
				return false;
		}
		return true;
	}
};