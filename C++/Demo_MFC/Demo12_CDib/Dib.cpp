//======================================================================
// �ļ��� Dib.cpp
// ���ݣ� �豸�޹�λͼ��-Դ�ļ�
// ���ܣ� ��1��λͼ�ļ����뱣�棻
//        ��2��λͼ��Ϣ�Ļ�ȡ��
//        ��3��λͼ���ݵĻ�ȡ��
//        ��3��λͼ����ʾ��
//        ��4��λͼ��ת����
//        ��5��λͼ����жϣ�
//======================================================================

#include "StdAfx.h"
#include "Dib.h"

//=======================================================
// �������ܣ� ���캯������ʼ�����ݳ�Ա
// ��������� ��
// ����ֵ��   ��
//=======================================================
CDib::CDib(void)
{
    // ���ݳ�Ա��ʼ��
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
}

//=======================================================
// �������ܣ� �����������ͷ��ڴ�ռ�
// ��������� ��
// ����ֵ��   ��
//=======================================================
CDib::~CDib(void)
{
    // ����ռ�
    Empty();
}

//=======================================================
// �������ܣ� ���ļ�����λͼ
// ��������� LPCTSTR lpszPath-������λͼ�ļ�·��
// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::LoadFromFile(LPCTSTR lpszPath)
{
    // ��¼λͼ�ļ���
    strcpy(m_fileName, lpszPath);

    // �Զ�ģʽ��λͼ�ļ�
    CFile dibFile;
    if(!dibFile.Open(m_fileName, CFile::modeRead | CFile::shareDenyWrite))
    {
        return FALSE;
    }

    // ����ռ�
    Empty(FALSE); 
    
    // Ϊλͼ�ļ�ͷ����ռ䣬����ʼ��Ϊ0
    m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
    memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER)); 

    // ��ȡλͼ�ļ�ͷ
    int nCount = dibFile.Read((void *)m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));
    if(nCount != sizeof(BITMAPFILEHEADER)) 
    {
        return FALSE;
    } 

    // �жϴ��ļ��ǲ���λͼ�ļ�����0x4d42������BM����
    if(m_lpBmpFileHeader->bfType == 0x4d42)
    {
        // ��λͼ�ļ�

        // �����λͼ�ļ�ͷ�Ŀռ��С������ռ䲢��ʼ��Ϊ0
        DWORD dwDibSize = dibFile.GetLength() - sizeof(BITMAPFILEHEADER);
        m_lpDib = new BYTE[dwDibSize];
        memset(m_lpDib, 0, dwDibSize);

        // ��ȡ��λͼ�ļ�ͷ����������
        dibFile.Read(m_lpDib, dwDibSize);

        // �ر�λͼ�ļ�
        dibFile.Close();

        // ����λͼ��Ϣָ��
        m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;

        // ����λͼ��Ϣͷָ��
        m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;

        // ����λͼ��ɫ��ָ��
        m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);

        // ���λͼû������λͼʹ�õ���ɫ����������
        if(m_lpBmpInfoHeader->biClrUsed == 0)
        {
            m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
        }

        // ������ɫ����
        DWORD dwRgbQuadLength = CalcRgbQuadLength();

        // ����λͼ����ָ��
        m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;

        // �ж��Ƿ�����ɫ��
        if(m_lpRgbQuad == (LPRGBQUAD)m_lpData)
        {
            m_lpRgbQuad = NULL;    // ��λͼ��ɫ��ָ���ÿ�
            m_bHasRgbQuad = FALSE; // ����ɫ��
        }
        else
        {
            m_bHasRgbQuad = TRUE;  // ����ɫ��
            MakePalette();         // ������ɫ�����ɵ�ɫ��
        }        

        // ����λͼ��С����Ϊ�ܶ�λͼ�ļ��������ô��
        m_lpBmpInfoHeader->biSizeImage = GetSize();

        // λͼ��Ч
        m_bValid = TRUE;

        return TRUE;
    }
    else
    {
        // ����λͼ�ļ�
        m_bValid = FALSE;

        return FALSE;
    }     

}

//=======================================================
// �������ܣ� ��λͼ���浽�ļ�
// ��������� LPCTSTR lpszPath-λͼ�ļ�����·��
// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::SaveToFile(LPCTSTR lpszPath)
{
    // ��дģʽ���ļ�
	CFile dibFile;
	if(!dibFile.Open(lpszPath, CFile::modeCreate | CFile::modeReadWrite 
		| CFile::shareExclusive))
    {
        return FALSE;
    }

    // ��¼λͼ�ļ���
    strcpy(m_fileName, lpszPath);

    // ���ļ�ͷ�ṹд���ļ�
    dibFile.Write(m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));

    // ���ļ���Ϣͷ�ṹд���ļ�
    dibFile.Write(m_lpBmpInfoHeader, sizeof(BITMAPINFOHEADER));

    // ������ɫ����
    DWORD dwRgbQuadlength = CalcRgbQuadLength();

    // �������ɫ��Ļ�������ɫ��д���ļ�
    if(dwRgbQuadlength != 0)
    {
        dibFile.Write(m_lpRgbQuad, dwRgbQuadlength);
    }                                                        

    // ��λͼ����д���ļ�
    DWORD dwDataSize = GetLineByte() * GetHeight();
    dibFile.Write(m_lpData, dwDataSize);

    // �ر��ļ�
    dibFile.Close();
		
    return TRUE;
}

//=======================================================
// �������ܣ� ��ȡλͼ�ļ���
// ��������� ��
// ����ֵ��   LPCTSTR-λͼ�ļ���
//=======================================================
LPCTSTR CDib::GetFileName()
{
    return m_fileName;
}

//=======================================================
// �������ܣ� ��ȡλͼ���
// ��������� ��
// ����ֵ��   LONG-λͼ���
//=======================================================
LONG CDib::GetWidth()
{
    return m_lpBmpInfoHeader->biWidth;
}

//=======================================================
// �������ܣ� ��ȡλͼ�߶�
// ��������� ��
// ����ֵ��   LONG-λͼ�߶�
//=======================================================
LONG CDib::GetHeight()
{
    return m_lpBmpInfoHeader->biHeight;
}

//=======================================================
// �������ܣ� ��ȡλͼ�Ŀ�Ⱥ͸߶�
// ��������� ��
// ����ֵ��   CSize-λͼ�Ŀ�Ⱥ͸߶�
//=======================================================
CSize CDib::GetDimension()
{
    return CSize(GetWidth(), GetHeight());
}

//=======================================================
// �������ܣ� ��ȡλͼ��С
// ��������� ��
// ����ֵ��   DWORD-λͼ��С
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
// �������ܣ� ��ȡ����������ռλ��
// ��������� ��
// ����ֵ��   WORD-����������ռλ��
//=======================================================
WORD CDib::GetBitCount()
{
    return m_lpBmpInfoHeader->biBitCount;
}       

//=======================================================
// �������ܣ� ��ȡÿ��������ռ�ֽ���
// ��������� ��
// ����ֵ��   UINT-ÿ��������ռ�ֽ���
//=======================================================
UINT CDib::GetLineByte()
{ 
    return (GetWidth() * GetBitCount() / 8 + 3) / 4 * 4;;
}

//=======================================================
// �������ܣ� ��ȡλͼ��ɫ��
// ��������� ��
// ����ֵ��   DWORD-λͼ��ɫ��
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
// �������ܣ� ����λͼ��ɫ����
// ��������� ��
// ����ֵ��   DWORD-λͼ��ɫ����
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
// �������ܣ� ��ȡλͼ��ɫ��
// ��������� ��
// ����ֵ��   LPRGBQUAD-λͼ��ɫ��ָ��
//=======================================================
LPRGBQUAD CDib::GetRgbQuad()
{
    return m_lpRgbQuad;
}

//=======================================================
// �������ܣ� ��ȡλͼ����
// ��������� ��
// ����ֵ��   LPBYTE-λͼ����ָ��
//=======================================================
LPBYTE CDib::GetData()
{
    return m_lpData;
}

//=======================================================
// �������ܣ� ������ɫ�����ɵ�ɫ��
// ��������� ��
// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::MakePalette()
{
    // ������ɫ����
    DWORD dwRgbQuadLength = CalcRgbQuadLength();

    // �����ɫ����Ϊ0���������߼���ɫ��
	if(dwRgbQuadLength == 0) 
    {
        return FALSE;
    }

	//ɾ���ɵĵ�ɫ�����
	if(m_hPalette != NULL) 
    {
        DeleteObject(m_hPalette);
        m_hPalette = NULL;
    }

	// ���뻺��������ʼ��Ϊ0
    DWORD dwNumOfColor = GetNumOfColor();
    DWORD dwSize = 2 * sizeof(WORD) + dwNumOfColor * sizeof(PALETTEENTRY);
	LPLOGPALETTE lpLogPalette = (LPLOGPALETTE) new BYTE[dwSize];
    memset(lpLogPalette, 0, dwSize);

    // �����߼���ɫ��
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

	// �����߼���ɫ��
	m_hPalette = CreatePalette(lpLogPalette);

	// �ͷŻ�����
	delete [] lpLogPalette;

    return TRUE;
}

//=======================================================
// �������ܣ� ��ʾλͼ
// ���������
//            CDC *pDC-�豸����ָ��
//            CPoint origin-��ʾ������������Ͻ�
//            CSize size-��ʾ��������ĳߴ�
// ����ֵ��
//            BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::Draw(CDC *pDC, CPoint origin, CSize size)
{
    // λͼ��Ч���޷����ƣ����ش���
    if(!IsValid())
    {
        return FALSE;
    }

    // �ɵĵ�ɫ����
	HPALETTE hOldPalette = NULL;

	// ���λͼָ��Ϊ�գ��򷵻�FALSE
	if(m_lpDib == NULL) 
    {
        return FALSE;
    }

	// ���λͼ�е�ɫ�壬��ѡ���豸������
	if(m_hPalette != NULL) 
    {
		hOldPalette = SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
	}

	// ����λͼ����ģʽ
	pDC->SetStretchBltMode(COLORONCOLOR);

	// ��λͼ��pDC��ָ����豸�Ͻ�����ʾ
	StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y, size.cx, size.cy,
		0, 0, GetWidth(), GetHeight(), m_lpData, m_lpBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	// �ָ��ɵĵ�ɫ��
	if(hOldPalette != NULL)
    {
        SelectPalette(pDC->GetSafeHdc(), hOldPalette, TRUE);
    }

    return TRUE;
}

//=======================================================
// �������ܣ� 24λ��ɫλͼת8λ�Ҷ�λͼ
// ��������� ��
// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::RgbToGrade()
{
    // λͼ��Ч��ʧ�ܷ���
    if(!IsValid())
    {
        return FALSE;
    }

    // ����24λλͼ��ʧ�ܷ���
    if(GetBitCount() != 24)
    {
        return FALSE;
    }

    // ��ѹ��λͼ��ʧ�ܷ���
    if(m_lpBmpInfoHeader->biCompression != BI_RGB)
    {
        return FALSE;
    }

    // ������ǻҶ�λͼ������Ҫת��
    if(!IsGrade())
    {
        // ��ȡԭλͼ��Ϣ
        LONG lHeight = GetHeight();
        LONG lWidth = GetWidth();
        UINT uLineByte = GetLineByte();

        // ����Ҷ�λͼ��������ռ�
        UINT uGradeBmpLineByte = (lWidth + 3) / 4 * 4;
        DWORD dwGradeBmpDataSize = uGradeBmpLineByte * lHeight; 

        // ����Ҷ�λͼ����ռ�
        DWORD dwGradeBmpSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + dwGradeBmpDataSize;

        // ���ûҶ�λͼ�ļ�ͷ
        LPBITMAPFILEHEADER lpGradeBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
        memset(lpGradeBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
        lpGradeBmpFileHeader->bfType = 0x4d42;
        lpGradeBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwGradeBmpSize;
        lpGradeBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
                                          + sizeof(RGBQUAD) * 256;
        lpGradeBmpFileHeader->bfReserved1 = 0;
        lpGradeBmpFileHeader->bfReserved2 = 0;            

        // Ϊ�Ҷ�λͼ����ռ䣬����ʼ��Ϊ0
        LPBYTE lpGradeBmp = (LPBYTE)new BYTE[dwGradeBmpSize];
        memset(lpGradeBmp, 0, dwGradeBmpSize);

        // ���ûҶ�λͼ��Ϣͷ
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

        // ���ûҶ�λͼ��ɫ��
        LPRGBQUAD lpGradeBmpRgbQuad = (LPRGBQUAD)(lpGradeBmp + sizeof(BITMAPINFOHEADER));

        // ��ʼ��8λ�Ҷ�ͼ�ĵ�ɫ����Ϣ
        LPRGBQUAD lpRgbQuad;               
        for(int k = 0; k < 256; k++)
        {
            lpRgbQuad = (LPRGBQUAD)(lpGradeBmpRgbQuad + k);
            lpRgbQuad->rgbBlue = k; 
            lpRgbQuad->rgbGreen = k;
            lpRgbQuad->rgbRed = k;
            lpRgbQuad->rgbReserved = 0;
        }

        // �Ҷ�λͼ���ݴ���
        BYTE r, g, b; 
        LPBYTE lpGradeBmpData = (LPBYTE)(lpGradeBmp + sizeof(BITMAPINFOHEADER) 
                                         + sizeof(RGBQUAD) * 256);
        // ������ɫת��
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

        // �ͷ�ԭ��λͼ�ռ�
        Empty(FALSE);

        // �����趨ԭλͼָ��ָ��
        m_lpBmpFileHeader = lpGradeBmpFileHeader;
        m_lpDib = lpGradeBmp;
        m_lpBmpInfo = (LPBITMAPINFO)(lpGradeBmp);
        m_lpBmpInfoHeader = lpGradeBmpInfoHeader;
        m_lpRgbQuad = lpGradeBmpRgbQuad;
        m_lpData = lpGradeBmpData;

        // ������ɫ���־
        m_bHasRgbQuad = TRUE;  
        // ����λͼ��Ч��־
        m_bValid = TRUE;
        // ���ɵ�ɫ��
        MakePalette();
    }

    return TRUE;   
}   

//=======================================================
// �������ܣ� 8λ�Ҷ�λͼת24λ��ɫλͼ
// ��������� ��
// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::GradeToRgb()
{
    // λͼ��Ч��ʧ���˳�
    if(!IsValid())
    {
        return FALSE;
    }

    // ����8λλͼ��ʧ���˳�
    if(GetBitCount() != 8)
    {
        return FALSE;
    }

    // ��ѹ��λͼ��ʧ�ܷ���
    if(m_lpBmpInfoHeader->biCompression != BI_RGB)
    {
        return FALSE;
    }

    // �ǻҶ�ͼʱ������ת��
    if(IsGrade())
    {
        // ��ȡԭλͼ��Ϣ
        LONG lHeight = GetHeight();
        LONG lWidth = GetWidth();
        UINT uLineByte = GetLineByte();

        // �����ɫλͼ��������ռ�
        UINT uColorBmpLineByte = (lWidth * 24 / 8 + 3) / 4 * 4;
        DWORD dwColorBmpDataSize = uColorBmpLineByte * lHeight; 

        // �����ɫλͼ����ռ�
        DWORD dwColorBmpSize = sizeof(BITMAPINFOHEADER) + dwColorBmpDataSize;

        // ���ò�ɫλͼ�ļ�ͷ
        LPBITMAPFILEHEADER lpColorBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
        memset(lpColorBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
        lpColorBmpFileHeader->bfType = 0x4d42;
        lpColorBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwColorBmpSize;
        lpColorBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        lpColorBmpFileHeader->bfReserved1 = 0;
        lpColorBmpFileHeader->bfReserved2 = 0;    

        // Ϊ��ɫλͼ����ռ䣬����ʼ��Ϊ0
        LPBYTE lpColorBmp = (LPBYTE)new BYTE[dwColorBmpSize];
        memset(lpColorBmp, 0, dwColorBmpSize);

        // ���ò�ɫλͼ��Ϣͷ
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

        // ��ɫλͼ���ݴ���
        LPBYTE lpColorBmpData = (LPBYTE)(lpColorBmp + sizeof(BITMAPINFOHEADER));
        // ������ɫת��
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

        // �ͷ�ԭ��λͼ�ռ�
        Empty(FALSE);

        // �����趨ԭλͼָ��ָ��
        m_lpBmpFileHeader = lpColorBmpFileHeader;
        m_lpDib = lpColorBmp;
        m_lpBmpInfo = (LPBITMAPINFO)(lpColorBmp);
        m_lpBmpInfoHeader = lpColorBmpInfoHeader;
        m_lpRgbQuad = NULL;
        m_lpData = lpColorBmpData;

        // ������ɫ���־
        m_bHasRgbQuad = FALSE;  
        // ����λͼ��Ч��־
        m_bValid = TRUE;        
    }        

    return TRUE;   
}   
 
//=======================================================
// �������ܣ� �ж��Ƿ�����ɫ��
// ��������� ��
// ����ֵ��   �жϽ����TRUE-������ɫ��FALSE-������ɫ��
//=======================================================
BOOL CDib::HasRgbQuad()
{
    return m_bHasRgbQuad;
}

//=======================================================
// �������ܣ� �ж��Ƿ��ǻҶ�ͼ
// ��������� ��
// ����ֵ��   �жϽ����TRUE-�ǻҶ�ͼ��FALSE-�ǲ�ɫͼ
//=======================================================
BOOL CDib::IsGrade()
{
    return (GetBitCount() < 9 && GetBitCount() > 0);
}

//=======================================================
// �������ܣ� �ж�λͼ�Ƿ���Ч
// ��������� ��
// ����ֵ��   �жϽ����TRUE-λͼ��Ч��FALSE-λͼ��Ч
//=======================================================
BOOL CDib::IsValid()
{
    return m_bValid;
}

//=======================================================
// �������ܣ� ����ռ�
// ��������� BOOL bFlag-TRUE ȫ����գ�FALSE �������
// ����ֵ��   ��
//=======================================================
void CDib::Empty(BOOL bFlag)
{
    // �ļ������
    if(bFlag)
    {
        strcpy(m_fileName, "");
    }      

    // �ͷ�λͼ�ļ�ͷָ��ռ�
    if(m_lpBmpFileHeader != NULL)
    {
        delete [] m_lpBmpFileHeader;
        m_lpBmpFileHeader = NULL;
    }    

    // �ͷ�λͼָ��ռ�
    if(m_lpDib != NULL)
    {
        delete [] m_lpDib;
        m_lpDib = NULL;
        m_lpBmpInfo = NULL;
        m_lpBmpInfoHeader = NULL;
        m_lpRgbQuad = NULL;
        m_lpData = NULL;           
    }       

    // �ͷŵ�ɫ��
    if(m_hPalette != NULL)
    {
        DeleteObject(m_hPalette);
        m_hPalette = NULL;
    }    

    // ���ò�����ɫ��
    m_bHasRgbQuad = FALSE;
    
    // ����λͼ��Ч
    m_bValid = FALSE;

}  
