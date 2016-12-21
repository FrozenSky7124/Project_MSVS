#include <iostream>
#include <ctime>

using namespace std;

class Solution1 {
public:
	int hammingDistance(int x, int y) {
		return __builtin_popcount(x^y);
	}
};

class Solution2 {
public:
	int hammingDistance(int x, int y) {
		int res = 0;
		for (int i = 0; i < 32; ++i) {
			if ((x & (1 << i)) ^ (y & (1 << i))) {
				++res;
			}
		}
		return res;
	}
};

int main()
{
	return 0;
}