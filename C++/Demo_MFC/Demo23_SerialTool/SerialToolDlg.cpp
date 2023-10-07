
// SerialToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Demo23_SerialTool.h"
#include "SerialToolDlg.h"
#include "afxdialogex.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// SerialToolDlg dialog



SerialToolDlg::SerialToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO23_SERIALTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void SerialToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR SerialToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void SerialToolDlg::OnClicked_BtnOpen()
{
	HANDLE hSerial;
	hSerial = CreateFile(L"\\\\.\\COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			TRACE("Serial port does not exist.\n");
		}
		TRACE("Error opening serial port.\n");
	}
	else
	{
		TRACE("Serial port open.\n");
		// Configure the serial port settings...
	}
}
