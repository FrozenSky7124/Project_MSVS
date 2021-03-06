#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#include <fstream>
#include <string>
#include <algorithm>

#define MAXSIZE 1024
#define MAXNIDARRAY 1024

#pragma comment(lib, "Wininet.lib")

void GetFileName(char* cFileName, int size)
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	sprintf_s(cFileName, size, "%04d%02d%02d.tle", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
}

void urlopen(char* url)
{
	HINTERNET hSession = InternetOpen("UrlTest", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hHttp = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);

		if (hHttp != NULL)
		{
			printf_s("%s\n", url);

			char Temp[MAXSIZE];
			ULONG Number = 1;
			while (Number > 0)
			{
				InternetReadFile(hHttp, Temp, MAXSIZE - 1, &Number);
				Temp[Number] = '\0';
				printf("%s", Temp);
			}
			InternetCloseHandle(hHttp);
			hHttp = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
}

void urlopen(const char* url, std::string& tlestring)
{
	HINTERNET hSession = InternetOpen("UrlTest", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hHttp = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);

		if (hHttp != NULL)
		{
			//printf_s("%s\n", url);

			char Temp[MAXSIZE];
			ULONG Number = 1;
			while (Number > 0)
			{
				InternetReadFile(hHttp, Temp, MAXSIZE - 1, &Number);
				Temp[Number] = '\0';
				//printf("%s", Temp);
				tlestring += Temp;
				while (tlestring.find("\r") != -1)
				{
					tlestring.replace(tlestring.find("\r"), 1, "");
				}	
			}
			InternetCloseHandle(hHttp);
			hHttp = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
}

int readNIDList(int* iArray, int& n)
{
	std::string tempstr;
	std::ifstream infile;
	infile.open("TLE_list.txt");
	if (!infile.is_open())
	{
		printf("Fail to open NID list file.\n");
		return -1;
	}
	/*
	while (!infile.eof())
	{
		infile >> tempstr;
		//printf("%s\n", tempstr.c_str());
		instring += tempstr;
	}
	*/
	int i = 0;
	while (getline(infile, tempstr, ','))
	{
		*(iArray + i) = atoi(tempstr.c_str());
		i++;
		//printf("%02d %6d\n", i, atoi(tempstr.c_str()));
	}
	n = i;
	infile.close();

	//printf("%s\n", instring.c_str());

	return 0;
}

int main(int argc, char* argv[])
{
	//urlopen("https://celestrak.com/satcat/tle.php?CATNR=20025");

	int iNIDArray[MAXNIDARRAY];
	int iNIDState[MAXNIDARRAY];
	int nNIDCount = 0;
	std::string strUrl, strTLE, strTemp;
	char outFileName[128];
	GetFileName(outFileName, 128);
	FILE *outfile;
	outfile = fopen(outFileName, "w");
	readNIDList(iNIDArray, nNIDCount);
	std::fill(iNIDState, iNIDState + nNIDCount, 0);

	//for (int i = 0; i < nNIDCount; i++)
	//{
	//	strTemp = "";
	//	strUrl = "http://celestrak.com/satcat/tle.php?CATNR=" + std::to_string(iNIDArray[i]);
	//	urlopen(strUrl.c_str(), strTemp);
	//	if (strTemp.length() < 50) // check data size of tle string
	//	{
	//		printf("Internet error! Fail to load TLE:%6d.\n", iNIDArray[i]);
	//		continue;
	//	}
	//	strTemp = "0 " + strTemp;
	//	printf("%s", strTemp.c_str());
	//	fprintf(outfile, "%s", strTemp.c_str());
	//	Sleep(500);
	//}

	//Loop 3 times
	printf("当前数据源： CelesTrak\n");
	Sleep(1000);
	printf("目标列表： TLE_list.txt\n");
	Sleep(1000);
	printf("模式： HTTP\n");
	Sleep(1000);
	printf("开始下载 TLE 数据...\n");
	int nFinished = 0;
	int iLoopTime = 0;
	while (iLoopTime < 3)
	{
		iLoopTime++;
		//Get TLE data
		for (int i = 0; i < nNIDCount; i++)
		{
			if (iNIDState[i] == 1) continue;
			strTemp = "";
			strUrl = "http://celestrak.com/satcat/tle.php?CATNR=" + std::to_string(iNIDArray[i]);
			urlopen(strUrl.c_str(), strTemp);
			if (strTemp.length() < 50) // check data size of tle string
			{
				printf("\nInternet error! Fail to load TLE:%6d.\n", iNIDArray[i]);
				continue;
			}
			strTemp = "0 " + strTemp;
			//printf("%s", strTemp.c_str());
			fprintf(outfile, "%s", strTemp.c_str());
			iNIDState[i] = 1;
			nFinished++;

			//Processer
			int n1 = int(50 * nFinished / nNIDCount);
			int n2 = 50 - n1;
			printf("\r| ");
			for (int p = 0; p < n1; p++) printf(">");
			for (int p = 0; p < n2; p++) printf("-");
			printf(" | %3d \/%3d |", nFinished, nNIDCount);

			Sleep(500);
		}
		
	}

	fclose(outfile);

	printf("\n数据下载完成： %s\n\n", outFileName);
	system("pause");
	return 0;
}