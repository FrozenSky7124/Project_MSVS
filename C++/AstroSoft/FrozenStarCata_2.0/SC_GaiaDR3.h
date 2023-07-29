#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _SC_GAIA_DR3_
#define _SC_GAIA_DR3_

class SC_GAIA_DR3
{
public:
	SC_GAIA_DR3();
	~SC_GAIA_DR3();

private:
	char* m_pFilePath;
	FILE* m_pFILE;

	char m_starID[32] = "";
	double m_starRa, m_starDe;
	double m_starPMRa, m_starPMDe;
	double m_starMag;

public:
	int loadFile(char* filePath);
	int testFile();
	int nextData(char* ID, double& Ra, double& De, double& pmRa, double& pmDe, double& Mag);
};

#endif _SC_GAIA_DR3_