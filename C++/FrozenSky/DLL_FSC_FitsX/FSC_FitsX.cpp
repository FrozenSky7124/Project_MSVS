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


int FSC_FitsX::GetHDUNum()
{
	return m_iHDUNum;
}


int FSC_FitsX::GetMinPixelCount()
{
	return m_iMinPixelCount;
}


int FSC_FitsX::GetMaxPixelCount()
{
	return m_iMaxPixelCount;
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
