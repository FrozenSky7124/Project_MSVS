
// SerialToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Demo23_SerialTool.h"
#include "SerialToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UINT WINAPI uiFuncRead(LPVOID lpParam)
{
	SerialToolDlg* pThis = (SerialToolDlg*)lpParam;
	FSC_Serial* pSerial = pThis->m_pSerial;

	while (true)
	{
		byte byteBuff[129];
		unsigned long NbBytesRead = 0;
		pSerial->Read(byteBuff, 128, &NbBytesRead);
		if (NbBytesRead > 0)
		{
			byteBuff[NbBytesRead] = '\0';
			TRACE("recv %2d %s\n", NbBytesRead, byteBuff);
			unsigned long textLength = pThis->m_Info.GetWindowTextLength();
			pThis->m_Info.SetSel(textLength, textLength, FALSE);
			pThis->m_Info.ReplaceSel("\r\n" + CString(byteBuff));
		}
		if (byteBuff[0] == 0xFF)
			break;
	}

	return 0;
}


// SerialToolDlg dialog


SerialToolDlg::SerialToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO23_SERIALTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SerialToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, m_Info);
}

BEGIN_MESSAGE_MAP(SerialToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_Open, &SerialToolDlg::OnClicked_BtnOpen)
END_MESSAGE_MAP()


// SerialToolDlg message handlers

BOOL SerialToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void SerialToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void SerialToolDlg::OnCancel()
{
	int iRst = m_pSerial->Close();
	TRACE("Serial.Close iRst = %d.\n", iRst);
	if (m_pSerial != NULL)
	{
		delete m_pSerial;
		m_pSerial = NULL;
	}
	CDialogEx::OnCancel();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR SerialToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void SerialToolDlg::OnClicked_BtnOpen()
{
	m_pSerial = new FSC_Serial();
	int iRst;
	iRst = m_pSerial->Open("COM4", false);
	TRACE("Serial.Open iRst = %d.\n", iRst);
	iRst = m_pSerial->SetSerialPort(CBR_115200, NOPARITY, 8, ONESTOPBIT);
	TRACE("Serial.SetSerialPort iRst = %d.\n", iRst);
	iRst = m_pSerial->SetTimeout(10, 0, 10);
	TRACE("Serial.SetTimeout iRst = %d.\n", iRst);

	UINT uiThreadRead;
	m_hThreadRead = (HANDLE)_beginthreadex(NULL, 0, &uiFuncRead, this, 0, &uiThreadRead);
}

