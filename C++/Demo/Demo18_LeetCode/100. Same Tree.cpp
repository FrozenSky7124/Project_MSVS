#include <iostream>

using namespace std;

/**
* Definition for a binary tree node.
*/
struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
	bool isSameTree(TreeNode* p, TreeNode* q) {
		if (p == NULL && q == NULL)
			return true;
		else if (p != NULL && q != NULL)
		{
			if (p->val != q->val)
				return false;
			else
			{
				if (isSameTree(p->left, q->left))
				{
					return isSameTree(p->right, q->right);
				}
				else
				{
					return false;
				}
			}
		}
		else
			return false;
		return true;
	}
};