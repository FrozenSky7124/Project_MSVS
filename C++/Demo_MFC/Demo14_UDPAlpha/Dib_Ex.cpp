//======================================================================
// 文件： Dib_Ex.cpp
// 内容： 设备无关位图类-源文件
// 功能： 
//        （1）位图的加载与保存；
//        （2）位图信息的获取；
//        （3）位图数据的获取；
//        （3）位图的显示；
//        （4）位图的转换；
//        （5）位图相关判断；
// 新增功能： 
//        （1）从BYTE*类型的相机原始数据指针加载图像
//======================================================================

#include "StdAfx.h"
#include "Dib_Ex.h"

//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 无
// 返回值：   无
//=======================================================
CDib::CDib(void)
{
    // 数据成员初始化
    strcpy(m_fileName, "");
    m_lpBmpFileHeader = NULL;
    m_lpDib = NULL;   
    m_lpBmpInfo = NULL;
    m_lpBmpInfoHeader = NULL;
    m_lpRgbQuad = NULL;
    m_lpData = NULL;
    m_hPalette = NULL;
    m_bHasRgbQuad = FALSE;
    m_bValid = FALSE;
	m_lpRgbQuad_Mem = NULL;
}

//=======================================================
// 函数功能： 析构函数，释放内存空间
// 输入参数： 无
// 返回值：   无
//=======================================================
CDib::~CDib(void)
{
    // 清理空间
    Empty();
}

//=======================================================
// 函数功能： 从文件加载位图
// 输入参数： LPCTSTR lpszPath-待加载位图文件路径
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::LoadFromFile(LPCTSTR lpszPath)
{
    // 记录位图文件名
    strcpy(m_fileName, lpszPath);

    // 以读模式打开位图文件
    CFile dibFile;
    if(!dibFile.Open(m_fileName, CFile::modeRead | CFile::shareDenyWrite))
    {
        return FALSE;
    }

    // 清理空间
    Empty(FALSE); 
    
    // 为位图文件头分配空间，并初始化为0
    m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
    memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER)); 

    // 读取位图文件头
    int nCount = dibFile.Read((void *)m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));
    if(nCount != sizeof(BITMAPFILEHEADER)) 
    {
        return FALSE;
    } 

    // 判断此文件是不是位图文件（“0x4d42”代表“BM”）
    if(m_lpBmpFileHeader->bfType == 0x4d42)
    {
        // 是位图文件

        // 计算除位图文件头的空间大小，分配空间并初始化为0
        DWORD dwDibSize = dibFile.GetLength() - sizeof(BITMAPFILEHEADER);
        m_lpDib = new BYTE[dwDibSize];
        memset(m_lpDib, 0, dwDibSize);

        // 读取除位图文件头的所有数据
        dibFile.Read(m_lpDib, dwDibSize);

        // 关闭位图文件
        dibFile.Close();

        // 设置位图信息指针
        m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;

        // 设置位图信息头指针
        m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;

        // 设置位图颜色表指针
        m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);

        // 如果位图没有设置位图使用的颜色数，设置它
        if(m_lpBmpInfoHeader->biClrUsed == 0)
        {
            m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
        }

        // 计算颜色表长度
        DWORD dwRgbQuadLength = CalcRgbQuadLength();

        // 设置位图数据指针
        m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;

        // 判断是否有颜色表
        if(m_lpRgbQuad == (LPRGBQUAD)m_lpData)
        {
            m_lpRgbQuad = NULL;    // 将位图颜色表指针置空
            m_bHasRgbQuad = FALSE; // 无颜色表
        }
        else
        {
            m_bHasRgbQuad = TRUE;  // 有颜色表
            MakePalette();         // 根据颜色表生成调色板
        }        

        // 设置位图大小（因为很多位图文件都不设置此项）
        m_lpBmpInfoHeader->biSizeImage = GetSize();

        // 位图有效
        m_bValid = TRUE;

        return TRUE;
    }
    else
    {
        // 不是位图文件
        m_bValid = FALSE;

        return FALSE;
    }     

}

//=======================================================
// 函数功能： 从外部位图数据加载位图
// 输入参数： BYTE* pBmpDataBuffer, LONG lWidth, LONG lHeight, UINT iBitCount = 8/24
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::LoadFromBuffer(BYTE* pBmpDataBuffer, LONG lWidth, LONG lHeight, UINT iBitCount)
{
	if (pBmpDataBuffer == NULL)
		return FALSE;

	// 计算位图每行的字节数
	UINT uBmpLineByte = (lWidth * iBitCount + 31) / 8;
	uBmpLineByte = uBmpLineByte / 4 * 4;
	// 计算位图数据区字节数
	DWORD dwBmpDataSize = uBmpLineByte * lHeight;

	// 计算位图除文件头字节数
	DWORD dwBmpSize;
	if (iBitCount > 8)
		dwBmpSize = sizeof(BITMAPINFOHEADER) + dwBmpDataSize;
	else if (iBitCount == 8)
		dwBmpSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + dwBmpDataSize;

	// 设置BMP文件头
	if (m_lpBmpFileHeader == NULL)
	{
		m_lpBmpFileHeader = (LPBITMAPFILEHEADER) new BYTE[sizeof(BITMAPFILEHEADER)];
		memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
	}
	m_lpBmpFileHeader->bfType = 0x4d42;
	m_lpBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwBmpSize;
	m_lpBmpFileHeader->bfReserved1 = 0;
	m_lpBmpFileHeader->bfReserved2 = 0;
	if (iBitCount > 8)
		m_lpBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	else if (iBitCount == 8)
		m_lpBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	
	// 为BMP数据区分配空间
	if (m_lpDib == NULL)
	{
		m_lpDib = (LPBYTE) new BYTE[dwBmpSize];
		memset(m_lpDib, 0, dwBmpSize);
	}

	// 设置BMP信息
	m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;

	// 设置BMP信息头
	m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;
	m_lpBmpInfoHeader->biBitCount = iBitCount;
	m_lpBmpInfoHeader->biClrImportant = 0;
	m_lpBmpInfoHeader->biCompression = BI_RGB;
	m_lpBmpInfoHeader->biHeight = lHeight;
	m_lpBmpInfoHeader->biWidth = lWidth;
	m_lpBmpInfoHeader->biPlanes = 1;
	m_lpBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	m_lpBmpInfoHeader->biSizeImage = dwBmpDataSize;
	//m_lpBmpInfoHeader->biXPelsPerMeter = 2834;
	//m_lpBmpInfoHeader->biYPelsPerMeter = 2834;
	m_lpBmpInfoHeader->biXPelsPerMeter = 0;
	m_lpBmpInfoHeader->biYPelsPerMeter = 0;
	if (iBitCount > 8)
		m_lpBmpInfoHeader->biClrUsed = 0;
	else if (iBitCount == 8)
		m_lpBmpInfoHeader->biClrUsed = 256;

	// 设置8位BMP颜色表
	m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + sizeof(BITMAPINFOHEADER));
	if (iBitCount == 8)
	{
		memcpy(m_lpRgbQuad, m_lpRgbQuad_Mem, m_iClrUse * sizeof(RGBQUAD));
	}

	// 设置BMP图像数据
	m_lpData = (LPBYTE)(m_lpDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetNumOfColor());
	memcpy(m_lpData, pBmpDataBuffer, dwBmpDataSize);

	// 计算颜色表长度
	DWORD dwRgbQuadLength = CalcRgbQuadLength();

	// 判断是否有颜色表
	if (m_lpRgbQuad == (LPRGBQUAD)m_lpData)
	{
		m_lpRgbQuad = NULL;    // 将位图颜色表指针置空
		m_bHasRgbQuad = FALSE; // 无颜色表
	}
	else
	{
		m_bHasRgbQuad = TRUE;  // 有颜色表
		MakePalette();         // 根据颜色表生成调色板
	}

	// 位图有效
	m_bValid = TRUE;

	return TRUE;
}

//=======================================================
// 函数功能： 将位图保存到文件
// 输入参数： LPCTSTR lpszPath-位图文件保存路径
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::SaveToFile(LPCTSTR lpszPath)
{
    // 以写模式打开文件
	CFile dibFile;
	if(!dibFile.Open(lpszPath, CFile::modeCreate | CFile::modeReadWrite 
		| CFile::shareExclusive))
    {
        return FALSE;
    }

    // 记录位图文件名
    strcpy(m_fileName, lpszPath);

    // 将文件头结构写进文件
    dibFile.Write(m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));

    // 将文件信息头结构写进文件
    dibFile.Write(m_lpBmpInfoHeader, sizeof(BITMAPINFOHEADER));

    // 计算颜色表长度
    DWORD dwRgbQuadlength = CalcRgbQuadLength();

    // 如果有颜色表的话，将颜色表写进文件
    if(dwRgbQuadlength != 0)
    {
        dibFile.Write(m_lpRgbQuad, dwRgbQuadlength);
    }                                                        

    // 将位图数据写进文件
    DWORD dwDataSize = GetLineByte() * GetHeight();
    dibFile.Write(m_lpData, dwDataSize);

    // 关闭文件
    dibFile.Close();
		
    return TRUE;
}

//=======================================================
// 函数功能： 获取位图文件名
// 输入参数： 无
// 返回值：   LPCTSTR-位图文件名
//=======================================================
LPCTSTR CDib::GetFileName()
{
    return m_fileName;
}

//=======================================================
// 函数功能： 获取位图宽度
// 输入参数： 无
// 返回值：   LONG-位图宽度
//=======================================================
LONG CDib::GetWidth()
{
    return m_lpBmpInfoHeader->biWidth;
}

//=======================================================
// 函数功能： 获取位图高度
// 输入参数： 无
// 返回值：   LONG-位图高度
//=======================================================
LONG CDib::GetHeight()
{
    return m_lpBmpInfoHeader->biHeight;
}

//=======================================================
// 函数功能： 获取位图的宽度和高度
// 输入参数： 无
// 返回值：   CSize-位图的宽度和高度
//=======================================================
CSize CDib::GetDimension()
{
    return CSize(GetWidth(), GetHeight());
}

//=======================================================
// 函数功能： 获取位图大小
// 输入参数： 无
// 返回值：   DWORD-位图大小
//=======================================================
DWORD CDib::GetSize()
{
    if(m_lpBmpInfoHeader->biSizeImage != 0)
    {
        return m_lpBmpInfoHeader->biSizeImage;
    }
    else
    {       
        return GetWidth() * GetHeight();
    }
}

//=======================================================
// 函数功能： 获取单个像素所占位数
// 输入参数： 无
// 返回值：   WORD-单个像素所占位数
//=======================================================
WORD CDib::GetBitCount()
{
    return m_lpBmpInfoHeader->biBitCount;
}       

//=======================================================
// 函数功能： 获取每行像素所占字节数
// 输入参数： 无
// 返回值：   UINT-每行像素所占字节数
//=======================================================
UINT CDib::GetLineByte()
{ 
    return (GetWidth() * GetBitCount() / 8 + 3) / 4 * 4;;
}

//=======================================================
// 函数功能： 获取位图颜色数
// 输入参数： 无
// 返回值：   DWORD-位图颜色数
//=======================================================
DWORD CDib::GetNumOfColor()
{
    UINT dwNumOfColor;     

    if ((m_lpBmpInfoHeader->biClrUsed == 0) 
        && (m_lpBmpInfoHeader->biBitCount < 9))
	{
		switch (m_lpBmpInfoHeader->biBitCount)
		{
		    case 1: dwNumOfColor = 2; break;
		    case 4: dwNumOfColor = 16; break;
		    case 8: dwNumOfColor = 256;
		}
	}
    else
    {
        dwNumOfColor = m_lpBmpInfoHeader->biClrUsed;
    }  		

    return dwNumOfColor; 
}

//=======================================================
// 函数功能： 计算位图颜色表长度
// 输入参数： 无
// 返回值：   DWORD-位图颜色表长度
//=======================================================
DWORD CDib::CalcRgbQuadLength()
{
    DWORD dwNumOfColor = GetNumOfColor();
    if(dwNumOfColor > 256)
    {
        dwNumOfColor = 0;
    }
    return  dwNumOfColor * sizeof(RGBQUAD);
}

//=======================================================
// 函数功能： 获取位图颜色表
// 输入参数： 无
// 返回值：   LPRGBQUAD-位图颜色表指针
//=======================================================
LPRGBQUAD CDib::GetRgbQuad()
{
    return m_lpRgbQuad;
}

//=======================================================
// 函数功能： 获取位图数据
// 输入参数： 无
// 返回值：   LPBYTE-位图数据指针
//=======================================================
LPBYTE CDib::GetData()
{
    return m_lpData;
}

//=======================================================
// 函数功能： 根据颜色表生成调色板
// 输入参数： 无
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::MakePalette()
{
    // 计算颜色表长度
    DWORD dwRgbQuadLength = CalcRgbQuadLength();

    // 如果颜色表长度为0，则不生成逻辑调色板
	if(dwRgbQuadLength == 0) 
    {
        return FALSE;
    }

	//删除旧的调色板对象
	if(m_hPalette != NULL) 
    {
        DeleteObject(m_hPalette);
        m_hPalette = NULL;
    }

	// 申请缓冲区，初始化为0
    DWORD dwNumOfColor = GetNumOfColor();
    DWORD dwSize = 2 * sizeof(WORD) + dwNumOfColor * sizeof(PALETTEENTRY);
	LPLOGPALETTE lpLogPalette = (LPLOGPALETTE) new BYTE[dwSize];
    memset(lpLogPalette, 0, dwSize);

    // 生成逻辑调色板
	lpLogPalette->palVersion = 0x300;
	lpLogPalette->palNumEntries = dwNumOfColor;
	LPRGBQUAD lpRgbQuad = (LPRGBQUAD) m_lpRgbQuad;
	for(int i = 0; i < dwNumOfColor; i++) 
    {
		lpLogPalette->palPalEntry[i].peRed = lpRgbQuad->rgbRed;
		lpLogPalette->palPalEntry[i].peGreen = lpRgbQuad->rgbGreen;
		lpLogPalette->palPalEntry[i].peBlue = lpRgbQuad->rgbBlue;
		lpLogPalette->palPalEntry[i].peFlags = 0;
		lpRgbQuad++;
	}

	// 创建逻辑调色板
	m_hPalette = CreatePalette(lpLogPalette);

	// 释放缓冲区
	delete [] lpLogPalette;

    return TRUE;
}

//=======================================================
// 函数功能： 显示位图
// 输入参数：
//            CDC *pDC-设备环境指针
//            CPoint origin-显示矩形区域的左上角
//            CSize size-显示矩形区域的尺寸
// 返回值：
//            BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::Draw(CDC *pDC, CPoint origin, CSize size)
{
    // 位图无效，无法绘制，返回错误
    if(!IsValid())
    {
        return FALSE;
    }

    // 旧的调色板句柄
	HPALETTE hOldPalette = NULL;

	// 如果位图指针为空，则返回FALSE
	if(m_lpDib == NULL) 
    {
        return FALSE;
    }

	// 如果位图有调色板，则选进设备环境中
	if(m_hPalette != NULL) 
    {
		hOldPalette = SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
	}

	// 设置位图伸缩模式
	pDC->SetStretchBltMode(COLORONCOLOR);

	// 将位图在pDC所指向的设备上进行显示
	StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y, size.cx, size.cy,
		0, 0, GetWidth(), GetHeight(), m_lpData, m_lpBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	// 恢复旧的调色板
	if(hOldPalette != NULL)
    {
        SelectPalette(pDC->GetSafeHdc(), hOldPalette, TRUE);
    }

    return TRUE;
}

//=======================================================
// 函数功能： 24位彩色位图转8位灰度位图
// 输入参数： 无
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::RgbToGrade()
{
    // 位图无效，失败返回
    if(!IsValid())
    {
        return FALSE;
    }

    // 不是24位位图，失败返回
    if(GetBitCount() != 24)
    {
        return FALSE;
    }

    // 是压缩位图，失败返回
    if(m_lpBmpInfoHeader->biCompression != BI_RGB)
    {
        return FALSE;
    }

    // 如果不是灰度位图，才需要转换
    if(!IsGrade())
    {
        // 获取原位图信息
        LONG lHeight = GetHeight();
        LONG lWidth = GetWidth();
        UINT uLineByte = GetLineByte();

        // 计算灰度位图数据所需空间
        UINT uGradeBmpLineByte = (lWidth + 3) / 4 * 4;
        DWORD dwGradeBmpDataSize = uGradeBmpLineByte * lHeight; 

        // 计算灰度位图所需空间
        DWORD dwGradeBmpSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + dwGradeBmpDataSize;

        // 设置灰度位图文件头
        LPBITMAPFILEHEADER lpGradeBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
        memset(lpGradeBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
        lpGradeBmpFileHeader->bfType = 0x4d42;
        lpGradeBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwGradeBmpSize;
        lpGradeBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
                                          + sizeof(RGBQUAD) * 256;
        lpGradeBmpFileHeader->bfReserved1 = 0;
        lpGradeBmpFileHeader->bfReserved2 = 0;            

        // 为灰度位图分配空间，并初始化为0
        LPBYTE lpGradeBmp = (LPBYTE)new BYTE[dwGradeBmpSize];
        memset(lpGradeBmp, 0, dwGradeBmpSize);

        // 设置灰度位图信息头
        LPBITMAPINFOHEADER lpGradeBmpInfoHeader = (LPBITMAPINFOHEADER)(lpGradeBmp);
        lpGradeBmpInfoHeader->biBitCount = 8;
        lpGradeBmpInfoHeader->biClrImportant = 0;
        lpGradeBmpInfoHeader->biClrUsed = 256;
        lpGradeBmpInfoHeader->biCompression = BI_RGB;
        lpGradeBmpInfoHeader->biHeight = lHeight;
        lpGradeBmpInfoHeader->biPlanes = 1;
        lpGradeBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
        lpGradeBmpInfoHeader->biSizeImage = dwGradeBmpDataSize;
        lpGradeBmpInfoHeader->biWidth = lWidth;
        lpGradeBmpInfoHeader->biXPelsPerMeter = m_lpBmpInfoHeader->biXPelsPerMeter;
        lpGradeBmpInfoHeader->biYPelsPerMeter = m_lpBmpInfoHeader->biYPelsPerMeter;

        // 设置灰度位图颜色表
        LPRGBQUAD lpGradeBmpRgbQuad = (LPRGBQUAD)(lpGradeBmp + sizeof(BITMAPINFOHEADER));

        // 初始化8位灰度图的调色板信息
        LPRGBQUAD lpRgbQuad;               
        for(int k = 0; k < 256; k++)
        {
            lpRgbQuad = (LPRGBQUAD)(lpGradeBmpRgbQuad + k);
            lpRgbQuad->rgbBlue = k; 
            lpRgbQuad->rgbGreen = k;
            lpRgbQuad->rgbRed = k;
            lpRgbQuad->rgbReserved = 0;
        }

        // 灰度位图数据处理
        BYTE r, g, b; 
        LPBYTE lpGradeBmpData = (LPBYTE)(lpGradeBmp + sizeof(BITMAPINFOHEADER) 
                                         + sizeof(RGBQUAD) * 256);
        // 进行颜色转换
        for(int i = 0; i < lHeight; i++)
        {
            for(int j = 0; j < lWidth; j++)
            {
                b = m_lpData[i * uLineByte + 3 * j];
                g = m_lpData[i * uLineByte + 3 * j + 1];
                r = m_lpData[i * uLineByte + 3 * j + 2];
                lpGradeBmpData[i * uGradeBmpLineByte + j] = (BYTE)(0.299 * r + 0.587 * g + 0.114 * b); 
            }
        }

        // 释放原有位图空间
        Empty(FALSE);

        // 重新设定原位图指针指向
        m_lpBmpFileHeader = lpGradeBmpFileHeader;
        m_lpDib = lpGradeBmp;
        m_lpBmpInfo = (LPBITMAPINFO)(lpGradeBmp);
        m_lpBmpInfoHeader = lpGradeBmpInfoHeader;
        m_lpRgbQuad = lpGradeBmpRgbQuad;
        m_lpData = lpGradeBmpData;

        // 设置颜色表标志
        m_bHasRgbQuad = TRUE;  
        // 设置位图有效标志
        m_bValid = TRUE;
        // 生成调色板
        MakePalette();
    }

    return TRUE;   
}   

//=======================================================
// 函数功能： 8位灰度位图转24位彩色位图
// 输入参数： 无
// 返回值：   BOOL-TRUE 成功；FALSE 失败
//=======================================================
BOOL CDib::GradeToRgb()
{
    // 位图无效，失败退出
    if(!IsValid())
    {
        return FALSE;
    }

    // 不是8位位图，失败退出
    if(GetBitCount() != 8)
    {
        return FALSE;
    }

    // 是压缩位图，失败返回
    if(m_lpBmpInfoHeader->biCompression != BI_RGB)
    {
        return FALSE;
    }

    // 是灰度图时，才需转换
    if(IsGrade())
    {
        // 获取原位图信息
        LONG lHeight = GetHeight();
        LONG lWidth = GetWidth();
        UINT uLineByte = GetLineByte();

        // 计算彩色位图数据所需空间
        UINT uColorBmpLineByte = (lWidth * 24 / 8 + 3) / 4 * 4;
        DWORD dwColorBmpDataSize = uColorBmpLineByte * lHeight; 

        // 计算彩色位图所需空间
        DWORD dwColorBmpSize = sizeof(BITMAPINFOHEADER) + dwColorBmpDataSize;

        // 设置彩色位图文件头
        LPBITMAPFILEHEADER lpColorBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
        memset(lpColorBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
        lpColorBmpFileHeader->bfType = 0x4d42;
        lpColorBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwColorBmpSize;
        lpColorBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        lpColorBmpFileHeader->bfReserved1 = 0;
        lpColorBmpFileHeader->bfReserved2 = 0;    

        // 为彩色位图分配空间，并初始化为0
        LPBYTE lpColorBmp = (LPBYTE)new BYTE[dwColorBmpSize];
        memset(lpColorBmp, 0, dwColorBmpSize);

        // 设置彩色位图信息头
        LPBITMAPINFOHEADER lpColorBmpInfoHeader = (LPBITMAPINFOHEADER)(lpColorBmp);
        lpColorBmpInfoHeader->biBitCount = 24;
        lpColorBmpInfoHeader->biClrImportant = 0;
        lpColorBmpInfoHeader->biClrUsed = 0;
        lpColorBmpInfoHeader->biCompression = BI_RGB;
        lpColorBmpInfoHeader->biHeight = lHeight;
        lpColorBmpInfoHeader->biPlanes = 1;
        lpColorBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
        lpColorBmpInfoHeader->biSizeImage = dwColorBmpDataSize;
        lpColorBmpInfoHeader->biWidth = lWidth;
        lpColorBmpInfoHeader->biXPelsPerMeter = m_lpBmpInfoHeader->biXPelsPerMeter;
        lpColorBmpInfoHeader->biYPelsPerMeter = m_lpBmpInfoHeader->biYPelsPerMeter;

        // 彩色位图数据处理
        LPBYTE lpColorBmpData = (LPBYTE)(lpColorBmp + sizeof(BITMAPINFOHEADER));
        // 进行颜色转换
        for(int i = 0; i < lHeight; i++)
        {
            for(int j = 0; j < lWidth; j++)
            {
                BYTE btValue = m_lpData[i * uLineByte + j]; 
                lpColorBmpData[i * uColorBmpLineByte + 3 * j] = btValue;
                lpColorBmpData[i * uColorBmpLineByte + 3 * j + 1] = btValue;
                lpColorBmpData[i * uColorBmpLineByte + 3 * j + 2] = btValue;  
            }
        }

        // 释放原有位图空间
        Empty(FALSE);

        // 重新设定原位图指针指向
        m_lpBmpFileHeader = lpColorBmpFileHeader;
        m_lpDib = lpColorBmp;
        m_lpBmpInfo = (LPBITMAPINFO)(lpColorBmp);
        m_lpBmpInfoHeader = lpColorBmpInfoHeader;
        m_lpRgbQuad = NULL;
        m_lpData = lpColorBmpData;

        // 设置颜色表标志
        m_bHasRgbQuad = FALSE;  
        // 设置位图有效标志
        m_bValid = TRUE;        
    }        

    return TRUE;   
}   
 
//=======================================================
// 函数功能： 判断是否含有颜色表
// 输入参数： 无
// 返回值：   判断结果：TRUE-含有颜色表；FALSE-不含颜色表
//=======================================================
BOOL CDib::HasRgbQuad()
{
    return m_bHasRgbQuad;
}

//=======================================================
// 函数功能： 判断是否是灰度图
// 输入参数： 无
// 返回值：   判断结果：TRUE-是灰度图；FALSE-是彩色图
//=======================================================
BOOL CDib::IsGrade()
{
    return (GetBitCount() < 9 && GetBitCount() > 0);
}

//=======================================================
// 函数功能： 判断位图是否有效
// 输入参数： 无
// 返回值：   判断结果：TRUE-位图有效；FALSE-位图无效
//=======================================================
BOOL CDib::IsValid()
{
    return m_bValid;
}

//=======================================================
// 函数功能： 清理空间
// 输入参数： BOOL bFlag-TRUE 全部清空；FALSE 部分清空
// 返回值：   无
//=======================================================
void CDib::Empty(BOOL bFlag)
{
    // 文件名清空
    if(bFlag)
    {
        strcpy(m_fileName, "");
    }      

	// 释放预构建颜色表内存
	if (m_lpRgbQuad_Mem != NULL)
	{
		delete [] m_lpRgbQuad_Mem;
		m_lpRgbQuad_Mem = NULL;
	}

    // 释放位图文件头指针空间
    if(m_lpBmpFileHeader != NULL)
    {
        delete [] m_lpBmpFileHeader;
        m_lpBmpFileHeader = NULL;
    }    

    // 释放位图指针空间
    if(m_lpDib != NULL)
    {
        delete [] m_lpDib;
        m_lpDib = NULL;
        m_lpBmpInfo = NULL;
        m_lpBmpInfoHeader = NULL;
        m_lpRgbQuad = NULL;
        m_lpData = NULL;           
    }       

    // 释放调色板
    if(m_hPalette != NULL)
    {
        DeleteObject(m_hPalette);
        m_hPalette = NULL;
    }    

    // 设置不含颜色表
    m_bHasRgbQuad = FALSE;
    
    // 设置位图无效
    m_bValid = FALSE;

}  

//=======================================================
// 函数功能： Ex:预构建指定的颜色表内存空间
// 输入参数： 无
// 返回值：   TRUE:成功 FALSE:失败
//=======================================================
BOOL CDib::MakeRgbQuadMem(WORD wBitCount)
{
	if (wBitCount == 8)
	{
		// 为预构建的颜色表分配空间
		m_lpRgbQuad_Mem = (LPRGBQUAD)new BYTE[sizeof(RGBQUAD) * 256];
		if (!m_lpRgbQuad_Mem) return FALSE;

		// 设置预构建颜色表的颜色数
		m_iClrUse = 256;

		for (int i = 0; i < 256; i++)
		{
			LPRGBQUAD lpTempRgbQuad = m_lpRgbQuad_Mem + i;
			lpTempRgbQuad->rgbBlue = i;
			lpTempRgbQuad->rgbGreen = i;
			lpTempRgbQuad->rgbRed = i;
			lpTempRgbQuad->rgbReserved = 0;
		}
		
		return TRUE;
	}
	return FALSE;
}