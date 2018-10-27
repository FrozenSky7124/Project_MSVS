//======================================================================
// FSC_FitsX.h
//
// FrozenSky's FITS Format Class Library
// 
// Func:
//       1.
//       2.
//====================================================================== 


#include "afx.h"
#include <iostream>
#include <vector>


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
	// @Return: {int} (FITS file data)
	//=================================================
	int GetFitsData(int x, int y);

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