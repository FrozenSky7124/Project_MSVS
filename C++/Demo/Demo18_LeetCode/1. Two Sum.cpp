/*

1. Two Sum

Given an array of integers, return indices of the two numbers such that they add up to a specific target.
You may assume that each input would have exactly one solution.

Example:

Given nums = [2, 7, 11, 15], target = 9,
Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].

UPDATE (2016/2/13):
The return format had been changed to zero-based indices. Please read the above updated description carefully.

*/

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		vector<int> result;
		int n = nums.size();
		int a = 0;
		int b = 0;
		for (a = 0; a < n - 1; a++)
		{
			for (b = a + 1; b < n; b++)
			{
				if (nums[a] + nums[b] == target)
				{
					result.push_back(a);
					result.push_back(b);
					break;
				}
			}
			if (result.size() == 2)
				break;
		}
		return result;
	}
};

class Solution1 {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		vector<int> result;
		unordered_map<int, int> hash;
		for (int i = 0; i < nums.size(); i++)
		{
			int numberToFind = target - nums[i];
			if (hash.find(numberToFind) != hash.end())
			{
				result.push_back(hash[numberToFind]);
				result.push_back(i);
			}
			hash[nums[i]] = i;
		}
		return result;
	}
};

int main()
{
	Solution1 sol;
	vector<int> test = {5, 1, 3, 0, 11, 7};
	vector<int> result;
	int target = 7;
	result = sol.twoSum(test, target);
	cout << result.at(0) << " " << result.at(1) << endl;
	system("pause");
	return 0;
}