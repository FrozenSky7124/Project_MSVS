#include <iostream>
#include <math.h>

using namespace std;

class Solution1 {
public:
	int findComplement(int num) {
		int i = 0;
		for (i = 0; i < 32; i++)
		{
			if ((num > pow(2, i) - 1) && (num <= pow(2, i + 1) - 1))
				break;
		}
		return (pow(2, i + 1) - 1 - num);
	}
};

class Solution2 {
public:
	int findComplement(int num) {
		unsigned mask = ~0;
		while (num & mask) mask <<= 1;
		return ~mask & ~num;
	}
};

int main()
{
	Solution1 sol1;
	Solution2 sol2;
	cout << sol1.findComplement(3) << endl;
	cout << sol2.findComplement(3) << endl;
	system("pause");
	return 0;
}