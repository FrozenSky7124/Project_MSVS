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

	//=================================================
	// @Method: Get FITS Data by (x, y)
	// @Param: {int} x (x | 0 from left to right)   {int} y (y | 0 from top to bottom)
	// @Return: {int} (FITS file data, -1 when error)
	//=================================================
	int GetFitsData(int x, int y);

	//=================================================
	// @Method: Get FITS Data by position
	// @Param: {long} lIndex (the ÒðµÙ¿ËË¿ of data)
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
};