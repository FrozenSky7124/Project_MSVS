#include <iostream>
#include <iomanip>
using namespace std;
enum color {
	red, yellow, blue, white, black,
};
void main()
{
	color c;
	int i, j, k;
	int n = 0;
	int p = 0;
	for (i = red;i <= black;i++)
	{
		for (j = red;j <= black;j++)
		{
			if (j != i)
			{
				for (k = red;k <= black;k++)
				{
					if (k != i && k != j)
					{
						n++;
						cout << setw(3) << n << " ";
						for (p = 1;p <= 3;p++)
						{
							switch (p)
							{
							case 1:
								c = color(i);
								break;
							case 2:
								c = color(j);
								break;
							default:
								c = color(k);
								break;
							}
							switch (c)
							{
							case red:cout << setw(7) << "red";break;
							case yellow:cout << setw(7) << "yellow";break;
							case blue:cout << setw(7) << "blue";break;
							case white:cout << setw(7) << "white";break;
							default:cout << setw(7) << "black";break;
							}
						}
						cout << endl;
					}
				}
			}
		}
	}
	cout << "共有：" << n << "种取法。" << endl;
	system("pause");
}