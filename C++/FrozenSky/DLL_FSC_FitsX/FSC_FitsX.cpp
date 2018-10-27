//======================================================================
// FSC_FitsX.cpp
//
// FrozenSky's FITS Format Class Library
// 
// Func:
//       1.
//       2.
//====================================================================== 


#include "FSC_FitsX.h"


FSC_FitsX::FSC_FitsX()
{

}


FSC_FitsX::~FSC_FitsX()
{

}


bool FSC_FitsX::OpenFitsFile(LPCTSTR lpszPath)
{
	// Release old data
	// ...
	
	// Store FITS file path
	strcpy(m_filePath, lpszPath);

	// Open FITS file
	CFile FitsFile;
	if (!FitsFile.Open(m_filePath, CFile::modeRead | CFile::shareDenyWrite))
	{
		return FALSE;
	}
	// Read FITS HDU Header (FITSUnitSize = 2880 Bytes)
	BYTE FitsHeader[FITSUnitSize];
	memset(&FitsHeader, 0, FITSUnitSize);
	FitsFile.Read(&FitsHeader, FITSUnitSize);
	// Load FITS HDU Header Key & Value
	for (m_iHDUNum = 0; m_iHDUNum < FITSUnitSize / 80; m_iHDUNum++)
	{
		CString csTmpUnit;
		CString csTmpKey;
		CString csTmpValue;
		CHAR tmpUnit[80];

		memcpy_s(tmpUnit, 80, FitsHeader + m_iHDUNum * 80, 80);
		tmpUnit[79] = '\0';
		csTmpUnit = CString(tmpUnit);
		csTmpKey = csTmpUnit.Left(8);
		csTmpKey.TrimLeft();
		csTmpKey.TrimRight();
		csTmpValue = csTmpUnit.Mid(9);
		csTmpValue.TrimLeft();
		csTmpValue.TrimRight();
		if (csTmpKey == _T("BITPIX")) m_iBITPIX = atoi(csTmpValue);
		if (csTmpKey == _T("NAXIS1")) m_iNAXIS1 = atoi(csTmpValue);
		if (csTmpKey == _T("NAXIS2")) m_iNAXIS2 = atoi(csTmpValue);
		if (csTmpKey == _T("BSCALE")) m_dBSCALE = atoi(csTmpValue);
		if (csTmpKey == _T("BZERO"))  m_dBZERO  = atoi(csTmpValue);
		if (csTmpKey == _T("END")) break;
		m_vHDUKey.push_back(csTmpKey);
		m_vHDUValue.push_back(csTmpValue);
	}

	for (int i = 0; i < m_iHDUNum; i++)
	{
		TRACE(_T("%-8s=%s\n"), m_vHDUKey.at(i), m_vHDUValue.at(i));
	}

	FitsFile.Close();
	return false;
}