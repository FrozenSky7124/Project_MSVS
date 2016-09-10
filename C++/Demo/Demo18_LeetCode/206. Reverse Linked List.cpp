#include <iostream>

using namespace std;

struct ListNode {
	int val;
	ListNode *next;
	ListNode(int x) : val(x), next(NULL) {}
};

/**
* Definition for singly-linked list.
* struct ListNode {
*     int val;
*     ListNode *next;
*     ListNode(int x) : val(x), next(NULL) {}
* };
*/

class Solution {
public:
	ListNode* reverseList(ListNode* head) {
		if (head == NULL)
			return NULL;
		if (head->next == NULL)
			return head;
		ListNode* pre = NULL;
		ListNode* cur = head;
		while (cur->next != NULL)
		{
			head = cur->next;
			cur->next = pre;
			pre = cur;
			cur = head;
		}
		cur->next = pre;
		head = cur;
		return head;
	}
};

int main()
{
	return 0;
}