#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
	void moveZeroes(vector<int>& nums) {
		int lastNoZero = 0;
		int size = nums.size();
		for (int curN = 0; curN < size; curN++)
		{
			if (nums[curN] != 0)
			{
				int tmp = nums[lastNoZero];
				nums[lastNoZero] = nums[curN];
				lastNoZero++;
				nums[curN] = tmp;
			}
		}
	}
};

int main()
{
	vector<int> test = { 0, 0, 2, 1, 3, 1, 0, 0, 2, 4, 5, 0};
	Solution sol;
	sol.moveZeroes(test);
	for (int i = 0; i < test.size(); i++)
	{
		cout << test[i] << " ";
	}
	cout << endl;
	system("pause");
}