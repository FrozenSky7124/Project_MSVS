// MainDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo02_ReadSqlite.h"
#include "MainDialog.h"
#include "afxdialogex.h"
#include <mmsystem.h>

//#define dbPath "D:\\Project_MSVS\\C++\\Demo_Hack\\Demo02_ReadSqlite\\Debug\\dbQA.db"
#define dbPath "C:\\dbQA.db"

char* m_char;
wchar_t* m_wchar;

int CStringHexToInt(CString str);

//��̬�ͷ��ڴ�
void Release()
{
	if (m_char)
	{
		delete m_char;
		m_char = NULL;
	}
	if (m_wchar)
	{
		delete m_wchar;
		m_wchar = NULL;
	}
}

//wchar_t TO char
char* WcharToChar(const wchar_t* wp)
{
	Release();
	int len = WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

//char TO wchar_t
wchar_t* CharToWchar(const char* c)
{
	Release();
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

int CStringHexToInt(CString str)
{
	int nRet = 0;
	int count = 1;
	for (int i = str.GetLength() - 1; i >= 0; --i)
	{
		int nNum = 0;
		char chTest;
		chTest = str.GetAt(i);
		if (chTest >= '0' && chTest <= '9')
		{
			nNum = chTest - '0';
		}
		else if (chTest >= '?' && chTest <= 'F')
		{
			nNum = chTest - 'A' + 10;
		}
		else if (chTest >= 'a' && chTest <= 'f')
		{
			nNum = chTest - 'a' + 10;
		}
		nRet += nNum*count;
		count *= 16;

	}
	return nRet;
}

WCHAR * Utf8ToUnicode(char* unicode)
{

	int len = MultiByteToWideChar(CP_UTF8, 0, unicode, -1, NULL, 0);
	WCHAR  *szUtf8 = new wchar_t[len + 1];
	memset(szUtf8, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, unicode, -1, szUtf8, len);
	szUtf8[len] = '\0';
	len = len - 1;
	return szUtf8;
}

char* UnicodeToUtf8(WCHAR*  wszUtf8)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
	char* szUtf8 = new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

//�ظ��𰸹���
bool isRepeat(CString *filter, int *num, CString key)
{
	for (int i = 0; i < *num; i++)
	{
		if (filter[i] == key)
		{
			return true;
		}
	}
	filter[*num] = key;
	*num = *num + 1;
	return false;
}

// MainDialog �Ի���

IMPLEMENT_DYNAMIC(MainDialog, CDialogEx)

MainDialog::MainDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MainDlg, pParent)
{
	
}

MainDialog::~MainDialog()
{
	sqlite3_close(db);
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MainDialog::OnBnClickedStart)
	ON_WM_ACTIVATE()
	ON_EN_CHANGE(IDC_SearchEdit, &MainDialog::OnEnChangeSearchEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// MainDialog �ص�����
/*
void CALLBACK SearchMemory(
	HWND hWnd,      // handle of CWnd that called SetTimer
	UINT nMsg,      // WM_TIMER
	UINT_PTR nIDEvent,   // timer identification
	DWORD dwTime    // system time
	)
*/
void MainDialog::OnTimer(UINT_PTR nIDEvent)
{
	//��ʼ���ݼ��
	//TRACE(_T("FrozenDebug: Enter OnTimer."));
	//this->confirmAddr = false;
	CString cAddrStr;
	if (this->firstCall)
	{
		this->firstCall = false;
		LPWSTR lpAddrStr = new wchar_t[10];
		memset(lpAddrStr, 0, sizeof(lpAddrStr));
		GetDlgItemText(IDC_AddrEdit, lpAddrStr, 9);
		TRACE(_T("FrozenDebug: lpAddrStr = %s"), lpAddrStr);
		cAddrStr = lpAddrStr;
		delete[] lpAddrStr;
		if (cAddrStr.IsEmpty() == true)
		{
			//��Ҫ�Զ�Ѱ��
			TRACE(_T("FrozenDebug: AddrStr = NULL"));
			this->confirmAddr = false;
		}
		else
		{
			//����Ҫ�Զ�Ѱ��
			TRACE(_T("FrozenDebug: AddrStr = %s"), cAddrStr);
			this->confirmAddr = true;
			this->idAddr = CStringHexToInt(cAddrStr);
		}
	}

	if (confirmAddr)
	{
		//�Զ��������ݿ�

		/*
		static CString preStr = _T("");
		CString curStr;
		WCHAR* tempChar = Utf8ToUnicode((char*)idAddr);
		curStr.Format(_T("%s"), tempChar);
		delete[] tempChar;

		if (preStr != curStr)
		{
			preStr = curStr;
			TRACE(_T("FrozenDebug: ��⵽��Ŀ����"));
			char *dbErrMsg = 0;
			char sqlQuery[128];
			memset(sqlQuery, 0, sizeof(sqlQuery));
			char* tempChar = UnicodeToUtf8((WCHAR*)(curStr.GetString()));
			sprintf_s(sqlQuery, "SELECT * FROM qatable WHERE question = '%s';", tempChar);
			delete[] tempChar;
			int nRow = 0, nColumn = 0; //��ѯ�����������������
			char **dbResult; //��ά�����Ž��
			sqlite3_get_table(db, sqlQuery, &dbResult, &nRow, &nColumn, &dbErrMsg);
			if (nRow == 0)
			{
				//û���ҵ�
			}
			else
			{
				//�ҵ�
				CString resultCStr;
				CString *filter;
				int filterNum = 0;
				filter = new CString[nRow + 1];
				resultCStr = _T("");
				for (int i = 6; i < (nRow + 1) * nColumn; i = i + 4)
				{
					CString temp2;
					WCHAR* tempwchar2 = Utf8ToUnicode(dbResult[i + 1]);
					temp2.Format(_T("�� %s ��"), tempwchar2);
					delete[] tempwchar2;
					if (isRepeat(filter, &filterNum, temp2))
					{
						continue;
					}
					resultCStr += temp2;
				}
				TRACE(_T("FrozenDebug: db Row=%d Column=%d"), nRow, nColumn);
				SetDlgItemText(IDC_ResultEdit, resultCStr);
				//MessageBeep(0x00000030L);
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
				delete[] filter;
			}
		}
		else
		{
			//��Ŀû��
		}
		*/
		
		int curID = *((int *)idAddr);

		if (preID != curID)
		{
			preID = curID;
			TRACE(_T("FrozenDebug: ��⵽��ĿID����Ϊ��%d"), curID);
			char *dbErrMsg = 0;
			char sqlQuery[128];
			memset(sqlQuery, 0, sizeof(sqlQuery));
			sprintf_s(sqlQuery, "SELECT * FROM qatable WHERE id = '%d';", curID);
			int nRow = 0, nColumn = 0; //��ѯ�����������������
			char **dbResult; //��ά�����Ž��
			sqlite3_get_table(db, sqlQuery, &dbResult, &nRow, &nColumn, &dbErrMsg);
			if (nRow == 0)
			{
				//û���ҵ�
				TRACE(_T("FrozenDebug: �޷�����ID:%d �ҵ���Ӧ��Ŀ"), curID);
			}
			else
			{
				//�ҵ�
				CString resultCStr;
				CString *filter;
				int filterNum = 0;
				filter = new CString[nRow + 1];
				resultCStr = _T("");
				for (int i = 7; i < (nRow + 1) * nColumn; i = i + 5)
				{
					CString temp2;
					WCHAR* tempwchar2 = Utf8ToUnicode(dbResult[i + 2]);//i+2 ȡ������ֶ�
					temp2.Format(_T("�� %s ��"), tempwchar2);
					delete[] tempwchar2;
					if (isRepeat(filter, &filterNum, temp2))
					{
						continue;
					}
					resultCStr += temp2;
				}
				TRACE(_T("FrozenDebug: db Row=%d Column=%d"), nRow, nColumn);
				(GetDlgItem(IDC_ResultEdit))->SetFont(&m_editFontH);
				SetDlgItemText(IDC_ResultEdit, resultCStr);
				//MessageBeep(0x00000030L);
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
				delete[] filter;
			}
		}
		else
		{
			//��Ŀû��
			//TRACE(_T("FrozenDebug: δ��⵽��Ŀ����"));
		}
	}
	else
	{
		//�Զ�������Ŀ��ַ
		//�ҵ���ַ��ʹconfirmAddr=true;
	}
	CDialogEx::OnTimer(nIDEvent);
}


// MainDialog ��Ϣ�������
void MainDialog::OnBnClickedStart()
{
	if (this->timerStatus == true)
	{
		this->timerStatus = false;
		KillTimer(7);
		SetDlgItemText(IDC_BUTTON1, _T("Start"));
	}
	else
	{
		this->timerStatus = true;
		SetDlgItemText(IDC_BUTTON1, _T("Stop"));
		this->firstCall = true;
		//������ʱ��
		//��ʱ�����Ϊ7�����1000ms���ص�SearchMemory
		//UINT_PTR m_nCallbackTimer = SetTimer(7, 500, &SearchMemory);
		UINT_PTR m_nCallbackTimer = SetTimer(7, 500, NULL);
		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT);
	}
}

void MainDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	//�������ɼ����¼�������¼�����ʱ
	if (nState == 1 || nState == 2)
	{
		SetDlgItemText(IDC_SearchEdit, _T(""));
		GetDlgItem(IDC_SearchEdit)->SetFocus();
	}

#ifdef _DEBUG
	CString debugStr;
	debugStr.Format(_T("FrozenDebug: OnActivate nState = %d bMini = %d"), nState, bMinimized);
	TRACE(debugStr);
#endif
}

bool MainDialog::OpenDB()
{
	int dbRet = sqlite3_open(dbPath, &db);
	TRACE(_T("OpenDB dbRet = %d"), dbRet);
	if (dbRet)
	{
		sqlite3_close(db);
		db = NULL;
		return false;
	}
	else
	{
		return true;
	}
}

void MainDialog::OnEnChangeSearchEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	(GetDlgItem(IDC_ResultEdit))->SetFont(&m_editFont);
	// ȡ��ѯ�ַ���
	CString SearchCStr;
	LPWSTR lpSearchStr = new wchar_t[15];
	memset(lpSearchStr, 0, sizeof(lpSearchStr));
	GetDlgItemText(IDC_SearchEdit, lpSearchStr, 14);
	SearchCStr = lpSearchStr;
	delete[] lpSearchStr;

	if (SearchCStr.GetLength() < 3)
	{
		return;
	}

	// ��ʼ����
	//(GetDlgItem(IDC_ResultEdit))->SetFont(&m_editFont);
	char *dbErrMsg = 0;
	char sqlQuery[128];
	memset(sqlQuery, 0, sizeof(sqlQuery));
	sprintf_s(sqlQuery, "SELECT * FROM qatable WHERE pyquestion LIKE '%%%s%%';", WcharToChar(SearchCStr.GetString()));
	int nRow = 0, nColumn = 0; //��ѯ�����������������
	char **dbResult; //��ά�����Ž��
	TRACE(_T("FrozenDebug: before get table"));
	sqlite3_get_table(db, sqlQuery, &dbResult, &nRow, &nColumn, &dbErrMsg);
	CString resultCStr;
	CString *filter;
	int filterNum = 0;
	filter = new CString[nRow + 1];
	resultCStr = _T("");
	for (int i = 7; i < (nRow + 1) * nColumn; i = i + 5)
	{
		CString tempID;
		WCHAR* tempIDchar = Utf8ToUnicode(dbResult[i - 2]); //ȡ��ID�ַ���
		tempID.Format(_T("%s"), tempIDchar);
		delete[] tempIDchar;
		if (_ttoi(tempID) > 12000 && _ttoi(tempID) < 18000)
		{
			SetDlgItemText(IDC_CurID, tempID);
		}
		else
		{
			SetDlgItemText(IDC_CurID, _T("NULL"));
		}
		CString temp1;
		WCHAR* tempwchar1 = Utf8ToUnicode(dbResult[i]); //ȡ�������ַ���
		temp1.Format(_T("%s  "), tempwchar1);
		delete[] tempwchar1;
		CString temp2;
		WCHAR* tempwchar2 = Utf8ToUnicode(dbResult[i + 1]); //ȡ�����ַ���
		temp2.Format(_T("�� %s ��\r\n"), tempwchar2);
		delete[] tempwchar2;
		if (isRepeat(filter, &filterNum, temp2))
		{
			continue;
		}
		resultCStr += temp1;
		resultCStr += temp2;
	}
	SetDlgItemText(IDC_ResultEdit, resultCStr);
	TRACE(_T("FrozenDebug: before delete filter"));
	delete[] filter;
	TRACE(_T("FrozenDebug: before delete dbresult db Row=%d Column=%d"), nRow, nColumn);
	//delete dbResult;
}


BOOL MainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������
	m_editFont.CreatePointFont(120, _T("΢���ź�"));
	m_editFontH.CreatePointFont(240, _T("΢���ź�"));
	(GetDlgItem(IDC_ResultEdit))->SetFont(&m_editFont);
	(GetDlgItem(IDC_SearchEdit))->SetFont(&m_editFont);
	(GetDlgItem(IDC_AddrEdit))->SetFont(&m_editFont);
	(GetDlgItem(IDC_CurID))->SetFont(&m_editFont);

	if (!OpenDB())
	{
		MessageBox(_T("�޷��������ݿ⣡"), _T("����"), MB_OK);
	}
	//firstCall = true;
	this->firstCall = true;
	this->confirmAddr = false;
	this->idAddr = 0;
	this->preID = 0;
	this->timerStatus = false;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}