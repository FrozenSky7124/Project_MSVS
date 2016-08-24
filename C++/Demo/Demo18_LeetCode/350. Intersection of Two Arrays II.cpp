#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//FrozenSky's stupid but accepted idea.
class Solution1 {
public:
	vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
		vector<int> ans;
		int len1 = nums1.size();
		int len2 = nums2.size();
		sort(nums1.begin(), nums1.end());
		sort(nums2.begin(), nums2.end());
		for (int i = 0; i < len1; i++)
		{
			for (int j = 0; j < len2; j++)
			{
				if (nums1[i] == nums2[j])
				{
					int temp = nums1[i];
					int count1 = 1;
					int count2 = 1;
					for (int n1 = i + 1; n1 < len1; n1++)
					{
						if (nums1[n1] == temp)
							count1++;
					}
					i = count1 + i - 1;
					for (int n2 = j + 1; n2 < len2; n2++)
					{
						if (nums2[n2] == temp)
							count2++;
					}
					j = count2 + j - 1;
					if (count1 <= count2)
					{
						while (count1-- > 0)
						{
							ans.push_back(temp);
						}
					}
					else
					{
						while (count2-- > 0)
						{
							ans.push_back(temp);
						}
					}
				}
			}
		}
		return ans;
	}
};

//8ms Use sort vector.
class Solution {
public:
	vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
		vector<int> ans;
		sort(nums1.begin(), nums1.end());
		sort(nums2.begin(), nums2.end());
		auto iter1 = nums1.begin();
		auto iter2 = nums2.begin();
		while (iter1 != nums1.end() && iter2 != nums2.end())
		{
			while (*iter1 == *iter2 && iter1 != nums1.end() && iter2 != nums2.end())
			{
				ans.push_back(*iter1);
				iter1++;
				iter2++;
			}
			while (iter1 != nums1.end() && *iter1 < *iter2) ++iter1;
			while (iter2 != nums2.end() && *iter1 > *iter2) ++iter2;
		}
		return ans;
	}
};

void main()
{
	vector<int> a = { 1, 2, 2, 1 };
	vector<int> b = { 1, 2, 2 };
	Solution sol;
	vector<int> c = sol.intersect(a, b);
	for (int i = 0; i < c.size(); i++)
	{
		cout << c[i] << " ";
	}
	system("pause");
	return;
}