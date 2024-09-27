
// PowerAlertUI.cpp : implementation file
//

#include "stdafx.h"
#include "PowerAlert.h"
#include "PowerAlertUI.h"
#include "afxdialogex.h"

#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audioclient.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString GetDateTime()
{
	char cDateTime[64];
	memset(cDateTime, 0, 64);
	struct tm now_time;
	time_t time_seconds = time(0);
	localtime_s(&now_time, &time_seconds);
	//printf("Local Time: %02d-%02d-%02d\n", now_time.tm_year + 1900, now_time.tm_mon + 1, now_time.tm_mday);
	sprintf_s(cDateTime, "%02d-%02d-%02d %02d:%02d:%02d", 
		now_time.tm_year + 1900, now_time.tm_mon + 1, now_time.tm_mday, now_time.tm_hour, now_time.tm_min, now_time.tm_sec);
	return cDateTime;
}


/*!
 * Append string in CEdit.
 * 
 * @param[in] edit   CEdit control
 * @param[in] txt    String to append after
 */
void CEditPrintf(CEdit& edit, const char* txt)
{
	unsigned long textLength = edit.GetWindowTextLength();
	edit.SetSel(textLength, textLength, FALSE);
	edit.ReplaceSel(CString(txt));
}


bool SetVolum(int volume)
{
	HRESULT hr;
	IMMDeviceEnumerator* pDeviceEnumerator = 0;
	IMMDevice* pDevice = 0;
	IAudioEndpointVolume* pAudioEndpointVolume = 0;
	IAudioClient* pAudioClient = 0;

	try {
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
		if (FAILED(hr)) throw "CoCreateInstance";
		hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
		if (FAILED(hr)) throw "GetDefaultAudioEndpoint";
		hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndpointVolume);
		if (FAILED(hr)) throw "pDevice->Active";
		hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
		if (FAILED(hr)) throw "pDevice->Active";

		hr = pAudioEndpointVolume->SetMute(FALSE, NULL);
		if (FAILED(hr)) throw "SetMute";

		float fVolume;
		fVolume = volume / 100.0f;
		hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume, &GUID_NULL);
		if (FAILED(hr)) throw "SetMasterVolumeLevelScalar";

		pAudioClient->Release();
		pAudioEndpointVolume->Release();
		pDevice->Release();
		pDeviceEnumerator->Release();
		return true;
	}
	catch (...) {
		if (pAudioClient) pAudioClient->Release();
		if (pAudioEndpointVolume) pAudioEndpointVolume->Release();
		if (pDevice) pDevice->Release();
		if (pDeviceEnumerator) pDeviceEnumerator->Release();
		throw;
	}
	return false;
}


// PowerAlertUI dialog


PowerAlertUI::PowerAlertUI(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_POWERALERT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nDeviceID = -1;
	m_MonitorStatus = 0;
	m_AlertStatus = 0;
	m_Interval = 300;
	m_Volume = 20;
}

void PowerAlertUI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT_INTERVAL, m_EditInterval);
	DDX_Control(pDX, IDC_EDIT_VOLUME, m_EditVolume);
}

BEGIN_MESSAGE_MAP(PowerAlertUI, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &PowerAlertUI::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &PowerAlertUI::OnBnClickedButton2)
END_MESSAGE_MAP()


// PowerAlertUI message handlers

BOOL PowerAlertUI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here
	InitUI();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void PowerAlertUI::OnPaint()
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
HCURSOR PowerAlertUI::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void PowerAlertUI::InitUI()
{
	CString csInterval, csVolume;
	csInterval.Format(_T("%d"), m_Interval);
	csVolume.Format(_T("%d"), m_Volume);
	m_EditInterval.SetWindowText(csInterval);
	m_EditVolume.SetWindowText(csVolume);
	CEditPrintf(m_Edit1, GetDateTime() + _T("   Software start.\r\n"));
}

void PowerAlertUI::Monitor()
{
	//CEditPrintf(m_Edit1, GetDateTime() + _T("   OnTimer.\r\n"));

	if (m_AlertStatus == 1)
		return;

	if (m_MonitorStatus == 0)
	{
		CEditPrintf(m_Edit1, GetDateTime() + _T("   Monitor in Error State. Code-001.\r\n"));
		return;
	}

	SYSTEM_POWER_STATUS sps;
	if (0 == GetSystemPowerStatus(&sps))
		CEditPrintf(m_Edit1, GetDateTime() + _T("   Power in Error State. Code-002.\r\n"));
	TRACE(_T("ACLineStatus  %d, BatteryFlag  %d\n"), sps.ACLineStatus, sps.BatteryFlag);
	if (sps.ACLineStatus == 1)
		return;

	// Power Alert Start	
	if (sps.ACLineStatus == 0)
	{
		m_AlertStatus = 1;
		CEditPrintf(m_Edit1, GetDateTime() + _T("   >>>>>  Power Failure  <<<<<\r\n"));
	}

	// MCI Open
	if (m_nDeviceID == -1)
	{
		MCI_OPEN_PARMS mciOP;
		mciOP.lpstrDeviceType = nullptr;
		mciOP.lpstrElementName = _T("sound1.mp3");

		if (0 == mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE, (DWORD_PTR)(static_cast<LPVOID>(&mciOP))))
		{
			m_nDeviceID = mciOP.wDeviceID;
		}
		else
		{
			m_nDeviceID = -1;
			CEditPrintf(m_Edit1, GetDateTime() + _T("   Alert in Error State. Code-003.\r\n"));
		}
	}
	// MCI Stop
	MCI_SEEK_PARMS mciSP{};
	if (0 == mciSendCommand(m_nDeviceID, MCI_SEEK, MCI_WAIT | MCI_NOTIFY | MCI_SEEK_TO_START, (DWORD_PTR)(static_cast<LPVOID>(&mciSP))))
	{

	}
	else
	{

	}
	// MCI Play
	MCI_PLAY_PARMS mciPP{};
	if (0 == mciSendCommand(m_nDeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)(static_cast<LPVOID>(&mciPP))))
	{

	}
	else
	{
		
	}
	// Volume
	CoInitialize(0);
	SetVolum(m_Volume);
	CoUninitialize();
}


void PowerAlertUI::OnBnClickedButton1()
{
	if (m_MonitorStatus == 1)
		return;

	CString csInterval, csVolume;
	m_EditInterval.GetWindowText(csInterval);
	m_EditVolume.GetWindowText(csVolume);
	m_Interval = atoi(csInterval);
	m_Volume = atoi(csVolume);
	if (m_Interval <= 0) m_Interval = 300;
	if (m_Volume < 0 || m_Volume > 100) m_Volume = 20;
	csInterval.Format(_T("%d"), m_Interval);
	csVolume.Format(_T("%d"), m_Volume);
	m_EditInterval.SetWindowText(csInterval);
	m_EditVolume.SetWindowText(csVolume);

	if (0 == SetTimer(1, m_Interval * 1000, NULL))
	{
		CEditPrintf(m_Edit1, GetDateTime() + _T("   Failed to start Power Monitor.\r\n"));
	}
	else
	{
		CEditPrintf(m_Edit1, GetDateTime() + _T("   Power Monitor start.\r\n"));
		m_MonitorStatus = 1;
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	};
	return;
}


void PowerAlertUI::OnBnClickedButton2()
{
	// TODO:
	KillTimer(1);
	m_MonitorStatus = 0;
	// Stop Alert
	MCI_GENERIC_PARMS mciGP{};
	if (0 == mciSendCommand(m_nDeviceID, MCI_CLOSE, MCI_NOTIFY | MCI_WAIT, (DWORD_PTR)(static_cast<LPVOID>(&mciGP))))
	{
		m_nDeviceID = -1;
	}
	else
	{

	}
	// Volume
	CoInitialize(0);
	SetVolum(0);
	CoUninitialize();

	m_AlertStatus = 0;
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	CEditPrintf(m_Edit1, GetDateTime() + _T("   Power Monitor stop.\r\n"));
}


void PowerAlertUI::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:
	switch (nIDEvent)
	{
	case 1:
		Monitor();
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
