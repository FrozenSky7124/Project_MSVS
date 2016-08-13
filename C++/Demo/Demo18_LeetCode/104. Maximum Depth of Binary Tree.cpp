/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
	int maxDepth(TreeNode* root) {
		if (root == NULL)
		{
			return 0;
		}
		int leftDepth = 0;
		int rightDepth = 0;
		if (root->left == NULL)
		{
			leftDepth = 0;
		}
		else
		{
			leftDepth = maxDepth(root->left);
		}
		if (root->right == NULL)
		{
			rightDepth = 0;
		}
		else
		{
			rightDepth = maxDepth(root->right);
		}
		return (leftDepth > rightDepth) ? (leftDepth + 1) : (rightDepth + 1);
	}
};
