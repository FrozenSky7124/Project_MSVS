#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
	int singleNumber(vector<int>& nums) {
		int sz = nums.size();
		if (sz < 1) return 0;
		if (sz == 1) return nums[0];

		/*============================================================================================
		V 1.1 : use bit ^ to see if A^A=0, A^B^A=B, find the single one.
		Time: O(n),    space: O(1)
		*///==========================================================================================
		int result = 0;
		for (int i = 0; i < sz; i++) {
			result ^= nums[i]; // A^B^A=B to get single one.
		}
		return result;

		/*============================================================================================
		V 1.0 : sort the vector, see if(nums[i] != nums[i+1]) to find the single one.
		Time: O(2n),    space: O(1)     BUG: should use O(n) time, not sort().
		*///==========================================================================================
		/*        sort(nums.begin(), nums.end()); // this is O(nlogn), not right, thanks @kohli fix it.
		for(int i = 0; i < sz; i += 2){
		if(nums[i] != nums[i+1]) return nums[i];
		}
		return 0; // in case of NO single one.
		*/
	}
};