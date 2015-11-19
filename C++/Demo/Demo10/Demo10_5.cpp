#include <iostream>

using namespace std;

int main()
{
	bool bA = true;
	printf("True:ba=%d\n", bA);
	bA = false;
	printf("False:ba=%d\n", bA);
	bA = 1024;
	printf("1024:ba=%d\n", bA);
	bA = 0;
	printf("0:ba=%d\n", bA);
	bA = -1;
	printf("-1:ba=%d\n", bA);
	system("pause");
	return 0;
}