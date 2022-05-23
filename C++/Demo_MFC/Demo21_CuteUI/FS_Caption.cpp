// FS_Caption.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo21_CuteUI.h"
#include "FS_Caption.h"


// FS_Caption

IMPLEMENT_DYNAMIC(FS_Caption, CStatic)

FS_Caption::FS_Caption()
{

}

FS_Caption::~FS_Caption()
{

}

void FS_Caption::Init(int x, int y, int W, int H)
{
	MoveWindow(x, y, W, H);
}

HBRUSH FS_Caption::CtlColor(CDC * pDC, UINT)
{
	if (!pDC) return NULL;
	// ����һ���ջ�ˢ�����������ˢ�������þ�̬�ؼ��ı���͸��
	//HBRUSH hr = (HBRUSH)GetStockObject(NULL_BRUSH);
	// ����һ����ɫ��ˢ�����������ˢ�������þ�̬�ؼ��ı�����ɫ
	HBRUSH hr = CreateSolidBrush(RGB(60, 60, 60));
	// ʹ���ֱ���͸��/������ɫ
	pDC->SetBkMode(TRANSPARENT); // ���ֱ���͸��
	//pDC->SetBkColor(RGB(31, 31, 31)); // ���ֱ�����ɫ
	// ����������ɫ
	pDC->SetTextColor(RGB(102, 204, 255));
	// ��������
	CFont font;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 20; // �����߼�����ĸ߶�
	lf.lfWeight = FW_BOLD; // �����߼�����Ϊ����
	::lstrcpy(lf.lfFaceName, _T("Bitsumishi")); // ��������
	font.CreateFontIndirect(&lf); // ʹ���߼����� lf ����һ�� CFont ����
	pDC->SelectObject(&font); // Ӧ������

	return hr;
	//return HBRUSH();
}


BEGIN_MESSAGE_MAP(FS_Caption, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// FS_Caption ��Ϣ�������


