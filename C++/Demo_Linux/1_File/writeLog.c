void writelog(const char *log)
{
	time_t tDate;
	struct tm *eventTime;
	time(&tDate);//获得系统当前时间
	eventTime = localtime(&tDate);//将time_t数据结构转化为struct tm结构
	int iYear = eventTime->tm_year + 1900;
	int iMon  = eventTime->tm_mon + 1;
	int iDay  = eventTime->tm_mday;
	int iHour = eventTime->tm_hour;
	int iMin  = eventTime->tm_min;
	int iSec  = eventTime->tm_sec;

	//printf("tm_isdst = %d\n", eventTime->tm_isdst);//是否夏时制

	char sDate[16];
	sprintf(sDate, "%04d-%02d-%02d", iYear, iMon, iDay);
	char sTime[16];
	sprintf(sTime, "%02d:%02d:%02d", iHour, iMin, iSec);
	char s[1024];
	sprintf(s, "%s %s %s\n", sDate, sTime, log);
	FILE *p = fopen("my.log", "a+");
	if (p == NULL)
	{
		printf("write log my.log error:%s\n", strerror(errno));
	} else
	{
		fputs(s, p);
		fclose(p);
	}
	return;
}