//=================================================
// Class   : Serial Port X Powered by FrozenSky
// Function: SerialX Class to operate serial port.
//=================================================
#include "stdafx.h"
#include "SerialX.h"

SerialX::SerialX()
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_csPortNo = _T("");
	m_bOverlapped = FALSE;
	memset(&m_osRead, 0, sizeof(OVERLAPPED));
	memset(&m_osWrite, 0, sizeof(OVERLAPPED));
	memset(&m_osWait, 0, sizeof(OVERLAPPED));
	memset(&m_dcb, 0, sizeof(DCB));
}

SerialX::~SerialX()
{
	// Close serial port.
	if (m_hComm != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hComm);
	}
}

HANDLE SerialX::GetHandle()
{
	return m_hComm;
}

CString SerialX::GetPortNo()
{
	return m_csPortNo;
}

DWORD SerialX::GetBaudRate()
{
	return m_dcb.BaudRate;
}

BYTE SerialX::GetParity()
{
	return m_dcb.Parity;
}

BYTE SerialX::GetByteSize()
{
	return m_dcb.ByteSize;
}

BYTE SerialX::GetStopBits()
{
	return m_dcb.StopBits;
}

int SerialX::OpenSerialPort(TCHAR* tcPortNo, BOOL bOverlapped)
{
	// Serial port already opened.
	if (m_hComm != INVALID_HANDLE_VALUE)
		return 1;
	// Use Overlapped or not.
	if (!bOverlapped)
	{
		m_hComm = CreateFile(tcPortNo, //�˿�����(COMx)
			GENERIC_READ | GENERIC_WRITE, //�˿�����Ϊ�ɶ�д
			0, //�˿��豸���뱻��ռ�Եķ���
			NULL, //�ް�ȫ����
			OPEN_EXISTING, //�˿��豸����ʹ��OPEN_EXISTING����
			FILE_ATTRIBUTE_NORMAL, //ͬ��ʽ I/O
			0); //���ڶ˿��豸���Դ˲�������Ϊ0
		m_bOverlapped = FALSE;
	}
	else
	{
		m_hComm = CreateFile(tcPortNo, //�˿�����(COMx)
			GENERIC_READ | GENERIC_WRITE, //�˿�����Ϊ�ɶ�д
			0, //�˿��豸���뱻��ռ�Եķ���
			NULL, //�ް�ȫ����
			OPEN_EXISTING, //�˿��豸����ʹ��OPEN_EXISTING����
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�첽ʽ I/O
			0); //���ڶ˿��豸���Դ˲�������Ϊ0
		m_bOverlapped = TRUE;

		m_osRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_osWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_osWait.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	// Serial port open Faild.
	if (m_hComm == INVALID_HANDLE_VALUE)
		return 2;
	GetCommState(m_hComm, &m_dcb);
	m_csPortNo = tcPortNo;
	return 0;
}


int SerialX::CloseSerialPort()
{
	// Close serial port.
	if (m_hComm == INVALID_HANDLE_VALUE)
		return 1;
	if (m_hComm != INVALID_HANDLE_VALUE) {
		if (!CloseHandle(m_hComm))
			return 2;
		m_hComm = INVALID_HANDLE_VALUE;
	}
	return 0;
}


int SerialX::SetSerialPort(DWORD dwBaudRate, BYTE bParity, BYTE bByteSize, BYTE bStopBits, DWORD dwFlagParity)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
		return 1;
	GetCommState(m_hComm, &m_dcb);
	m_dcb.BaudRate = dwBaudRate;       //������
	m_dcb.Parity = bParity;            //��żУ�鷽�� 0-4 = None,Odd,Even,Mark,Space
	m_dcb.ByteSize = bByteSize;        //����λ 4-8
	m_dcb.StopBits = bStopBits;        //ֹͣλ 0,1,2 = 1, 1.5, 2
	m_dcb.fParity = 1;                 //��żУ��λ Enable parity checking
	if (!SetCommState(m_hComm, &m_dcb))
	{
		GetCommState(m_hComm, &m_dcb);
		return 2;
	}
	PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return 0;
}


int SerialX::Read(BYTE* byteData, DWORD dwByteToRead, DWORD* dwByteRead)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
		return 1;
	if (!m_bOverlapped)
	{
		BOOL bReadStat = ReadFile(m_hComm, byteData, dwByteToRead, dwByteRead, NULL);
		if (!bReadStat)
		{
			TRACE(_T("Faild in ReadFile (SYN).\n"));
			PurgeComm(m_hComm, PURGE_RXABORT | PURGE_RXCLEAR);
			return 2;
		}
	}
	else
	{
		BOOL bReadStat = ReadFile(m_hComm, byteData, dwByteToRead, dwByteRead, &m_osRead);
		if (!bReadStat)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				// GetOverlappedResult���������һ��������ΪTRUE��������һֱ�ȴ���ֱ����������ɻ����ڴ�������ء�
				BOOL bResult = GetOverlappedResult(m_hComm, &m_osRead, dwByteRead, TRUE);
				if (!bResult)
				{
					TRACE(_T("Faild in ReadFile (ASYN).\n"));
					PurgeComm(m_hComm, PURGE_RXABORT | PURGE_RXCLEAR);
					return 2;
				}
			}
		}
	}
	return 0;
}


int SerialX::Write(BYTE* byteData, DWORD dwByteToSend, DWORD* dwByteSent)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
		return 1;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError(m_hComm, &dwErrorFlags, &ComStat);
	if (!m_bOverlapped)
	{
		bWriteStat = WriteFile(m_hComm, byteData, dwByteToSend, dwByteSent, NULL);
		if (!bWriteStat) //ͬ��I/O д��ʧ��
		{
			TRACE(_T("Faild in WriteFile (SYN).\n"));
			PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			return 2;
		}
	}
	else
	{
		bWriteStat = WriteFile(m_hComm, byteData, dwByteToSend, dwByteSent, &m_osWrite);
		if (!bWriteStat)
		{
			if (GetLastError() == ERROR_IO_PENDING) //�첽I/O��GetLastError()��������ERROR_IO_PENDING,�����������ڽ��ж�����
			{
				BOOL bResult = GetOverlappedResult(m_hComm, &m_osWrite, dwByteSent, TRUE);
				if (!bResult)
				{
					TRACE(_T("Faild in WriteFile (ASYN).\n"));
					PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
					return 2;
				}
			}
		}
	}
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	return 0;
}


int SerialX::SetMask(DWORD dwEvtMask)
{
	if (!SetCommMask(m_hComm, dwEvtMask))
		return 1;
	return 0;
}