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
 * Function : 打开指定的端口（同步或异步模式），成功后读取端口参数结构体
 * Parameter:
 *      _IN_  strPortNo   : 端口名称（端口号） 如： "COM1"
 *      _IN_  bOverlapped : 是否使用异步IO
 * Return   : [-1] 打开失败
 *            [ 1] 已存在句柄（端口已经打开）
 *            [ 0] 打开成功
 * Updated  : 2023-10-09 @FrozenSky
 */
int FSC_Serial::Open(const std::string & strPortNo, bool bOverlapped)
{
	// 端口句柄未正常释放（端口已经打开了）
	if (m_hComm != INVALID_HANDLE_VALUE)
		return 1;
	// 是否使用异步
	if (!bOverlapped)
	{
		m_hComm = CreateFile(strPortNo.c_str(), // 端口名称 (COMx)
			GENERIC_READ | GENERIC_WRITE,       // 端口属性为可读写
			0,                                  // 端口设备必须被独占性的访问
			NULL,                               // 无安全属性
			OPEN_EXISTING,                      // 端口设备必须使用OPEN_EXISTING参数
			FILE_ATTRIBUTE_NORMAL,              // 同步式 I/O
			0);                                 // 对于端口设备而言此参数必须为0
	}
	else
	{
		;
		m_hComm = CreateFile(strPortNo.c_str(), // 端口名称 (COMx)
			GENERIC_READ | GENERIC_WRITE,       // 端口属性为可读写
			0,                                  // 端口设备必须被独占性的访问
			NULL,                               // 无安全属性
			OPEN_EXISTING,                      // 端口设备必须使用OPEN_EXISTING参数
			FILE_ATTRIBUTE_NORMAL | 
			FILE_FLAG_OVERLAPPED,               // 异步式 I/O
			0);                                 // 对于端口设备而言此参数必须为0

		m_osRead.hEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_osWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_osWait.hEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	// 打开失败
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			0 == 0; // [TODO] 端口不存在
		return -1;
	}

	// 打开成功 保存端口句柄信息
	m_bOverlapped = bOverlapped;
	strcpy_s(m_strPortNo, sizeof(strPortNo.c_str()), strPortNo.c_str());
	GetCommState(m_hComm, &m_dcb);
	return 0;
}


/**
 * Function : 关闭端口
 * Return   : [-1] 关闭端口出现问题，端口句柄未正常释放
 *            [ 1] 该端口已经关闭了
 *            [ 0] 关闭成功，句柄已释放
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
 * Function : 设置端口参数(DCB)
 * Parameter:
 *      _IN_  dwBaudRate  : 波特率 CBR_115200 ... (see WinBase.h)
 *      _IN_  bParity     : 奇偶校验方法 0-4 = NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
 *      _IN_  bByteSize   : 数字位 4-8
 *      _IN_  bStopBits   : 停止位 0,1,2 = ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
 *      _IN_  dwFlagParity: 奇偶校验位是否允许
 * Return   : [-2] 设置端口参数出错
 *            [-1] 端口句柄无效（端口未打开）
 *            [ 0] 设置成功
 * Updated  : 2023-10-09 @FrozenSky
 */
int FSC_Serial::SetSerialPort(DWORD dwBaudRate, BYTE bParity, BYTE bByteSize, BYTE bStopBits, DWORD dwFlagParity)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
		return -1;
	GetCommState(m_hComm, &m_dcb);
	m_dcb.BaudRate = dwBaudRate;       // 波特率 CBR_115200 ...
	m_dcb.Parity   = bParity;          // 奇偶校验方法 0-4 = NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
	m_dcb.ByteSize = bByteSize;        // 数字位 4-8
	m_dcb.StopBits = bStopBits;        // 停止位 0,1,2 = ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
	m_dcb.fParity  = 1;                // 奇偶校验位是否允许
	if (!SetCommState(m_hComm, &m_dcb))
	{
		GetCommState(m_hComm, &m_dcb);
		return -2;
	}
	PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR); // 清除端口的输入输出缓冲区
	return 0;
}


/**
 * Function : 设置端口超时(COMMTIMEOUTS)
 * Parameter:
 *      _IN_  dwRIT       : 设置 ReadIntervalTimeout 读取间隔超时
 *      _IN_  dwRTTM      : 设置 ReadTotalTimeoutMultiplier 读取总超时系数
 *      _IN_  dwRTTC      : 设置 ReadTotalTimeoutConstant 读取总超时常数
 * Return   : [-2] 设置端口参数出错
 *            [-1] 端口句柄无效（端口未打开）
 *            [ 0] 设置成功
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
 * Function : 读取字节数据，指定要读取的数据长度，输出成功读取到的数据长度
 *            非异步将造成阻塞，需要设置读出超时时间 (see struct: COMMTIMEOUTS in WinBase.h)
 * Parameter:
 *     _OUT_  byteBuff    : 存储读出数据
 *      _IN_  dwNbToRead  : 要读取的数据最大长度
 *     _OUT_  dwNbRead    : 成功读出的数据长度
 * Return   : [-2] 读出错误
 *            [-1] 端口句柄无效（端口未打开）
 *            [ 0] 读出成功
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