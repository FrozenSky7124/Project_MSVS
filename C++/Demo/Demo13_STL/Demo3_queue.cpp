#include "iostream"
#include "queue"
using namespace std;

void main01()
{
	//��һ������
	queue<int> q;
	//x����������һ��Ԫ�� 
	for (int i = 0; i < 5; i++)
	{
		q.push(i);
	}
	while (!q.empty())
	{
		//��ȡ����ͷԪ��
		int tmp = q.front();
		cout << tmp << endl;
		//��������Ԫ��
		q.pop();
	}
	system("pause");
}


struct Teacher
{
	int age;
	char name[10];
};

void printfFont(queue<Teacher *> &q)
{
	while (!q.empty())
	{
		//��ȡ����ͷԪ��
		Teacher * tmp = q.front();
		cout << tmp->age << endl;
		//��������Ԫ��
		q.pop();
	}
}
void main()
{
	//��һ������
	queue<Teacher *> q;
	Teacher t1, t2, t3;
	t1.age = 11;
	t2.age = 22;
	t3.age = 33;

	//x����������һ��Ԫ�� 
	q.push(&t1);
	q.push(&t2);
	q.push(&t3);

	printfFont(q);

	// 	while(!q.empty())
	// 	{
	// 		//��ȡ����ͷԪ��
	// 		Teacher * tmp = q.front();
	// 		cout<<tmp->age<<endl;
	// 		//��������Ԫ��
	// 		q.pop();
	// 	}
	system("pause");
}