#pragma once

#include <atlstr.H>
#include <strsafe.h>
#include <assert.h>
#include <atltime.h>


SERVICE_STATUS_HANDLE	g_hServiceStatus;
SERVICE_STATUS			g_ServiceStatus;

BOOL					g_bInstall = false;
DWORD					g_dwThreadID = 0;
HANDLE					g_hThread = NULL;

#define	ID_TIMER1 1
#define ID_TIMER2 2

#define DAT_EXT			_T(".dat")


enum DOWNFILE_STATE
{
	DOWNFILE_FAILE,
	DOWNFILE_SUCCESS,
	DOWNFILE_NOEXIT,
	DOWNFILE_CANCEL,
};

typedef struct CDownFileInfo_TYP
{
	SYSTEMTIME		m_st;					// ϵͳ��ǰʱ��
	char			m_szFileName[MAX_PATH]; //�����ļ���
	char			m_szIp[16];				// �ͻ���IP
	float			m_fDownSpeed;			//�����ٶ�
	DOWNFILE_STATE	m_enuDownFileState;		// �Ƿ�ɹ�����
} CDownFileInfo;

typedef struct DOWNFILECOUNT_TYP
{
	//char m_szFileName[MAX_PATH];
	SYSTEMTIME	m_st;
	int			m_iFrequency;
	float		m_fDownSpeed;
	int			m_type;
}DownFileCount;

//����ȫ�ֺ���

void InitData();
BOOL IsInstalled();
BOOL InstallService();
BOOL UninstallService();
void WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv);
void WINAPI ServiceStrl(DWORD dwOpcode);

void LogEvent(LPCTSTR pszFormat, ...);


// ��־������غ�������
BOOL Log(LPCTSTR lpLogFilePath,LPCTSTR lpszFormat, ...);
void OnSetTimer();
void CALLBACK TimerProc( HWND hwnd,	UINT message,UINT idTimer,DWORD dwTime);
DWORD WINAPI ThreadProc(void *p);

bool AutoToDB();
bool LogFileIntoSqlDatabase(LPCSTR szLogFile);
bool GetTimeFromStr(int* nYear ,int* nMonth,int* nDay,LPCSTR szTime);
bool FindGameIDAndFileName(CString & strSrc, CString &strFileName, int & iGameId);
bool LogFileIntoSqlDatabase(LPCSTR szLogFile,CString strTableName);


