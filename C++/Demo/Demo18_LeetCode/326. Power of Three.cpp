#include <iostream>

using namespace std;

class Solution {
public:
	bool isPowerOfThree(int n) {
		if (n < 1)
			return false;
		while (n % 3 == 0)
		{
			n /= 3;
		}
		if (n == 1)
		{
			return true;
		}
		return false;
	}
};

class Solution1 {
public:
	bool isPowerOfThree(int n) {
		return n > 0 && 1162261467 % n == 0;
	}
};