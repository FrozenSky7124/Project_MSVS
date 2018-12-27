//======================================================================
// �ļ��� Dib_Ex.h
// ���ݣ� �豸�޹�λͼ��-ͷ�ļ�
// ���ܣ� 
//        ��1��λͼ�ļ����뱣�棻
//        ��2��λͼ��Ϣ�Ļ�ȡ��
//        ��3��λͼ���ݵĻ�ȡ��
//        ��3��λͼ����ʾ��
//        ��4��λͼ��ת����
//        ��5��λͼ����жϣ�
// �������ܣ� 
//        ��1����BYTE*���͵����ԭʼ����ָ�����ͼ��
//        ��2����ʾλͼ��ָ������
//======================================================================

#pragma once

#include "afx.h"
#include "math.h"
#include <iostream>
#include <algorithm>

class CDib : public CObject
{
public:
    // ���캯������ʼ�����ݳ�Ա
    CDib(void);

    // �����������ͷ��ڴ�ռ�
    ~CDib(void);

    // ���ļ�����λͼ
    BOOL LoadFromFile(LPCTSTR lpszPath);

	// Ex:���ⲿ���ݼ���λͼ
	BOOL LoadFromBuffer(BYTE* pBmpDataBuffer, LONG lWidth, LONG lHeight, UINT iBitCount = 8);

    // ��λͼ���浽�ļ�
    BOOL SaveToFile(LPCTSTR lpszPath);

    // ��ȡλͼ�ļ���
    LPCTSTR GetFileName();

    // ��ȡλͼ���
    LONG GetWidth();

    // ��ȡλͼ�߶�
    LONG GetHeight();

    // ��ȡλͼ�Ŀ�Ⱥ͸߶�
    CSize GetDimension();  
    
    // ��ȡλͼ��С
    DWORD GetSize();

    // ��ȡ����������ռλ��
    WORD GetBitCount();

    // ��ȡÿ��������ռ�ֽ���
    UINT GetLineByte();

    // ��ȡλͼ��ɫ��
    DWORD GetNumOfColor();

    // ��ȡλͼ��ɫ��
    LPRGBQUAD GetRgbQuad();

    // ��ȡλͼ����
    LPBYTE GetData();
      
    // ��ʾλͼ
    BOOL Draw(CDC *pDC, CPoint origin, CSize size);

	// ��ʾλͼ��ָ������
	BOOL CDib::Draw(CDC *pDC, CPoint srcOri, CSize srcSize, CPoint destOri, CSize destSize);

    // 24λ��ɫλͼת8λ�Ҷ�λͼ
    BOOL RgbToGrade();

    // 8λ�Ҷ�λͼת24λ��ɫλͼ
    BOOL GradeToRgb();

    // �ж��Ƿ�����ɫ��
    BOOL HasRgbQuad();

    // �ж��Ƿ��ǻҶ�ͼ
    BOOL IsGrade();

    // �ж�λͼ�Ƿ���Ч
    BOOL IsValid();

protected:
    // ����λͼ��ɫ����
    DWORD CalcRgbQuadLength();

    // ������ɫ�����ɵ�ɫ��
    BOOL MakePalette();

    // ����ռ�
    void Empty(BOOL bFlag = TRUE);

private:
    // λͼ�ļ���
    char m_fileName[_MAX_PATH];

    // λͼ�ļ�ͷָ��    
    LPBITMAPFILEHEADER m_lpBmpFileHeader; // ��Ҫ��̬������ͷ� 

    // λͼָ�루������λͼ�ļ�ͷ���������ݣ�
	LPBYTE m_lpDib;                       // ��Ҫ��̬������ͷ�

    // λͼ��Ϣָ��
    LPBITMAPINFO m_lpBmpInfo;

	// λͼ��Ϣͷָ��
	LPBITMAPINFOHEADER m_lpBmpInfoHeader;  

    // λͼ��ɫ��ָ��
	LPRGBQUAD m_lpRgbQuad; 

    // λͼ����ָ��
	LPBYTE m_lpData; 

    // ��ɫ����
	HPALETTE m_hPalette;

    // �Ƿ�����ɫ��
    BOOL m_bHasRgbQuad;

    // λͼ�Ƿ���Ч
    BOOL m_bValid;

	// Ex:Ԥ������λͼ��ɫ����ɫ��
//	UINT m_iClrUse;
};


class FSC_DibX : public CDib
{

};