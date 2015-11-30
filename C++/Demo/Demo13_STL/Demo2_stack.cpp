#include "iostream"
#include "stack"
using namespace std;


void main21()
{
	//����һ��ջ
	stack<int> s;
	//ջ��ֵ
	for (int i = 0; i < 5; i++)
	{
		//��ջ�з�Ԫ��
		s.push(i + 1);
	}

	//ջ����
	while (!s.empty())
	{
		//��ȡջ��Ԫ��
		int tmp = s.top();
		//����ջ��Ԫ��
		s.pop();
		printf("%d \n", tmp);
	}
	system("pause");
}


struct Teacher
{
	int age;
	char name[10];
};

void printfStack(stack<Teacher> &s)
{
	//ջ����
	while (!s.empty())
	{
		//��ȡջ��Ԫ��
		cout << s.top().age << endl;
		//����ջ��Ԫ��
		s.pop();
	}
}
void main()
{
	//����һ��ջ
	Teacher t1, t2, t3;
	t1.age = 11;
	t2.age = 22;
	t3.age = 33;
	stack<Teacher> s;
	s.push(t1);
	s.push(t2);
	s.push(t3);

	printfStack(s);


	system("pause");
}
