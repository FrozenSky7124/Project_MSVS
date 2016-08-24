#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
	int majorityElement(vector<int>& nums) {
		int major = nums[0];
		int count = 1;
		for (int i = 1; i < nums.size(); i++)
		{
			if (count == 0)
			{
				major = nums[i];
				count++;
			}
			else if (nums[i] == major)
			{
				count++;
			}
			else
			{
				count--;
			}
		}
		return major;
	}
};