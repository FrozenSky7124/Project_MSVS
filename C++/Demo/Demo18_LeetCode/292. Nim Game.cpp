#include <iostream>

using namespace std;

class Solution1 {
public:
	bool takeStone(int remain, int time)
	{
		if (remain <= 3 && time % 2 == 0)
		{
			return true;
		}
		else if (remain <= 3 && time % 2 == 1)
		{
			return false;
		}
		else
		{
			time++;
			return takeStone(remain - 1, time);
			return takeStone(remain - 2, time);
			return takeStone(remain - 3, time);
		}
	}
	bool canWinNim(int n) {
		return takeStone(n, 0);
	}
};

class Solution {
public:
	bool canWinNim(int n) {
		return (n % 4 != 0);
	}
};

int main()
{
	Solution sol;
	if (sol.canWinNim(64))
		cout << "true" << endl;
	else
		cout << "false" << endl;
	system("pause");
	return 0;
}