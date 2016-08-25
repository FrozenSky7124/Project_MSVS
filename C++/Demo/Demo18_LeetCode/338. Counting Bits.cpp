#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
	vector<int> countBits(int num) {
		vector<int> ans;
		ans.push_back(0);
		int index = 0;
		for (int i = 1; i <= num; i++)
		{
			if (i > (pow(2, index + 1) - 1))
				index++;
			ans.push_back(ans[i - pow(2, index)] + 1);
		}
		return ans;
	}
};

void main()
{
	vector<int> result;
	Solution sol;
	result = sol.countBits(16);
	int i = 0;
	while (i < result.size())
	{
		cout << result[i] << endl;
		i++;
	}
	system("pause");
	return;
}