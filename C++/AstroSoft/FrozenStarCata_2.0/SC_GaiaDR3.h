#pragma once
#include <stdio.h>
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

public:
	int loadFile(char* filePath);
	int test();
};

#endif _SC_GAIA_DR3_