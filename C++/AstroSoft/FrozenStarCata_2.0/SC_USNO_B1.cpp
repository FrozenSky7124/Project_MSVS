#pragma once
#include "SC_USNO_B1.h"
#include "memory.h"
#include "stdio.h"

SC_USNO_B1::SC_USNO_B1(char* cpData)
{
	m_cpSrcData = cpData;
}

SC_USNO_B1::~SC_USNO_B1()
{

}

double SC_USNO_B1::makeRa(int n)
{
	char* cpRecord = m_cpSrcData + 80 * n + 0;
	__int32 iRa = 0;
	memcpy_s(&iRa, 4, cpRecord, 4);
	//printf("RA:%10.6f\n", iRa * 0.01); // final value = data value * 0.01 arcsec
	return iRa * 0.01 / 3600.0;
}

double SC_USNO_B1::makeDe(int n)
{
	char* cpRecord = m_cpSrcData + 80 * n + 4;
	__int32 iDe = 0;
	memcpy_s(&iDe, 4, cpRecord, 4);
	//printf("RA:%10.6f\n", iDe * 0.01); // final value = data value * 0.01 arcsec
	return iDe * 0.01 / 3600.0;
}