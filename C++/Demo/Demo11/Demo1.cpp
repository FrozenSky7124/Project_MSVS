#include <iostream>
#include "MyPoint.h"
#include "MyCircle.h"

using namespace std;

int main()
{
	MyPoint mypoint;
	MyCircle mycircle;
	mypoint.setPoint(100.0, 100.0);
	mycircle.setCircle(100.0, 100.0, 10.0);
	cout << mycircle.judge(mypoint) << endl;
	system("pause");
	return 0;
}