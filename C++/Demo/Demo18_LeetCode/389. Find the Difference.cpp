#include <iostream>

using namespace std;

class Solution {
public:
	char findTheDifference(string s, string t) {
		int sLen = s.length();
		int result = 0;
		for (int i = 0; i < sLen; i++)
		{
			result ^= s[i] ^ t[i];
		}
		return result ^ t.back();
	}
};