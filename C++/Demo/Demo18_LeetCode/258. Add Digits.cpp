#include <iostream>

using namespace std;

class Solution {
public:
	int addDigits(int num) {
		while (num >= 10)
		{
			int sum = 0;
			while (num / 10 != 0)
			{
				sum += num % 10;
				num = num / 10;
			}
			sum += num;
			num = sum;
		}
		return num;
	}
};

int main()
{
	Solution sol;
	cout << sol.addDigits(1111111);
	system("pause");
	return 0;
}