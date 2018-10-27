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
	// Func: Open FITS file & Load FITS Data
	// Para: _IN_ LPCTSTR lpszPath   (FITS file path)
	//=================================================
	bool OpenFitsFile(LPCTSTR lpszPath);

private:
	char m_filePath[_MAX_PATH];         // FITS file name
	vector<CString> m_vHDUKey;          // FITS HDU Header Key vector
	vector<CString> m_vHDUValue;        // FITS HDU Header Value vector
	int m_iHDUNum;                      // FITS HDU Header Number
	int m_iBITPIX;                      // BITPIX
	int m_iNAXIS1;                      // NAXIS1
	int m_iNAXIS2;                      // NAXIS2
	double m_dBSCALE;                   // BSCALE
	double m_dBZERO;                    // BZERO
};