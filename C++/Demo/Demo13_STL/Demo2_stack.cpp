#include "iostream"
#include "stack"
using namespace std;


void main21()
{
	//定义一个栈
	stack<int> s;
	//栈赋值
	for (int i = 0; i < 5; i++)
	{
		//往栈中放元素
		s.push(i + 1);
	}

	//栈遍历
	while (!s.empty())
	{
		//获取栈顶元素
		int tmp = s.top();
		//弹出栈顶元素
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
	//栈遍历
	while (!s.empty())
	{
		//获取栈顶元素
		cout << s.top().age << endl;
		//弹出栈顶元素
		s.pop();
	}
}
void main()
{
	//定义一个栈
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
