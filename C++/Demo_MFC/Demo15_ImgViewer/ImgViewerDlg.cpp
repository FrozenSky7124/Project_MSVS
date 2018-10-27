
// ImgViewerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo15_ImgViewer.h"
#include "ImgViewerDlg.h"
#include "afxdialogex.h"
#include "FSC_FitsX\FSC_FitsX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ImgViewerDlg �Ի���



ImgViewerDlg::ImgViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO15_IMGVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ImgViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ImgViewerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// ImgViewerDlg ��Ϣ�������

BOOL ImgViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	// ���ÿؼ�λ��
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	GetClientRect(&m_mainRect);
	m_mainRect.left = (iScreenWidth - (1024 + 20)) / 2;
	m_mainRect.top = (iScreenHeight - (768 + 20 + 30)) / 2;
	m_mainRect.right = m_mainRect.left + 1024 + 20;
	m_mainRect.bottom = m_mainRect.top + 768 + 20 + 30;
	CalcWindowRect(&m_mainRect); //���ݿͻ�����С�����㴰���С
	SetWindowPos(NULL, m_mainRect.left, m_mainRect.top, m_mainRect.Width(), m_mainRect.Height(), SWP_NOZORDER); //���ô���λ��

	GetClientRect(&m_mainRect);
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&m_viewRect);
	m_viewRect.left = m_mainRect.left + 10;
	m_viewRect.top = m_mainRect.top + 10;
	m_viewRect.right = m_mainRect.right - 10;
	m_viewRect.bottom = m_mainRect.bottom - 30 - 10;
	GetDlgItem(IDC_STATIC_VIEW)->MoveWindow(m_viewRect);

	CRect rectTips;
	GetDlgItem(IDC_STATIC_TIPS)->GetClientRect(&rectTips);
	rectTips = CRect(m_mainRect.left + 10, m_mainRect.bottom - 30, m_mainRect.right - 10, m_mainRect.bottom);
	GetDlgItem(IDC_STATIC_TIPS)->MoveWindow(rectTips);

	// ���ó�Ա����
	m_pCDC = GetDlgItem(IDC_STATIC_VIEW)->GetDC();
	m_SubWin = NULL;
	m_lpCDibDataBuffer = NULL;

	GetClientRect(&m_mainRect);
	ClientToScreen(&m_mainRect);
	m_SubWinParam.iSubWidth = 256;
	m_SubWinParam.iSubHeight = 256;
	m_SubWinParam.iMainWinPosX = m_mainRect.left;
	m_SubWinParam.iMainWinPosY = m_mainRect.top;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ImgViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR ImgViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ImgViewerDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();

	// FSC_FitsX Test
	FSC_FitsX testFX;
	testFX.OpenFitsFile(_T("F:\\FrozenSky_Data\\Astronomy\\TeleImages\\36585-000000-0000-20181023125044050.fit"));
}


void ImgViewerDlg::OnCancel()
{
	// TODO:
	
	// �ͷ�Ԥ��ͼ�����ݻ���
	if (m_lpCDibDataBuffer)
		delete[] m_lpCDibDataBuffer;

	// �ͷ�Ԥ������
	if (m_SubWin != NULL)
	{
		m_SubWin->OnCancel();
		delete m_SubWin;
		m_SubWin = NULL;
	}
	
	CDialogEx::OnCancel();
}


void ImgViewerDlg::OnDropFiles(HDROP hDropInfo)
{
	// ��ȡ�����ļ�����
	UINT iFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (iFileCount > 1) return;

	// ��ȡ�����ļ�·��
	TCHAR tcFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, tcFilePath, _countof(tcFilePath));
	TRACE(_T("FilePath: %s\n"), tcFilePath);

	// �ж��ļ�����
	// ......
	
	// FSC_FitsX Test
	FSC_FitsX testFX;
	testFX.OpenFitsFile(tcFilePath);
	// �� FITS �ļ�����
	//m_cDibImage.LoadFromFitsFile(tcFilePath);
	//m_cDibImage.Draw(m_pCDC, CPoint(0, 0), CSize(1024, 768));

	/*
	// ����λͼ����
	m_cDibImage.LoadFromFile(tcFilePath);
	//m_cDibImage.RgbToGrade();
	m_cDibImage.Draw(m_pCDC, CPoint(0, 0), CSize(1024, 768));
	m_SubWinParam.pViewImage = NULL;

	// Test: �����Ӵ���
	m_SubWin = new SubWinDlg(&m_SubWinParam, this);
	m_SubWin->Create(IDD_SUBWIN_DIALOG, this);
	//m_SubWin->ShowWindow(SW_SHOW);
	*/
	CDialogEx::OnDropFiles(hDropInfo);
}

// ���㲢����Ԥ��ͼ�����
void ImgViewerDlg::BuildTempCDibImage(int x, int y, int width, int height)
{
	int iTempx = x - 10 - width / 2;
	int iTempy = y - 10 - height / 2;
	int iBitCount = 24;

	// ����λͼÿ�е��ֽ���
	//UINT uBmpLineByte = (width * iBitCount + 31) / 8;
	//uBmpLineByte = uBmpLineByte / 4 * 4;
	UINT uBmpLineByte = (width * iBitCount + 31) / 32 * 4;
	// ����λͼ�������ֽ���
	DWORD dwBmpDataSize = uBmpLineByte * height;

	// ����Ԥ��ͼ������������ռ�
	if (m_lpCDibDataBuffer == NULL)
		m_lpCDibDataBuffer = (LPBYTE) new BYTE[dwBmpDataSize];

	// д��Ԥ��ͼ������
	LPBYTE lpData = m_cDibImage.GetData();
	int iLineByteNum = m_cDibImage.GetLineByte();
	for (int ci = 0; ci < height; ci++)
	{
		memcpy_s(m_lpCDibDataBuffer + ci * uBmpLineByte, uBmpLineByte, lpData + (768 - (iTempy + height - ci)) * iLineByteNum + iTempx * iBitCount / 8, uBmpLineByte);
	}
	
	// ����Ԥ��ͼ��Dib����
	//m_cDibTemp.LoadFromBuffer(m_lpCDibDataBuffer, width, height, 8);
	m_cDibTemp.LoadFromBuffer(m_lpCDibDataBuffer, width, height, 24);
}

void ImgViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetDlgItem(IDC_STATIC_VIEW)->GetClientRect(&m_viewRect);
	if ((m_SubWin) &&
		(point.x >= m_viewRect.left + 10 + m_SubWinParam.iSubWidth / 2) && (point.x <= m_viewRect.left + 10 + 1024 - m_SubWinParam.iSubWidth / 2) &&
		(point.y >= m_viewRect.top + 10 + m_SubWinParam.iSubHeight / 2) && (point.y <= m_viewRect.top +10 + 768 - m_SubWinParam.iSubHeight / 2))
	{
		// ���㲢����Ԥ��ͼ�����
		BuildTempCDibImage(point.x, point.y, m_SubWinParam.iSubWidth, m_SubWinParam.iSubHeight);
		m_SubWinParam.pViewImage = &m_cDibTemp;

		CString tempStr;
		tempStr.Format(_T("MouseX: %4d MouseY: %4d"), point.x, point.y);
		SetDlgItemText(IDC_STATIC_TIPS, tempStr);
		if (m_SubWin != NULL)
		{
			m_SubWin->ShowWindow(SW_SHOW);
			m_SubWin->ShowViewImage();
		}
	}
	else
	{
		;
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
