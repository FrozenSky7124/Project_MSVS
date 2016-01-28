/*
Function: Create random number and not repeat.
*/
#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

int main()
{
	vector<int> vec;
	srand(time(NULL));

	//Create
	while (true)
	{
		int idx = rand() % 60;
		auto itr = vec.begin();
		for (;itr != vec.end();++itr)
		{
			if (idx == *itr)
			{
				break;
			}
		}
		if(itr==vec.end())
		{
			vec.push_back(idx);
			if(vec.size()==7)
			{
				break;
			}
		}

	}

	//Output
	auto itr = vec.begin();
	for (;itr != vec.end();++itr)
	{
		cout << *itr << " ";
	}
	cout << endl;
	system("pause");
	return 0;
}