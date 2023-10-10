/**********************************************************************
* Name:      FSC_Serial
* Function:  Simple serial port communication
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
	int Close();
	int SetSerialPort(DWORD dwBaudRate, BYTE bParity, BYTE bByteSize, BYTE bStopBits, DWORD dwFlagParity = 1);
	int SetTimeout(DWORD dwRIT, DWORD dwRTTM, DWORD dwRTTC);
	int Read(BYTE* byteBuff, DWORD dwNbToRead, DWORD* dwNbRead);

	HANDLE GetHandle();

private:
	HANDLE m_hComm;       // �˿ھ��
	BOOL m_bOverlapped;   // �Ƿ�ʹ���첽IO
	CHAR m_strPortNo[32]; // �˿ں�
	DCB m_dcb;            // �˿ڲ����ṹ��
	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;
	OVERLAPPED m_osWait;
};

#endif