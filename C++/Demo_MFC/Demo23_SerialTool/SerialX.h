//=================================================
// Class   : Serial Port X Powered by FrozenSky
// Function: SerialX Class to operate serial port.
//=================================================
#pragma once

class SerialX
{
private:
	CString m_csPortNo;
	HANDLE m_hComm;
	DCB m_dcb;
	BOOL m_bOverlapped;


public:
	SerialX();
	~SerialX();

	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;
	OVERLAPPED m_osWait;

	HANDLE GetHandle();
	CString GetPortNo();
	DWORD GetBaudRate();
	BYTE GetParity();
	BYTE GetByteSize();
	BYTE GetStopBits();

	//=================================================
	// Function: Open serial port
	// Parameter: <TCHAR*>Port number ex:"COM1".
	// 0: Serial port open Success.
	// 1: Serial port already opened.
	// 2: Serial port open Faild.
	//=================================================
	int OpenSerialPort(TCHAR* tcPortNo, BOOL bOverlapped = FALSE);

	//=================================================
	// Function: Close serial port
	// Parameter: 
	// 0: Serial port close Success.
	// 1: Serial port already closed.
	// 2: Unknown Error in CloseHandle(m_hComm)
	//=================================================
	int CloseSerialPort();

	//=================================================
	// Function: Config serial port
	// Parameter: DWORD dwBaudRate, BYTE bParity, BYTE bByteSize, BYTE bStopBits, DWORD dwFlagParity
	// ������
	// ��żУ�鷽�� 0-4 = None,Odd,Even,Mark,Space
	// ����λ 4-8
	// ֹͣλ 0,1,2 = 1, 1.5, 2
	// ��żУ��λ Enable parity checking
	// 0: Success.
	// 1: Serial Port not open.
	// 2: Serial reset.
	//=================================================
	int SetSerialPort(DWORD dwBaudRate, BYTE bParity, BYTE bByteSize, BYTE bStopBits, DWORD dwFlagParity = 1);


	//=================================================
	// Function: Write
	// Parameter: BYTE* byteData, DWORD dwByteToSend, DWORD* dwByteSent
	// 0: Success.
	// 1: Serial Port not open.
	// 2: Write Faild.
	//=================================================
	int Write(BYTE* byteData, DWORD dwByteToSend, DWORD* dwByteSent);

	//=================================================
	// Function: Read
	// Parameter: BYTE* byteData, DWORD dwByteToRead, DWORD* dwByteRead
	// 0: Success.
	// 1: Serial Port not open.
	// 2: Read Faild.
	//=================================================
	int Read(BYTE* byteData, DWORD dwByteToRead, DWORD* dwByteRead);

	//=================================================
	// Function: SetMask
	// 0: Success.
	// 1: Faild.
	//=================================================
	int SetMask(DWORD dwEvtMask);
};