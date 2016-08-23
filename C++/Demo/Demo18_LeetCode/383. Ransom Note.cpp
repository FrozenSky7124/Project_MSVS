#include <iostream>

using namespace std;

class Solution {
public:
	bool canConstruct(string ransomNote, string magazine) {
		char *pRansom = (char *)ransomNote.c_str();
		char *pMagaze = (char *)magazine.c_str();
		int lenRansom = strlen(pRansom);
		int lenMagaze = strlen(pMagaze);
		for (int i = 0; i < lenRansom; i++)
		{
			bool flag = false;
			for (int j = i; j < lenMagaze; j++)
			{
				if (pMagaze[j] == pRansom[i])
				{
					char tempChar = pMagaze[i];
					pMagaze[i] = pMagaze[j];
					pMagaze[j] = tempChar;
					flag = true;
					break;
				}
			}
			if (flag == true)
				continue;
			else
				return false;
			
		}
		//ËÑË÷½áÊø
		return true;
	}
};