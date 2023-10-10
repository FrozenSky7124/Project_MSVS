#include "FSC_Serial.h"


FSC_Serial::FSC_Serial()
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_bOverlapped = FALSE;
	strcpy_s(m_strPortNo, sizeof("COM0"), "COM0");
	memset(&m_osRead,  0, sizeof(OVERLAPPED));
	memset(&m_osWrite, 0, sizeof(OVERLAPPED));
	memset(&m_osWait,  0, sizeof(OVERLAPPED));
}

FSC_Serial::~FSC_Serial()
{
	Close();
}


/**
 * Function : ��ָ���Ķ˿ڣ�ͬ�����첽ģʽ�����ɹ����ȡ�˿ڲ����ṹ��
 * Parameter:
 *      _IN_  strPortNo   : �˿����ƣ��˿ںţ� �磺 "COM1"
 *      _IN_  bOverlapped : �Ƿ�ʹ���첽IO
 * Return   : [-1] ��ʧ��
 *            [ 1] �Ѵ��ھ�����˿��Ѿ��򿪣�
 *            [ 0] �򿪳ɹ�
 * Updated  : 2023-10-09 @FrozenSky
 */
int FSC_Serial::Open(const std::string & strPortNo, bool bOverlapped)
{
	// �˿ھ��δ�����ͷţ��˿��Ѿ����ˣ�
	if (m_hComm != INVALID_HANDLE_VALUE)
		return 1;
	// �Ƿ�ʹ���첽
	if (!bOverlapped)
	{
		m_hComm = CreateFile(strPortNo.c_str(), // �˿����� (COMx)
			GENERIC_READ | GENERIC_WRITE,       // �˿�����Ϊ�ɶ�д
			0,                                  // �˿��豸���뱻��ռ�Եķ���
			NULL,                               // �ް�ȫ����
			OPEN_EXISTING,                      // �˿��豸����ʹ��OPEN_EXISTING����
			FILE_ATTRIBUTE_NORMAL,              // ͬ��ʽ I/O
			0);                                 // ���ڶ˿��豸���Դ˲�������Ϊ0
	}
	else
	{
		;
		m_hComm = CreateFile(strPortNo.c_str(), // �˿����� (COMx)
			GENERIC_READ | GENERIC_WRITE,       // �˿�����Ϊ�ɶ�д
			0,                                  // �˿��豸���뱻��ռ�Եķ���
			NULL,                               // �ް�ȫ����
			OPEN_EXISTING,                      // �˿��豸����ʹ��OPEN_EXISTING����
			FILE_ATTRIBUTE_NORMAL | 
			FILE_FLAG_OVERLAPPED,               // �첽ʽ I/O
			0);                                 // ���ڶ˿��豸���Դ˲�������Ϊ0

		m_osRead.hEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_osWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_osWait.hEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	// ��ʧ��
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			0 == 0; // [TODO] �˿ڲ�����
		return -1;
	}

	// �򿪳ɹ� ����˿ھ����Ϣ
	m_bOverlapped = bOverlapped;
	strcpy_s(m_strPortNo, sizeof(strPortNo.c_str()), strPortNo.c_str());
	GetCommState(m_hComm, &m_dcb);
	return 0;
}


/**
 * Function : �رն˿�
 * Return   : [-1] �رն˿ڳ������⣬�˿ھ��δ�����ͷ�
 *            [ 1] �ö˿��Ѿ��ر���
 *            [ 0] �رճɹ���������ͷ�
 * Updated  : 2023-10-09 @FrozenSky
 */
int FSC_Serial::Close()
{
	if (m_hComm == INVALID_HANDLE_VALUE)
		return 1;
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		if (!CloseHandle(m_hComm))
			return -1;
		m_hComm = INVALID_HANDLE_VALUE;
	}
	return 0;
}


/**
 * Function : ���ö˿ڲ���(DCB)
 * Parameter:
 *      _IN_  dwBaudRate  : ������ CBR_115200 ... (see WinBase.h)
 *      _IN_  bParity     : ��żУ�鷽�� 0-4 = NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
 *      _IN_  bByteSize   : ����λ 4-8
 *      _IN_  bStopBits   : ֹͣλ 0,1,2 = ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
 *      _IN_  dwFlagParity: ��żУ��λ�Ƿ�����
 * Return   : [-2] ���ö˿ڲ�������
 *            [-1] �˿ھ����Ч���˿�δ�򿪣�
 *            [ 0] ���óɹ�
 * Updated  : 2023-10-09 @FrozenSky
 */
int FSC_Serial::SetSerialPort(DWORD dwBaudRate, BYTE bParity, BYTE bByteSize, BYTE bStopBits, DWORD dwFlagParity)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
		return -1;
	GetCommState(m_hComm, &m_dcb);
	m_dcb.BaudRate = dwBaudRate;       // ������ CBR_115200 ...
	m_dcb.Parity   = bParity;          // ��żУ�鷽�� 0-4 = NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
	m_dcb.ByteSize = bByteSize;        // ����λ 4-8
	m_dcb.StopBits = bStopBits;        // ֹͣλ 0,1,2 = ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
	m_dcb.fParity  = 1;                // ��żУ��λ�Ƿ�����
	if (!SetCommState(m_hComm, &m_dcb))
	{
		GetCommState(m_hComm, &m_dcb);
		return -2;
	}
	PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR); // ����˿ڵ��������������
	return 0;
}


/**
 * Function : ���ö˿ڳ�ʱ(COMMTIMEOUTS)
 * Parameter:
 *      _IN_  dwRIT       : ���� ReadIntervalTimeout ��ȡ�����ʱ
 *      _IN_  dwRTTM      : ���� ReadTotalTimeoutMultiplier ��ȡ�ܳ�ʱϵ��
 *      _IN_  dwRTTC      : ���� ReadTotalTimeoutConstant ��ȡ�ܳ�ʱ����
 * Return   : [-2] ���ö˿ڲ�������
 *            [-1] �˿ھ����Ч���˿�δ�򿪣�
 *            [ 0] ���óɹ�
 * Updated  : 2023-10-10 @FrozenSky
 */
int FSC_Serial::SetTimeout(DWORD dwRIT, DWORD dwRTTM, DWORD dwRTTC)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
		return -1;
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts(m_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = dwRIT;
	CommTimeOuts.ReadTotalTimeoutMultiplier = dwRTTM;
	CommTimeOuts.ReadTotalTimeoutConstant = dwRTTC;
	if (!SetCommTimeouts(m_hComm, &CommTimeOuts))
		return -2;
	PurgeComm(m_hComm, PURGE_RXABORT | PURGE_RXCLEAR);
	return 0;
}


/**
 * Function : ��ȡ�ֽ����ݣ�ָ��Ҫ��ȡ�����ݳ��ȣ�����ɹ���ȡ�������ݳ���
 *            ���첽�������������Ҫ���ö�����ʱʱ�� (see struct: COMMTIMEOUTS in WinBase.h)
 * Parameter:
 *     _OUT_  byteBuff    : �洢��������
 *      _IN_  dwNbToRead  : Ҫ��ȡ��������󳤶�
 *     _OUT_  dwNbRead    : �ɹ����������ݳ���
 * Return   : [-2] ��������
 *            [-1] �˿ھ����Ч���˿�δ�򿪣�
 *            [ 0] �����ɹ�
 * Updated  : 2023-10-09 @FrozenSky
 */
int FSC_Serial::Read(BYTE* byteBuff, DWORD dwNbToRead, DWORD* dwNbRead)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
		return -1;
	if (!m_bOverlapped)
	{
		BOOL bReadStat = ReadFile(m_hComm, byteBuff, dwNbToRead, dwNbRead, NULL);
		if (!bReadStat)
		{
			PurgeComm(m_hComm, PURGE_RXABORT | PURGE_RXCLEAR);
			return -2;
		}
	}
	return 0;
}


/**
 * Function : Other member function
 */

HANDLE FSC_Serial::GetHandle()
{
	return m_hComm;
}