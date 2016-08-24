#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//ʹ��sort�㷨������������ж�
class Solution1 {
public:
	bool containsDuplicate(vector<int>& nums)
	{
		if (nums.empty())
			return false;
		sort(nums.begin(), nums.end());
		for (int i = 0; i < nums.size() - 1; i++)
		{
			if (nums[i] == nums[i + 1])
				return true;
		}
		return false;
	}
};

//ʹ���Խ�bool��
class Solution {
public:
	bool containsDuplicate(vector<int>& nums)
	{
		int iMin = INT_MAX;
		int iMax = INT_MIN;
		for (int i = 0; i < nums.size(); i++)
		{
			if (iMin > nums[i])
				iMin = nums[i];
			if (iMax < nums[i])
				iMax = nums[i];
		}
		bool *boolList = new bool[iMax - iMin + 1];
		memset(boolList, 0, sizeof(bool) * (iMax - iMin + 1));
		for (int i = 0; i < nums.size(); i++)
		{
			if (boolList[nums[i] - iMin])
			{
				delete[] boolList;
				return true;
			}
			else
				boolList[nums[i] - iMin] = true;
		}
		delete[] boolList;
		return false;
	}
};