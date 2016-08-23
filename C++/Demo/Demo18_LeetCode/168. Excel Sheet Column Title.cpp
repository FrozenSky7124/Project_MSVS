#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
	string convertToTitle(int n) {
		vector<int> arr;
		intTo26(n, arr);
		char *arrChar = new char[arr.size() + 1];
		memset(arrChar, 0, sizeof(char) * (arr.size() + 1));
		int x = 0;
		for (int i = arr.size() - 1; i >= 0; i--)
		{
			arrChar[x++] = (char)(arr[i] + 'A');
		}
		return string(arrChar);
	}
	void intTo26(int m, vector<int> & arr) {
		m = m - 1;
		int consult = m / 26;
		int remain = m % 26;
		arr.push_back(remain);
		if (consult != 0)
			intTo26(consult, arr);
	}
};

void main()
{
	Solution sol;
	cout << sol.convertToTitle(26).c_str();
	system("pause");
	return;
}