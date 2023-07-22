#pragma once
#include "SC_GaiaDR3.h"

SC_GAIA_DR3::SC_GAIA_DR3()
{
	m_pFilePath = NULL;
	m_pFILE = NULL;
}

SC_GAIA_DR3::~SC_GAIA_DR3()
{
	if (m_pFilePath != NULL)
	{
		delete[] m_pFilePath;
		m_pFilePath = NULL;
	}
	if (m_pFILE != NULL)
	{
		fclose(m_pFILE);
		m_pFILE = NULL;
	}
}

int SC_GAIA_DR3::loadFile(char* filePath)
{
	int r = fopen_s(&m_pFILE, filePath, "r");
	if (r != 0)
	{
		printf("[ERR] loadFile > fopen\n");
		return -1;
	}
	size_t len = strlen(filePath);
	m_pFilePath = new char[len + 1];
	strcpy_s(m_pFilePath, (len + 1) * sizeof(char), filePath);
	return 0;
}

int SC_GAIA_DR3::test()
{
	int result = 0;
	unsigned int lineLen = 256;
	const char* sep = ",";
	char* cValue;
	char* line = new char[lineLen];
	if (fgets(line, lineLen * sizeof(char), m_pFILE))
	{
		if (strlen(line) == 0) result = -1;
		else
		{
			cValue = strtok(line, sep);
			cValue = strtok(NULL, sep);
			cValue = strtok(NULL, sep);
			cValue = strtok(NULL, sep);
			cValue = strtok(NULL, sep);
			cValue = strtok(NULL, sep);
		}
	}
	else
	{
		result = -1;
	}

	delete[] line;
	return result;
}
