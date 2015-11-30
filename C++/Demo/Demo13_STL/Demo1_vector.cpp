#include <iostream>
#include "vector"
using namespace std;

void printVector(vector<int> &x)
{
	int size = x.size();
	for (int i = 0;i < size;i++)
	{
		cout << "[" << i << "]=" << x[i] << " ";
	}
	cout << endl;
}
void main()
{
	vector<int> v1(5);
	for (int i = 0;i < 5;i++)
	{
		v1[i] = i;
	}
	printVector(v1);

	vector<int> v2(10);
	//v2 = v1;
	printVector(v2);
	system("pause");
}