#include <iostream>

using namespace std;

class Solution {
public:
	bool isPowerOfTwo(int n) {
		return n > 0 && 1073741824 % n == 0;
	}
};