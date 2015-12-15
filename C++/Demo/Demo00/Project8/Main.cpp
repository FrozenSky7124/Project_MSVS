#include <iostream>
#include <windows.h>

using namespace std;

class Time
{
private:
	int minute;
	int second;
public:
	Time(int m, int s);
	~Time();
	void showTime()
	{
		while (1)
		{
			second++;
			if (second == 60)
			{
				second = 0;
				minute++;
			}
			cout << "Time: " << minute << ":" << second << endl;
			Sleep(1000);
			system("cls");
		}
	}
};

Time::Time(int m = 0, int s = 0)
{
	minute = m;
	second = s;
}
Time::~Time()
{
	;
}
int main()
{
	Time t;
	t.showTime();
	system("pause");
	return 0;
}