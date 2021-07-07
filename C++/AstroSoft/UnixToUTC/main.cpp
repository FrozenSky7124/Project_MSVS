#include <stdio.h>
#include <time.h>

typedef struct _Time_UTC_ {
	int year; int month;  int day;
	int hour; int minute;  int second;
} st_UTC;

/**
 * Function : Convert Unix timestamp to UTC time
 * Parameter:
 *      _IN_ seconds: unix timestamp seconds
 *      _IN_ timezone: local timezone like 8 or -8
 *     _OUT_ utctime: struct of utc time
 */
void calculate(long long seconds, int timezone, st_UTC * utctime)
{
	int _year, _month, _day, _hour, _minute, _second;

	long long total = seconds + timezone * 3600;
	_second = total % 60;
	_minute = (total % 3600) / 60;
	_hour = (total % (3600 * 24)) / 3600;
	_year = 1970;
	_month = _day = 1;
	while (total >= 365 * 24 * 60 * 60)
	{
		bool leap = (_year % 4 == 0 && _year % 100 != 0) || (_year % 400 == 0);
		total -= leap ? 366 * 24 * 60 * 60 : 365 * 24 * 60 * 60;
		_year += 1;
	}
	while (total >= 86400)
	{
		bool leap = (_year % 4 == 0 && _year % 100 != 0) || (_year % 400 == 0);
		total -= 86400;
		_day++;
		if (_day == 32)
		{
			_month += 1;
			_day = 1;
		}
		if (_day == 31 && ((_month == 4) || (_month == 6) || (_month == 9) || (_month == 11)))
		{
			_month += 1;
			_day = 1;
		}
		if (leap && (_month == 2) && (_day == 30))
		{
			_month += 1;
			_day = 1;
		}
		if ((!leap) && _month == 2 && _day == 29)
		{
			_month += 1;
			_day = 1;
		}
	}

	utctime->year = _year;
	utctime->month = _month;
	utctime->day = _day;
	utctime->hour = _hour;
	utctime->minute = _minute;
	utctime->second = _second;
}

int main(void)
{
	time_t timep;
	struct tm* p;
	timep = 0x60D398AC;//time(&timep);
	p = localtime(&timep);
	timep = mktime(p);
	struct tm *mytm = localtime(&timep);
	//printf(" %d,  %d,   %d,   %d,  Minute is %d,  Second is %d\r\n", mytm->tm_year+1900, mytm->tm_mon + 1, mytm->tm_mday, mytm->tm_hour, mytm->tm_min, mytm->tm_sec);

	st_UTC t1;
	calculate(1582873200, 8, &t1);
	printf("%04d-%02d-%02d %02d-%02d-%02d\r\n", t1.year, t1.month, t1.day, t1.hour, t1.minute, t1.second);


	time_t unix_timestamp = 1582873200;
	struct tm *tmdate = localtime(&unix_timestamp);
	printf("Local Time is : asctime =%s", asctime(tmdate));
	printf("  GMT Time is : gmtime  =%s", asctime(gmtime(&unix_timestamp)));

	printf("[0x%08X]   %04d-%02d-%02d %02d-%02d-%02d\r\n", int(unix_timestamp), tmdate->tm_year + 1900, tmdate->tm_mon + 1, tmdate->tm_mday,
		tmdate->tm_hour, tmdate->tm_min, tmdate->tm_sec);

	getchar();
	return 0;
}
