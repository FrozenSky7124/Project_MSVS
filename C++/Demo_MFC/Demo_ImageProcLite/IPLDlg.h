
// IPLDlg.h : ͷ�ļ�
//

#pragma once

#include "Dib_Ex.h"
#include "FSC_FitsX.h"
#include "afxwin.h"

// IPLDlg �Ի���
class IPLDlg : public CDialogEx
{
// ����
public:
	IPLDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_IMAGEPROCLITE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnMenu_File_Open();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

public:

	bool OpenFile_FITS(LPCTSTR lpszPath);
	void ListFitsHDU();
	void ListImgInfo();
	void ComputeGrayLimit(double dLowPer, double dHighPer);

private:
	CDC* m_pCDCImgMain;
	FSC_FitsX m_FSCFitsX;
	FSC_DibX m_FSCDibX;
	int m_iMinPixelCount;
	int m_iMaxPixelCount;
	int m_iLowPixelCount;
	int m_iHighPixelCount;

	CFont m_FontStandard;
	CEdit m_EditFitsInfo;
	CEdit m_EditImgInfo;
public:
};
