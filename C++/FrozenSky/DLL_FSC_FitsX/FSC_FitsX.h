/**********************************************************************
* Name:      FSC_FitsX.h
* Function:  Open & Read FITS Format Files.
* Author:    FrozenSky
* Created:   2018-10-25
* Copyright: FrozenSky@Avalon
* License:   GPL
**********************************************************************/


#include "afx.h"
#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

#define FITSUnitSize 2880

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
	bool OpenFitsFile(LPCTSTR lpszPath);
	bool OpenFitsFile_KL4040(LPCTSTR lpszPath);

	bool OpenFitsFileToCheckTime(LPCTSTR lpszPath, CString & csOutput);

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
	// @Param : {long} lIndex (the ÒðµÙ¿ËË¿ of data)
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
	void GetOBSData(SYSTEMTIME & sysTime, double & RA, double & DEC);
	void GetOBSData_KL4040(SYSTEMTIME & sysTime, double & HA, double & DEC);

	//=================================================
	// @Method: Get FITS exposure time
	// @Return: {double} (FITS exposure time in second)
	//=================================================
	double GetExpTime();

	//=================================================
	// @Method: Calc FITS data OBS-DATE
	// @Return: {bool} (Success or Fail)
	//=================================================
	bool CalcOBSDate(CString & csDate, SYSTEMTIME & OT);
	bool CalcOBSDate_KL4040(CString & csDate, SYSTEMTIME & OT);

	//=================================================
	// @Method: Calc FITS data OBS-TIME
	// @Return: {bool} (Success or Fail)
	//=================================================
	bool CalcOBSTime(CString & csTime, SYSTEMTIME & OT);

	//=================================================
	// @Method: Calc FITS data RA
	// @Return: {double} (RA h)
	//=================================================
	double CalcRA(CString & csRA);

	//=================================================
	// @Method: Calc FITS data HA
	// @Return: {double} (HA h)
	//=================================================
	double CalcHA(CString & csHA);

	//=================================================
	// @Method: Calc FITS data DEC
	// @Return: {double} (DEC deg)
	//=================================================
	double CalcDEC(CString & csDEC);

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
	CString GetHDUKey(int iPos);

	//=================================================
	// @Method: Get FITS HDU Header Value
	// @Param: {int} (HDU Header Value Position)
	// @Return: {CString} (HDU Header Value)
	//=================================================
	CString GetHDUValue(int iPos);

private:
	char m_filePath[_MAX_PATH];         // FITS file name
	vector<CString> m_vHDUKey;          // FITS HDU Header Key vector
	vector<CString> m_vHDUValue;        // FITS HDU Header Value vector
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
	SYSTEMTIME m_SysTime;               // Date & Time

	int m_iMinPixelCount;               // Min pixel data value
	int m_iMaxPixelCount;               // Max pixel data value
	int m_iAveragePixelCount;           // Average pixel data value
};