/**********************************************************************
* Name:      FSC_FitsX
* Function:  Open & Read FITS Format Files.
* Author:    FrozenSky
* Created:   2018-10-25
* Updated:   2020-08-20
* Copyright: FrozenSky@Avalon
* License:   MIT
**********************************************************************/


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define FITS_UNIT_SIZE  2880
#define PATH_MAX_LENGTH 256

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	//define something for Windows (32-bit and 64-bit, this part is common)
	#ifdef _WIN64
		//define something for Windows (64-bit only)
	#else
		//define something for Windows (32-bit only)
	#endif
#elif __linux__
	//define something for linux
	#include "string.h"
	#include "stdio.h"
#elif __unix__
	//define something for unix
#endif

#ifndef _FITSXTIME_
#define _FITSXTIME_
typedef struct _FITSXTIME
{
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDayOfWeek;
	unsigned short wDay;
	unsigned short wHour;
	unsigned short wMinute;
	unsigned short wSecond;
	unsigned short wMseconds;
	unsigned short wUseconds;
} FITSXTIME;
#endif // !_FITSXTIME


using namespace std;

class FSC_FitsX
{
public:
	FSC_FitsX();
	~FSC_FitsX();

	//=================================================
	// @Method: Open FITS file & Load FITS Data
	// @Param: {LPCTSTR} lpszPath (FITS file path)
	// @Return: {bool} (Success or Failed)
	//=================================================
	bool OpenFitsFile(const char *pFilePath);
	bool OpenFitsFile_KL4040(const char *lpszPath);

	//=================================================
	// @Method: Get FITS Data Ptr
	// @Return: {int*} (FITS file data pointer)
	//=================================================
	int* GetFitsDataPtr();

	//=================================================
	// @Method: Get FITS Data by (x, y)
	// @Param: {int} x (x | 0 from left to right)   {int} y (y | 0 from top to bottom)
	// @Return: {int} (FITS file data, -1 when error)
	//=================================================
	int GetFitsData(int x, int y);

	//=================================================
	// @Method: Get FITS Data by position
	// @Param : {long} lIndex (the ��ٿ�˿ of data)
	// @Return: {int} (FITS file data, -1 when error)
	//=================================================
	int GetFitsData(long lIndex);

	//=================================================
	// @Method: Get FITS data width
	// @Return: {int} (FITS data width)
	//=================================================
	int GetWidth();

	//=================================================
	// @Method: Get FITS data height
	// @Return: {int} (FITS data height)
	//=================================================
	int GetHeight();

	//=================================================
	// @Method: Get OBS data
	// @Param : {SYSTEMTIME} sysTime (DATE-OBS & TIME-OBS in UTC)
	//          {double} RA (in hour)
	//          {double} DEC (in deg)
	//=================================================
	void GetOBSData(FITSXTIME & sysTime, double & RA, double & DEC);
	void GetOBSData_KL4040(FITSXTIME & sysTime, double & HA, double & DEC);

	//=================================================
	// @Method: Get FITS exposure time
	// @Return: {double} (FITS exposure time in second)
	//=================================================
	double GetExpTime();

	//=================================================
	// @Method: Calc FITS data OBS-DATE
	// @Return: {bool} (Success or Fail)
	//=================================================
	bool CalcOBSDate(string & csDate, FITSXTIME & OT);
	bool CalcOBSDate_KL4040(string & csDate, FITSXTIME & OT);

	//=================================================
	// @Method: Calc FITS data OBS-TIME
	// @Return: {bool} (Success or Fail)
	//=================================================
	bool CalcOBSTime(string & csTime, FITSXTIME & OT);

	//=================================================
	// @Method: Calc FITS data RA
	// @Return: {double} (RA h)
	//=================================================
	double CalcRA(string & csRA);

	//=================================================
	// @Method: Calc FITS data HA
	// @Return: {double} (HA h)
	//=================================================
	double CalcHA(string & csHA);

	//=================================================
	// @Method: Calc FITS data DEC
	// @Return: {double} (DEC deg)
	//=================================================
	double CalcDEC(string & csDEC);

	//=================================================
	// @Method: Get Min pixel data value
	// @Return: {int} (Min pixel data value)
	//=================================================
	int GetMinPixelCount();

	//=================================================
	// @Method: Get Max pixel data value
	// @Return: {int} (Max pixel data value)
	//=================================================
	int GetMaxPixelCount();

	//=================================================
	// @Method: Get Average pixel data value
	// @Return: {int} (Average pixel data value)
	//=================================================
	int GetAveragePixelCount();

	//=================================================
	// @Method: Get HDU Number
	// @Return: {int} (FITS HDU Number)
	//=================================================
	int GetHDUNum();
	
	//=================================================
	// @Method: Get FITS HDU Header Key
	// @Param: {int} (HDU Header Key Position)
	// @Return: {CString} (HDU Header Key)
	//=================================================
	string GetHDUKey(int iPos);

	//=================================================
	// @Method: Get FITS HDU Header Value
	// @Param: {int} (HDU Header Value Position)
	// @Return: {CString} (HDU Header Value)
	//=================================================
	string GetHDUValue(int iPos);

private:
	char m_filePath[PATH_MAX_LENGTH];         // FITS file name
	vector<string> m_vHDUKey;           // FITS HDU Header Key vector
	vector<string> m_vHDUValue;         // FITS HDU Header Value vector
	int* m_pFitsData;                   // FITS Data vector
	int m_iHDUNum;                      // FITS HDU Header Number (begin from 0)
	int m_iBITPIX;                      // BITPIX
	int m_iNAXIS1;                      // NAXIS1
	int m_iNAXIS2;                      // NAXIS2
	double m_dBSCALE;                   // BSCALE
	double m_dBZERO;                    // BZERO
	double m_dRA;                       // RA(h)
	double m_dHA;                       // HourAngle(h)
	double m_dDEC;                      // DEC(deg)
	double m_dExpTime;                  // EXPTIME(s)
	FITSXTIME m_SysTime;                // Date & Time

	int m_iMinPixelCount;               // Min pixel data value
	int m_iMaxPixelCount;               // Max pixel data value
	int m_iAveragePixelCount;           // Average pixel data value
};