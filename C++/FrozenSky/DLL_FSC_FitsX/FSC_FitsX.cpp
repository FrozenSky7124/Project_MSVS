#include "FSC_FitsX.h"


FSC_FitsX::FSC_FitsX()
{
	m_pFitsData = NULL;
}


FSC_FitsX::~FSC_FitsX()
{
	if (m_pFitsData != NULL)
	{
		delete[] m_pFitsData;
		m_pFitsData = NULL;
	}
	m_vHDUKey.clear();
	m_vHDUValue.clear();
}


bool FSC_FitsX::OpenFitsFile(LPCTSTR lpszPath)
{
	BYTE bLow, bHigh;
	SHORT sTmpValue;
	INT iValue;

	// Release old data
	if (m_pFitsData != NULL)
	{
		delete[] m_pFitsData;
		m_pFitsData = NULL;
	}
	m_vHDUKey.clear();
	m_vHDUValue.clear();
	
	// Store FITS file path
	strcpy(m_filePath, lpszPath);

	// Open FITS file
	CFile FitsFile;
	if (!FitsFile.Open(m_filePath, CFile::modeRead | CFile::shareDenyWrite))
	{
		return false;
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
		if (csTmpKey == _T("BSCALE")) m_dBSCALE = atof(csTmpValue);
		if (csTmpKey == _T("BZERO"))  m_dBZERO  = atof(csTmpValue);
		if (csTmpKey == _T("DATE-OBS")) CalcOBSDate(csTmpValue, m_SysTime);
		if (csTmpKey == _T("TIME-OBS")) CalcOBSTime(csTmpValue, m_SysTime);
		if (csTmpKey == _T("IEXPTIME")) m_dExpTime = atof(csTmpValue);
		if (csTmpKey == _T("RA")) m_dRA = CalcRA(csTmpValue);
		if (csTmpKey == _T("DEC")) m_dDEC = CalcDEC(csTmpValue);
		if (csTmpKey == _T("END")) break;
		m_vHDUKey.push_back(csTmpKey);
		m_vHDUValue.push_back(csTmpValue);
	}
	// Load FITS Data
	int minPixelCount = 65535;
	int maxPixelCount = 0;
	int iPixelSize = m_iBITPIX / 8;
	LONG lDataSize = iPixelSize * m_iNAXIS1 * m_iNAXIS2;
	BYTE* lpFitsData = (BYTE*) new BYTE[lDataSize];
	m_pFitsData = (int*) new int[m_iNAXIS1 * m_iNAXIS2];
	memset(lpFitsData, 0, lDataSize);
	FitsFile.Read(lpFitsData, lDataSize);
	for (int i = 0; i < m_iNAXIS2; i++)
	{
		int iWidthSize = i * m_iNAXIS1 * iPixelSize;
		for (int j = 0; j < m_iNAXIS1; j++)
		{
			bHigh = *(lpFitsData + iWidthSize + j * iPixelSize);
			bLow = *(lpFitsData + iWidthSize + j * iPixelSize + 1);
			sTmpValue = 0;
			sTmpValue = ((sTmpValue + bHigh) << 8) + bLow;
			//m_pFitsData[i * m_iNAXIS1 + j] = sTmpValue * (int)m_dBSCALE + (int)m_dBZERO;
			iValue = sTmpValue + (int)m_dBZERO;
			m_pFitsData[i * m_iNAXIS1 + j] = iValue;
			if (iValue < minPixelCount) minPixelCount = iValue;
			if (iValue > maxPixelCount) maxPixelCount = iValue;
		}
	}
	m_iMinPixelCount = minPixelCount;
	m_iMaxPixelCount = maxPixelCount;

	//for (int i = 0; i < m_iHDUNum; i++)
	//{
	//	TRACE(_T("%-8s=%s\n"), m_vHDUKey.at(i), m_vHDUValue.at(i));
	//}

	delete[] lpFitsData;
	FitsFile.Close();
	return true;
}


bool FSC_FitsX::OpenFitsFile_KL4040(LPCTSTR lpszPath)
{
	BYTE bLow, bHigh;
	SHORT sTmpValue;
	INT iValue;

	// Release old data
	if (m_pFitsData != NULL)
	{
		delete[] m_pFitsData;
		m_pFitsData = NULL;
	}
	m_vHDUKey.clear();
	m_vHDUValue.clear();

	// Store FITS file path
	strcpy(m_filePath, lpszPath);

	// Open FITS file
	CFile FitsFile;
	if (!FitsFile.Open(m_filePath, CFile::modeRead | CFile::shareDenyWrite))
	{
		return false;
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
		if (csTmpKey == _T("BSCALE")) m_dBSCALE = atof(csTmpValue);
		if (csTmpKey == _T("BZERO"))  m_dBZERO = atof(csTmpValue);
		if (csTmpKey == _T("DATE-OBS")) CalcOBSDate_KL4040(csTmpValue, m_SysTime);
		//if (csTmpKey == _T("TIME-OBS")) CalcOBSTime(csTmpValue, m_SysTime);
		if (csTmpKey == _T("EXPOSURE")) m_dExpTime = atof(csTmpValue);
		if (csTmpKey == _T("HA")) m_dHA = CalcHA(csTmpValue);
		if (csTmpKey == _T("DE")) m_dDEC = atof(csTmpValue);
		if (csTmpKey == _T("END")) break;
		m_vHDUKey.push_back(csTmpKey);
		m_vHDUValue.push_back(csTmpValue);
	}
	// Load FITS Data
	int minPixelCount = 65535;
	int maxPixelCount = 0;
	int iPixelSize = m_iBITPIX / 8;
	LONG lDataSize = iPixelSize * m_iNAXIS1 * m_iNAXIS2;
	BYTE* lpFitsData = (BYTE*) new BYTE[lDataSize];
	m_pFitsData = (int*) new int[m_iNAXIS1 * m_iNAXIS2];
	memset(lpFitsData, 0, lDataSize);
	FitsFile.Read(lpFitsData, lDataSize);
	for (int i = 0; i < m_iNAXIS2; i++)
	{
		int iWidthSize = i * m_iNAXIS1 * iPixelSize;
		for (int j = 0; j < m_iNAXIS1; j++)
		{
			bHigh = *(lpFitsData + iWidthSize + j * iPixelSize);
			bLow = *(lpFitsData + iWidthSize + j * iPixelSize + 1);
			sTmpValue = 0;
			sTmpValue = ((sTmpValue + bHigh) << 8) + bLow;
			//m_pFitsData[i * m_iNAXIS1 + j] = sTmpValue * (int)m_dBSCALE + (int)m_dBZERO;
			iValue = sTmpValue + (int)m_dBZERO;
			m_pFitsData[i * m_iNAXIS1 + j] = iValue;
			if (iValue < minPixelCount) minPixelCount = iValue;
			if (iValue > maxPixelCount) maxPixelCount = iValue;
		}
	}
	m_iMinPixelCount = minPixelCount;
	m_iMaxPixelCount = maxPixelCount;

	//for (int i = 0; i < m_iHDUNum; i++)
	//{
	//	TRACE(_T("%-8s=%s\n"), m_vHDUKey.at(i), m_vHDUValue.at(i));
	//}

	delete[] lpFitsData;
	FitsFile.Close();
	return true;
}


bool FSC_FitsX::OpenFitsFileToCheckTime(LPCTSTR lpszPath, CString & csOutput)
{
	// store time string
	csOutput = _T("");
	bool bTimeErr = true;
	// Release old data
	if (m_pFitsData != NULL)
	{
		delete[] m_pFitsData;
		m_pFitsData = NULL;
	}
	m_vHDUKey.clear();
	m_vHDUValue.clear();

	// Store FITS file path
	strcpy(m_filePath, lpszPath);

	// Open FITS file
	CFile FitsFile;
	if (!FitsFile.Open(m_filePath, CFile::modeRead | CFile::shareDenyWrite))
	{
		return false;
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
		if (csTmpKey == _T("BSCALE")) m_dBSCALE = atof(csTmpValue);
		if (csTmpKey == _T("BZERO"))  m_dBZERO = atof(csTmpValue);
		if (csTmpKey == _T("DATE-OBS")) CalcOBSDate(csTmpValue, m_SysTime);
		if (csTmpKey == _T("TIME-OBS"))
		{
			csOutput = csOutput + _T("TIME-OBS ") + csTmpValue + _T("\n");
			bool bFlag = CalcOBSTime(csTmpValue, m_SysTime);
			if (bFlag == false) bTimeErr = false;
		}
		if (csTmpKey == _T("RA")) m_dRA = CalcRA(csTmpValue);
		if (csTmpKey == _T("DEC")) m_dDEC = CalcDEC(csTmpValue);
		if (csTmpKey == _T("TIME-COM"))
		{
			csOutput = csOutput + _T("TIME-COM ") + csTmpValue;
			SYSTEMTIME OT;
			bool bFlag = CalcOBSTime(csTmpValue, OT);
			if (bFlag == false) bTimeErr = false;
		}
		if (csTmpKey == _T("END")) break;
		m_vHDUKey.push_back(csTmpKey);
		m_vHDUValue.push_back(csTmpValue);
	}
	FitsFile.Close();
	return bTimeErr;
}


int * FSC_FitsX::GetFitsDataPtr()
{
	return m_pFitsData;
}


int FSC_FitsX::GetFitsData(int x, int y)
{
	if (!m_pFitsData) return -1;
	if (x >= GetWidth()) return -1;
	if (y >= GetHeight()) return -1;
	return m_pFitsData[y * GetWidth() + x];
}


int FSC_FitsX::GetFitsData(long lIndex)
{
	if (!m_pFitsData) return -1;
	if (lIndex > GetWidth() * GetHeight()) return -1;
	return m_pFitsData[lIndex];
}


int FSC_FitsX::GetWidth()
{
	return m_iNAXIS1;
}


int FSC_FitsX::GetHeight()
{
	return m_iNAXIS2;
}


void FSC_FitsX::GetOBSData(SYSTEMTIME & sysTime, double & RA, double & DEC)
{
	sysTime = m_SysTime;
	RA = m_dRA;
	DEC = m_dDEC;
}


void FSC_FitsX::GetOBSData_KL4040(SYSTEMTIME & sysTime, double & HA, double & DEC)
{
	sysTime = m_SysTime;
	HA = m_dHA;
	DEC = m_dDEC;
}


double FSC_FitsX::GetExpTime()
{
	return m_dExpTime;
}


bool FSC_FitsX::CalcOBSDate(CString & csDate, SYSTEMTIME & OT)
{
	int iYear = atoi(csDate.Mid(1));
	int iMonth = atoi(csDate.Mid(6));
	int iDay = atoi(csDate.Mid(9));
	if (iYear < 0 || iMonth < 1 || iMonth > 12 || iDay < 1 || iDay > 32) return false;
	OT.wYear = iYear;
	OT.wMonth = iMonth;
	OT.wDay = iDay;
	return true;
}


bool FSC_FitsX::CalcOBSDate_KL4040(CString & csDate, SYSTEMTIME & OT)
{
	int iYear = atoi(csDate.Mid(1));
	int iMonth = atoi(csDate.Mid(6));
	int iDay = atoi(csDate.Mid(9));
	if (iYear < 0 || iMonth < 1 || iMonth > 12 || iDay < 1 || iDay > 32) return false;
	OT.wYear = iYear;
	OT.wMonth = iMonth;
	OT.wDay = iDay;

	int iH = atoi(csDate.Mid(12));
	int iM = atoi(csDate.Mid(15));
	int iS = atoi(csDate.Mid(18));
	int iMS = atoi(csDate.Mid(21));
	if (iH < 0 || iH > 24 || iM < 0 || iM > 60 || iS < 0) return false;
	OT.wHour = iH;
	OT.wMinute = iM;
	OT.wSecond = iS;
	OT.wMilliseconds = iMS;

	return true;
}


bool FSC_FitsX::CalcOBSTime(CString & csTime, SYSTEMTIME & OT)
{
	int iH = atoi(csTime.Mid(1));
	int iM = atoi(csTime.Mid(4));
	int iS = atoi(csTime.Mid(7));
	int iMS = atoi(csTime.Mid(10, 3));
	m_us = atoi(csTime.Mid(13));
	if (iH < 0 || iH > 24 || iM < 0 || iM > 60 || iS < 0) return false;
	OT.wHour = iH;
	OT.wMinute = iM;
	OT.wSecond = iS;
	OT.wMilliseconds = iMS;
	return true;
}


double FSC_FitsX::CalcRA(CString & csRA)
{
	int h = atoi(csRA.Mid(1));
	int m = atoi(csRA.Mid(4));
	double s = atof(csRA.Mid(7));
	return h + m / 60. + s / 3600.;
}


double FSC_FitsX::CalcHA(CString & csHA)
{
	double dHA = atof(csHA);
	dHA = dHA / 360.0 * 24.0;
	return dHA;
}


double FSC_FitsX::CalcDEC(CString & csDEC)
{
	int split1 = csDEC.Find(_T(":"), 0);
	int split2 = csDEC.Find(_T(":"), split1 + 1);
	if (split1 == -1 || split2 == -1) throw _T("Error in func: GetDEC(CString & csDEC).");

	int D = atoi(csDEC.Mid(1));
	D = abs(D);
	int M = atoi(csDEC.Mid(split1 + 1));
	double S = atof(csDEC.Mid(split2 + 1));

	double DEC = D + M / 60. + S / 3600.;
	if (-1 != csDEC.Find(_T("-"))) DEC = -DEC;
	return DEC;
}


int FSC_FitsX::GetMinPixelCount()
{
	return m_iMinPixelCount;
}


int FSC_FitsX::GetMaxPixelCount()
{
	return m_iMaxPixelCount;
}


int FSC_FitsX::GetAveragePixelCount()
{
	long long lTotal = 0;
	for (int i = 0; i < GetHeight(); i++)
	{
		for (int j = 0; j < GetWidth(); j++)
		{
			lTotal += *(GetFitsDataPtr() + i * GetWidth() + j);
		}
	}
	m_iAveragePixelCount = lTotal / GetWidth() / GetHeight();
	return m_iAveragePixelCount;
}


int FSC_FitsX::GetHDUNum()
{
	return m_iHDUNum;
}


CString FSC_FitsX::GetHDUKey(int iPos)
{
	if (iPos >= m_iHDUNum) return _T("");
	return m_vHDUKey.at(iPos);
}


CString FSC_FitsX::GetHDUValue(int iPos)
{
	if (iPos >= m_iHDUNum) return _T("");
	return m_vHDUValue.at(iPos);
}
