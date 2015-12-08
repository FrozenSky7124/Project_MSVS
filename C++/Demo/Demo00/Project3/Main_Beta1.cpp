#include <iostream>
#include <iomanip>
using namespace std;

enum color {
	red, yellow, blue, white, black,
};

int getSum(int i, int j, int k) //获得数据和 //假设：red 1, yellow 10, blue 100, white 1000, black 10000;
{
	int _sum = 0;
	int _i, _j, _k;
	switch (i)
	{
	case red:_i = 1;break;
	case yellow:_i = 10;break;
	case blue:_i = 100;break;
	case white:_i = 1000;break;
	default:_i = 10000;break;
	}
	switch (j)
	{
	case red:_j = 1;break;
	case yellow:_j = 10;break;
	case blue:_j = 100;break;
	case white:_j = 1000;break;
	default:_j = 10000;break;
	}
	switch (k)
	{
	case red:_k = 1;break;
	case yellow:_k = 10;break;
	case blue:_k = 100;break;
	case white:_k = 1000;break;
	default:_k = 10000;break;
	}
	_sum = _i + _j + _k;
	return _sum;
}

void main()
{
	int check[60] = { 0 };
	int sum = 0;
	color c;
	int i, j, k;
	int n = 0;
	int p = 0;
	int flag = 0;
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
						sum = getSum(i, j, k);
						flag = 0;
						for (int cn = 0;cn < n;cn++)
						{
							if (sum == check[cn])
							{
								flag = -1;
								break;
							}
						}
						if (flag == 0)
						{
							check[n] = sum;
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
	}
	cout << "共有：" << n << "种取法。" << endl;
	for (int w = 0;w < 60;w++)
	{
		cout << check[w] << " ";
	}
	system("pause");
}