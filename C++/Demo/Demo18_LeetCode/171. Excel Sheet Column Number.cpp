#include <iostream>
#include <cmath>

using namespace std;

class Solution {
public:
	int titleToNumber(string s) {
		int ans = 0;
		int len = s.size();
		for (int i = 0; i < len; i++)
		{
			ans += (((int)(s.at(i)) - 65 + 1) * pow(26, len - i - 1));
		}
		return ans;
	}
};

void main()
{
	Solution sol;
	cout << "AB " << sol.titleToNumber("AB") << endl;
	cout << "A  " << sol.titleToNumber("A") << endl;
	cout << "Z  " << sol.titleToNumber("Z") << endl;
	system("pause");
	return;
}