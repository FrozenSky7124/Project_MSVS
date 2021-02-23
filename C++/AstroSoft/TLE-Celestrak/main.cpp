#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#include <fstream>
#include <string>

#define MAXSIZE 1024
#pragma comment(lib, "Wininet.lib")

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

	int iNIDArray[1024];
	int nNIDCount = 0;
	std::string strUrl, strTLE, strTemp;
	FILE *outfile;
	outfile = fopen("TLE.tle", "w");
	readNIDList(iNIDArray, nNIDCount);
	for (int i = 0; i < nNIDCount; i++)
	{
		strTemp = "";
		strUrl = "http://celestrak.com/satcat/tle.php?CATNR=" + std::to_string(iNIDArray[i]);
		urlopen(strUrl.c_str(), strTemp);
		if (strTemp.length() < 50) // check data size of tle string
		{
			printf("Internet error! Fail to load TLE:%6d.\n", iNIDArray[i]);
			continue;
		}
		strTemp = "0 " + strTemp;
		printf("%s", strTemp.c_str());
		fprintf(outfile, "%s", strTemp.c_str());
		Sleep(500);
	}
	fclose(outfile);

	system("pause");
	return 0;
}