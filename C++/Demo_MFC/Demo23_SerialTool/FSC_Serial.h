/**********************************************************************
* Name:      FSC_Serial
* Function:  Serial port communication
* Author:    FrozenSky
* Created:   2018-06-01
* Updated:   2023-10-07
* Copyright: FrozenSky@Avalon
* License:   MIT
**********************************************************************/

#ifndef _FSC_SERIAL_H_
#define _FSC_SERIAL_H_

#pragma once

#include <iostream>
#include "Windows.h"

class FSC_Serial
{
public:
	FSC_Serial();
	~FSC_Serial();

	int Open(const std::string & strPortNo, bool bOverlapped);
private:
	HANDLE m_hComm; // 存储已经打开的串口句柄
	BOOL m_bOverlapped;
	char m_strPortNo[32];
};

#endif