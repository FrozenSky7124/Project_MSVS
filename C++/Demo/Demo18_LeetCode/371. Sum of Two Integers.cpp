#include <iostream>

using namespace std;

class Solution {
public:
	int getSum(int a, int b) 
	{
		int ans = a ^ b;
		int carry = a & b;
		while (carry != 0)
		{
			carry <<= 1;
			int ans_tmp = ans ^ carry;
			carry = ans & carry;
			ans = ans_tmp;
		}
		return ans;
	}
};

int main()
{
	Solution sol;
	cout << sol.getSum(2111111, -2111111);
	system("pause");
	return 0;
}