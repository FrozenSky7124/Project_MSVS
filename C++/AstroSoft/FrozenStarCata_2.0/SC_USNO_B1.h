#pragma once
#ifndef _SC_USNO_B1_
#define _SC_USNO_B1_

class SC_USNO_B1
{
public:
	SC_USNO_B1(char* cpData);
	~SC_USNO_B1();

private:
	char* m_cpSrcData;

public:
	double makeRa(int n);
	double makeDe(int n);
};

#endif _SC_USNO_B1_