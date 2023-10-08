#include "FSC_Serial.h"


FSC_Serial::FSC_Serial()
{
	m_hComm = INVALID_HANDLE_VALUE;
}

FSC_Serial::~FSC_Serial()
{
}


/**
 * Open Serial
 * Function : 打开指定的串口
 * Parameter:
 *     _IN_  strPortNo  : 端口名称（串口号） 如： "COM1"
 *     _IN_  dpArrayObs : Obs stars array (TYPE_double * 2 * nObs)
 *     _IN_  nObs : Obs stars count
 *     _IN_  dpArrayNav : Nav stars array (TYPE_double * 2 * nNav) (in radian)
 *     _IN_  nNav : Nav stars count
 * Return   : [-1]: 打开失败
 *            [ 1]: 已存在句柄（端口已经打开）
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

		/*m_osRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_osWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_osWait.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);*/
	}
	m_bOverlapped = bOverlapped;

	// 打开失败
	if (m_hComm == INVALID_HANDLE_VALUE)
		return -1;
	/*GetCommState(m_hComm, &m_dcb);*/
	/*m_csPortNo = tcPortNo;*/
	strcpy_s(m_strPortNo, sizeof(strPortNo.c_str()), strPortNo.c_str());
	return 0;
}