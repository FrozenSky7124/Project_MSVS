#include "FSC_FitsX.h"

void GetHDUInfo(char *pTmp, string *Key, string *Value)
{
	int pos1, pos2, pos3;
	for (pos1 = 7; pos1 >= 0; pos1--)
	{
		if (pTmp[pos1] != ' ') break;
	}
	for (pos2 = 9; pos2 < 80; pos2++)
	{
		if ((pTmp[pos2] != ' ') && (pTmp[pos2] != '\'')) break;
	}
	for (pos3 = 79; pos3 >= 9; pos3--)
	{
		if ((pTmp[pos3] != ' ') && (pTmp[pos3] != '\'')) break;
	}
	if (pos2 <= pos3)
	{
		*Key = string(pTmp, 0, pos1 + 1);
		*Value = string(pTmp, pos2, pos3 - pos2 + 1);
	}
	else
	{
		*Key = string(pTmp, 0, pos1 + 1);
		*Value = string(" ");
	}
}

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


bool FSC_FitsX::OpenFitsFile(const char *pFilePath)
{
	unsigned char bLow, bHigh;
	short sTmpValue;
	int iValue;

	// Release old data
	if (m_pFitsData != NULL)
	{
		delete[] m_pFitsData;
		m_pFitsData = NULL;
	}
	m_vHDUKey.clear();
	m_vHDUValue.clear();
	
	// Store FITS file path
	strcpy(m_filePath, pFilePath);

	// Open FITS file
	FILE* fpFitsFile;
	errno_t errFile;
	size_t sizeRead;
	errFile = fopen_s(&fpFitsFile, m_filePath, "rb");
	if (errFile != 0)
	{
		return false;
	}
	// Read FITS HDU Header (FITSUnitSize = 2880 Bytes)
	unsigned char FitsHeader[FITSUnitSize];
	memset(&FitsHeader, 0, FITSUnitSize);
	sizeRead = fread_s(FitsHeader, FITSUnitSize, FITSUnitSize, 1, fpFitsFile);
	if (sizeRead != 1)
	{
		printf("Error in FSC_FitsX > OpenFitsFile > fread_s FitsHeader.\n");
	}
	// Load FITS HDU Header Key & Value
	for (m_iHDUNum = 0; m_iHDUNum < FITSUnitSize / 80; m_iHDUNum++)
	{
		char tmpUnit[80];
		string sTmpKey, sTmpValue;

		memcpy_s(tmpUnit, 80, FitsHeader + m_iHDUNum * 80, 80);
		GetHDUInfo(tmpUnit, &sTmpKey, &sTmpValue);

		if (sTmpKey == "BITPIX") m_iBITPIX = atoi(sTmpValue.c_str());
		if (sTmpKey == "NAXIS1") m_iNAXIS1 = atoi(sTmpValue.c_str());
		if (sTmpKey == "NAXIS2") m_iNAXIS2 = atoi(sTmpValue.c_str());
		if (sTmpKey == "BSCALE") m_dBSCALE = atof(sTmpValue.c_str());
		if (sTmpKey == "BZERO")  m_dBZERO  = atof(sTmpValue.c_str());
		if (sTmpKey == "DATE-OBS") CalcOBSDate(sTmpValue, m_SysTime);
		if (sTmpKey == "TIME-OBS") CalcOBSTime(sTmpValue, m_SysTime);
		if (sTmpKey == "IEXPTIME") m_dExpTime = atof(sTmpValue.c_str());
		if (sTmpKey == "RA") m_dRA = CalcRA(sTmpValue);
		if (sTmpKey == "DEC") m_dDEC = CalcDEC(sTmpValue);
		if (sTmpKey == "END") break;
		m_vHDUKey.push_back(sTmpKey);
		m_vHDUValue.push_back(sTmpValue);
	}
	// Load FITS Data
	int minPixelCount = 65535;
	int maxPixelCount = 0;
	int iPixelSize = m_iBITPIX / 8;
	long lDataSize = iPixelSize * m_iNAXIS1 * m_iNAXIS2;
	unsigned char* lpFitsData = (unsigned char*) new unsigned char[lDataSize];
	m_pFitsData = (int*) new int[m_iNAXIS1 * m_iNAXIS2];
	memset(lpFitsData, 0, lDataSize);
	sizeRead = fread_s(lpFitsData, lDataSize, lDataSize, 1, fpFitsFile);
	if (sizeRead != 1)
	{
		printf("Error in FSC_FitsX > OpenFitsFile > fread_s lpFitsData.\n");
	}
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
	if (fpFitsFile)
	{
		errFile = fclose(fpFitsFile);
		if (errFile != 0)
			printf("Error in FSC_FitsX > OpenFitsFile > fclose.\n");
	}
	return true;
}


bool FSC_FitsX::OpenFitsFile_KL4040(const char *lpszPath)
{
	unsigned char bLow, bHigh;
	short sTmpValue;
	int iValue;

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
	FILE* fpFitsFile;
	errno_t errFile;
	size_t sizeRead;
	errFile = fopen_s(&fpFitsFile, m_filePath, "rb");
	if (errFile != 0)
	{
		return false;
	}
	// Read FITS HDU Header (FITSUnitSize = 2880 Bytes)
	unsigned char FitsHeader[FITSUnitSize];
	memset(&FitsHeader, 0, FITSUnitSize);
	sizeRead = fread_s(FitsHeader, FITSUnitSize, FITSUnitSize, 1, fpFitsFile);
	if (sizeRead != 1)
	{
		printf("Error in FSC_FitsX > OpenFitsFile > fread_s FitsHeader.\n");
	}
	// Load FITS HDU Header Key & Value
	for (m_iHDUNum = 0; m_iHDUNum < FITSUnitSize / 80; m_iHDUNum++)
	{
		char tmpUnit[80];
		string sTmpKey, sTmpValue;

		memcpy_s(tmpUnit, 80, FitsHeader + m_iHDUNum * 80, 80);
		GetHDUInfo(tmpUnit, &sTmpKey, &sTmpValue);
		
		if (sTmpKey == "BITPIX") m_iBITPIX = atoi(sTmpValue.c_str());
		if (sTmpKey == "NAXIS1") m_iNAXIS1 = atoi(sTmpValue.c_str());
		if (sTmpKey == "NAXIS2") m_iNAXIS2 = atoi(sTmpValue.c_str());
		if (sTmpKey == "BSCALE") m_dBSCALE = atof(sTmpValue.c_str());
		if (sTmpKey == "BZERO")  m_dBZERO = atof(sTmpValue.c_str());
		if (sTmpKey == "DATE-OBS") CalcOBSDate_KL4040(sTmpValue, m_SysTime);
		if (sTmpKey == "EXPOSURE") m_dExpTime = atof(sTmpValue.c_str());
		if (sTmpKey == "HA") m_dHA = CalcHA(sTmpValue);
		if (sTmpKey == "DE") m_dDEC = atof(sTmpValue.c_str());
		if (sTmpKey == "END") break;
		m_vHDUKey.push_back(sTmpKey);
		m_vHDUValue.push_back(sTmpValue);
	}
	// Load FITS Data
	int minPixelCount = 65535;
	int maxPixelCount = 0;
	int iPixelSize = m_iBITPIX / 8;
	long lDataSize = iPixelSize * m_iNAXIS1 * m_iNAXIS2;
	unsigned char* lpFitsData = (unsigned char*) new unsigned char[lDataSize];
	m_pFitsData = (int*) new int[m_iNAXIS1 * m_iNAXIS2];
	memset(lpFitsData, 0, lDataSize);
	sizeRead = fread_s(lpFitsData, lDataSize, lDataSize, 1, fpFitsFile);
	if (sizeRead != 1)
	{
		printf("Error in FSC_FitsX > OpenFitsFile_KL4040 > fread_s lpFitsData.\n");
	}
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
	if (fpFitsFile)
	{
		errFile = fclose(fpFitsFile);
		if (errFile != 0)
			printf("Error in FSC_FitsX > OpenFitsFile_KL4040 > fclose.\n");
	}
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


void FSC_FitsX::GetOBSData(FITSXTIME & sysTime, double & RA, double & DEC)
{
	sysTime = m_SysTime;
	RA = m_dRA;
	DEC = m_dDEC;
}


void FSC_FitsX::GetOBSData_KL4040(FITSXTIME & sysTime, double & HA, double & DEC)
{
	sysTime = m_SysTime;
	HA = m_dHA;
	DEC = m_dDEC;
}


double FSC_FitsX::GetExpTime()
{
	return m_dExpTime;
}


bool FSC_FitsX::CalcOBSDate(string & csDate, FITSXTIME & OT)
{
	int iYear = atoi(csDate.c_str());
	int iMonth = atoi(csDate.substr(5).c_str());
	int iDay = atoi(csDate.substr(8).c_str());
	if (iYear < 0 || iMonth < 1 || iMonth > 12 || iDay < 1 || iDay > 32) return false;
	OT.wYear = iYear;
	OT.wMonth = iMonth;
	OT.wDay = iDay;
	return true;
}


bool FSC_FitsX::CalcOBSDate_KL4040(string & csDate, FITSXTIME & OT)
{
	int iYear = atoi(csDate.c_str());
	int iMonth = atoi(csDate.substr(5).c_str());
	int iDay = atoi(csDate.substr(8).c_str());
	if (iYear < 0 || iMonth < 1 || iMonth > 12 || iDay < 1 || iDay > 32) return false;
	OT.wYear = iYear;
	OT.wMonth = iMonth;
	OT.wDay = iDay;

	int iH  = atoi(csDate.substr(11).c_str());
	int iM  = atoi(csDate.substr(14).c_str());
	int iS  = atoi(csDate.substr(17).c_str());
	int iMS = atoi(csDate.substr(20).c_str());
	if (iH < 0 || iH > 24 || iM < 0 || iM > 60 || iS < 0) return false;
	OT.wHour = iH;
	OT.wMinute = iM;
	OT.wSecond = iS;
	OT.wMseconds = iMS;

	return true;
}


bool FSC_FitsX::CalcOBSTime(string & csTime, FITSXTIME & OT)
{
	int iH  = atoi(csTime.c_str());
	int iM  = atoi(csTime.substr(3).c_str());
	int iS  = atoi(csTime.substr(6).c_str());
	int iMS = atoi(csTime.substr(9, 3).c_str());
	int iUS = atoi(csTime.substr(12, 3).c_str());
	if (iH < 0 || iH > 24 || iM < 0 || iM > 60 || iS < 0) return false;
	OT.wHour = iH;
	OT.wMinute = iM;
	OT.wSecond = iS;
	OT.wMseconds = iMS;
	OT.wUseconds = iUS;

	return true;
}


double FSC_FitsX::CalcRA(string & csRA)
{
	int h = atoi(csRA.c_str());
	int m = atoi(csRA.substr(3).c_str());
	double s = atof(csRA.substr(6).c_str());
	return h + m / 60. + s / 3600.;
}


double FSC_FitsX::CalcHA(string & csHA)
{
	double dHA = atof(csHA.c_str());
	dHA = dHA / 360.0 * 24.0;
	return dHA;
}


double FSC_FitsX::CalcDEC(string & csDEC)
{
	double DEC;
	int split1 = csDEC.find(':');
	int split2 = csDEC.find(':', split1 + 1);
	if ((split1 == -1) || (split2 == -1))
	{
		printf("Error in FSC_FitsX > CalcDEC > find().\n");
		return 0.0;
	}
	int D    = abs(atoi(csDEC.c_str()));
	int M    = atoi(csDEC.substr(split1 + 1).c_str());
	double S = atof(csDEC.substr(split2 + 1).c_str());
	DEC = D + M / 60.0 + S / 3600.0;
	if (csDEC.find('-') != -1) DEC = -DEC;
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


string FSC_FitsX::GetHDUKey(int iPos)
{
	if (iPos >= m_iHDUNum) return ("");
	return m_vHDUKey.at(iPos);
}


string FSC_FitsX::GetHDUValue(int iPos)
{
	if (iPos >= m_iHDUNum) return ("");
	return m_vHDUValue.at(iPos);
}
