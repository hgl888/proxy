// IGALogFileService.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "IGALogFileService.h"
#include "../include/zlib.h"

#include "EventLock.h"

//#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
//	no_namespace rename("EOF", "EndOfFile")

//#define ID_TIMER1 1
#define ID_TIMER2 2

#define  ZLIB
#include "GarbageCollector.h"
#include "CSQLConnection.h"
#include "IGA_Structs.h"
#include <io.h>
#include <Wininet.h>


//��־������ر�������
CSQLConnection	    g_SQLOperate;
CGarbageCollector   g_GarbageCollector;
//CHAR				*m_pFile;
GameInfos			g_Games;
PlayInfos			g_PlayInfos;
HardwareInfos	    g_HardwareInfos;	
UpdateInfos			g_UpdateInfos;
FocusInfos			g_FocusInfos;
CString				g_strPlayInfoPath;
CString				g_strUpdatePath;
CString				g_strHardwareInfoPath;
CString				g_strFocusInfoPath;
CString				g_strLastPlayInfoPath;
CString				g_strLastUpdatePath;
CString				g_strLastHardwareInfoPath;
CString				g_strLastFocusInfoPath;
CString				g_strVDPath;
CString				g_strDateBase;
CString				g_strPassword;
CString				g_strServer;
CString				g_strUser;
CString             g_strTimer;
CString				g_strLastDate;
CString				g_strServiceINIPath;
TCHAR				g_szServiveLogFile[_MAX_PATH];
int					g_iLastPlayInfoNum;
int					g_iLastUpdateInfoNum;
int					g_iLastHardwareInfoNum;
int					g_iLastFocusInfoNum;

// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	//::CoInitialize(NULL);
	try
	{

		OutputDebugString("_tmain()");
		InitData();
		SERVICE_TABLE_ENTRY service_table[] =
		{
			{ g_szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
			{ NULL, NULL }
		};
		if (argc >= 2) 
		{
			if (stricmp(argv[1], "/install") == 0)
			{
				OutputDebugString("��װLog����ɹ�!"); 
				InstallService();
			}
			else if (stricmp(argv[1], "/uninstall") == 0)
			{
				UninstallService();
			}
		}

		if (!::StartServiceCtrlDispatcher(service_table))
		{
			OutputDebugString("Register Service Main Function Error!");
			//LogEvent(_T("Register Service Main Function Error!"));
		}
		OutputDebugString("_tmain2");
		//::CoUninitialize();
	}
	catch(...)
	{
		OutputDebugString("_tmain2 Error");
		return -1;
	}
	return 0;
}

//*********************************************************
//Functiopn:			Init
//Description:			��ʼ��
//Calls:				main
//Called By:				
//Table Accessed:				
//Table Updated:				
//Input:				
//Output:				
//Return:				
//Others:				
//History:				
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void InitData()
{
	OutputDebugString("Init()");

	g_hServiceStatus = NULL;
	g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;


	TCHAR szFileName[_MAX_PATH];
	::GetModuleFileName(NULL, szFileName, _MAX_PATH);
	TCHAR	*p	= ::_tcsrchr(szFileName, _T('\\'));
	if(!p)
		p	= ::_tcsrchr(szFileName, _T('/'));
	*++p	= 0;

	/*
	��־�ļ�
	*/ 
	_snprintf(g_szServiveLogFile,_MAX_PATH,"%s\\IGALogService.log",szFileName);

	/*
	�����ļ�
	*/
	g_strServiceINIPath.Format("%s\\IGA.INI",szFileName);

	/*
	��ʱ��ʱ��
	*/
	TCHAR szData[_MAX_PATH]={0};
	GetPrivateProfileString("Common","Time","10",szData,sizeof(szData),g_strServiceINIPath);
	g_strTimer= szData;

	g_dwThreadID=0;
	g_hThread=NULL;

}

//*********************************************************
//Functiopn:			ServiceMain
//Description:			��������������������п��ƶԷ�����Ƶ�ע��
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void WINAPI ServiceMain(DWORD dwArgc,LPTSTR *lpszArgv)
{
	//::CoInitialize(NULL);

	OutputDebugString("ServiceMain()");

	// Register the control request handler
	g_hServiceStatus = RegisterServiceCtrlHandler(g_szServiceName, ServiceStrl);
	if (g_hServiceStatus == NULL)
	{
		LogEvent(_T("Handler not installed"));
		return;
	}

	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	SetServiceStatus(g_hServiceStatus, &g_ServiceStatus);

	//�����߳�ִ����־��������
	g_hThread	= ::CreateThread(NULL, 0, ThreadProc, NULL, 0, &g_dwThreadID);

	g_ServiceStatus.dwWin32ExitCode = S_OK;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(g_hServiceStatus, &g_ServiceStatus);

	/*
	����ʧ��
	*/
	if(g_hThread == NULL)
	{
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(g_hServiceStatus, &g_ServiceStatus);
		//LogEvent(_T("Service stopped"));
	}
	OutputDebugString("ServiceMain2");
}

//*********************************************************
//Functiopn:			ServiceStrl
//Description:			�������������������ʵ�ֶԷ���Ŀ��ƣ�
//						���ڷ����������ֹͣ����������ʱ���������д˴�����
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:				dwOpcode�����Ʒ����״̬
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void WINAPI ServiceStrl(DWORD dwOpcode)
{
	OutputDebugString("ServiceStrl)");
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		/*if(m_dwThreadID!=0)
            PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);*/
		if(g_hThread)
		{
			TerminateThread(g_hThread,0);
			g_hThread=NULL;
			g_dwThreadID=0;
		}

		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(g_hServiceStatus, &g_ServiceStatus);
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		OutputDebugString(_T("Bad service request"));
		break;
	}
}
//*********************************************************
//Functiopn:			IsInstalled
//Description:			�жϷ����Ƿ��Ѿ�����װ
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
BOOL IsInstalled()
{
	BOOL bResult = FALSE;

	//�򿪷�����ƹ�����
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//�򿪷���
		SC_HANDLE hService = ::OpenService(hSCM, g_szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

//*********************************************************
//Functiopn:			InstallService
//Description:			��װ������
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
BOOL InstallService()
{
	OutputDebugString("Install()");
	if (IsInstalled())
		return TRUE;

	//�򿪷�����ƹ�����
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), g_szServiceName, MB_OK);
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//��������
	SC_HANDLE hService = ::CreateService(
		hSCM, g_szServiceName, g_szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T(""), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), g_szServiceName, MB_OK);
		return FALSE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

//*********************************************************
//Functiopn:			UninstallService
//Description:			ɾ��������
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
BOOL UninstallService()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), g_szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, g_szServiceName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), g_szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//ɾ������
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	LogEvent(_T("Service could not be deleted"));
	return FALSE;
}

//*********************************************************
//Functiopn:			LogEvent
//Description:			��¼�����¼�
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void LogEvent(LPCTSTR pFormat, ...)
{
	//OutputDebugString(pFormat);

	return;

	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, g_szServiceName);
	if (hEventSource != NULL) 
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}

DWORD WINAPI ThreadProc(void *p)
{
	::CoInitialize(NULL);
	try
	{
		g_SQLOperate.Init();
		OnSetTimer();
		OutputDebugString("������ɶ�ʱ��");
		MSG   msg;  
		BOOL bRet;
		while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
		{ 
			if (bRet == -1)
			{
				// handle the error and possibly exit
				CTime	time = CTime::GetCurrentTime();
				Log(g_szServiveLogFile,"%s\t%s ",time.Format("%Y-%m-%d %H:%M:%S"),"GetMessageʧ��");
				break;
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}

	}
	catch(...)
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s ",time.Format("%Y-%m-%d %H:%M:%S"),"ThreadProcδ֪�쳣");
	}
	::CoUninitialize();
	return 0;
}

void OnSetTimer() 
{
	OutputDebugString("OnSetTimer()");
	int timer = atoi(g_strTimer);
	SetTimer(NULL,ID_TIMER2, 1000*timer, (TIMERPROC) TimerProc);
	CTime	time = CTime::GetCurrentTime();
	Log(g_szServiveLogFile,"%s\t%s %d����",time.Format("%Y-%m-%d %H:%M:%S"),"��ʼ������ʱ��",timer);
}

void CALLBACK TimerProc( 
						HWND hwnd,       
						UINT message,     
						UINT idTimer,     
						DWORD dwTime)   
{
	OutputDebugString("TimerProc()");

	CTime timeStart = CTime::GetCurrentTime();

	/*
	ȥ����ʱ��,��֪��Ϊʲô��ʧ��
	*/
	/*
	if(!KillTimer(NULL,ID_TIMER2))
	{
	Log(m_szLogFilePath,"%s\t ֹͣ��ʱ��ʧ��",
	timeStart.Format("%Y-%m-%d %H:%M:%S"));

	}
	*/

	AutoToDB();

	/*
	������ʱ��
	*/		
	//OnSetTimer();

	CTime timeEnd = CTime::GetCurrentTime();
	CTimeSpan need = timeEnd - timeStart;

	Log(g_szServiveLogFile,"%s\t��ʱ: %s",
		timeEnd.Format("%Y-%m-%d %H:%M:%S"),
		need.Format("%H:%M:%S\r\n"));

}

//  ��־�������ȫ�ֺ���ʵ��

BOOL LoadAllGameInfo()
{
	OutputDebugString("LoadAllGameInfo()");
	CTime	time = CTime::GetCurrentTime();
	Log(g_szServiveLogFile,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"���ڶ�ȡGameInfo��Ϣ...");
	g_SQLOperate.ExecuteSP("H_GetAllGame");
	(*(g_SQLOperate.m_pRecordset))->MoveFirst();
	while (!(*(g_SQLOperate.m_pRecordset))->EndOfFile)
	{
		GameInfo	*pGameInfo	= new GameInfo();
		if (pGameInfo == NULL)
		{
			return FALSE;
		}
		g_GarbageCollector.AddPtr(pGameInfo);
		ZeroMemory(pGameInfo, sizeof(GameInfo));
		g_SQLOperate.GetFieldValue("col0", pGameInfo->dwGameID);
		g_SQLOperate.GetFieldValue("col1", (char*)(pGameInfo->szGameVersion), sizeof(pGameInfo->szGameVersion));
		g_Games.Add(pGameInfo);
		(*(g_SQLOperate.m_pRecordset))->MoveNext();
	}
	return TRUE;
}

BOOL FindFile()
{
	OutputDebugString("FindFile()");
	CString strFilePath = _T("");
	strFilePath.Format("%s\\IGABackupOver.tmp",g_strVDPath);
	if(_access(strFilePath,0)!=-1)
	{
		KillTimer(NULL,ID_TIMER1);
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"��־�ļ��������!");

		AutoToDB();
		DeleteFile(strFilePath);
		time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"ɾ��tem�ļ��ɹ�!");
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


CString   dwIP2csIP(DWORD   ip)   
{   
	CString   csIP;   
	BYTE   field[4];   
	for(int   i=0;i<4;i++)   
	{   
		field[i]=(BYTE)(0x000000FF   &   ip>>i*8);   
	}   
	csIP.Format(TEXT("%d.%d.%d.%d"),field[0],field[1],field[2],field[3]);   
	return   csIP;   
}

BOOL GetTimeFromIni(int* nYear ,int* nMonth,int* nDay,CString strTime)
{
	strTime.TrimLeft();
	strTime.TrimRight();

	//20081126
	if(strTime.GetLength()!=8)
		return FALSE;

	CString szTmp;

	szTmp = strTime.Mid(0,4);
	*nYear = atoi((char*)(LPCSTR)szTmp);

	szTmp = strTime.Mid(4,2);
	*nMonth = atoi((char*)(LPCSTR)szTmp);

	szTmp = strTime.Mid(6,2);
	*nDay = atoi((char*)(LPCSTR)szTmp);

	return TRUE;
}

void SetPathsByTime(CString strINIpath,CTime	time)
{
	TCHAR szData[_MAX_PATH]={0};
	CString strTmp;

	CString   strTimeDay = time.Format("%Y%m%d");

	GetPrivateProfileString("SDK","PATH",		"",szData,sizeof(szData),strINIpath);
	strTmp = szData;
	if(strTmp.Right(1)!="\\")
		strTmp+="\\";
	g_strUpdatePath.Format("%sSDK_%s.LOG",strTmp,strTimeDay);

	GetPrivateProfileString("HardwareInfo","path","",szData,sizeof(szData),strINIpath);
	strTmp = szData;
	if(strTmp.Right(1)!="\\")
		strTmp+="\\";
	g_strHardwareInfoPath.Format("%sHardwareInfo_%s.LOG",strTmp,strTimeDay);

	GetPrivateProfileString("PlayInfo","path","",szData,sizeof(szData),strINIpath);
	strTmp = szData;
	if(strTmp.Right(1)!="\\")
		strTmp+="\\";
	g_strPlayInfoPath.Format("%s%s.playinfo",strTmp,strTimeDay);

	GetPrivateProfileString("FocusInfo","path","",szData,sizeof(szData),strINIpath);
	strTmp = szData;
	if(strTmp.Right(1)!="\\")
		strTmp+="\\";
	g_strFocusInfoPath.Format("%s%s.FocusInfo",strTmp,strTimeDay);
}

void UploadDataFromFile(CTime timeDay)
{
	/*
	m_nLastUpdateInfoNum ��ָȥ�غ���Ѿ��ɹ���������,������־���Ѿ�����ļ�¼��
	*/
	OnSdkInfo(g_strUpdatePath,g_iLastUpdateInfoNum,TRUE);

	/*
	m_nLastHardwareInfoNum ��ָȥ�غ���Ѿ��ɹ���������,������־���Ѿ�����ļ�¼��
	*/
	OnHarewareInfo(g_strHardwareInfoPath,g_iLastHardwareInfoNum,TRUE);

	/*
	m_nLastPlayInfoNum ���Ѿ�����ļ�¼��
	*/
	OnPlayInfor(g_strPlayInfoPath,g_iLastPlayInfoNum,TRUE);

	/*
	m_nLastFocusInfoNum ���Ѿ�����ļ�¼��,��ȡ����Ⱥϲ��ۼ���ͬ��
	*/
	OnFocusInfo(g_strFocusInfoPath,g_iLastFocusInfoNum,timeDay,TRUE);

}

void TreateDataByDay(CString strINIpath,CTime	theDay)
{
	try{

	/*
	�������ݿ�
	*/
	if(!g_SQLOperate.Connect(g_strDateBase,g_strServer,g_strPassword,g_strUser))
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s ",time.Format("%Y-%m-%d %H:%M:%S"),"���ݿ�����ʧ��");
		return ;
	}

	/*
	������һ�����־�ļ�·��
	*/
	SetPathsByTime(strINIpath,theDay);

	/*
	�ϴ���һ���
	*/
	UploadDataFromFile(theDay);

	/*
	���ݿ�ɾ�����������
	*/
	if(g_SQLOperate.ExecuteSP("H_DeleteNoInGame")!=1)
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"ExecuteSP:H_DeleteNoInGame ʧ��");
	}

	/*
	������һ����հ�װ�� 
	*/
	CString strDate = theDay.Format("%Y-%m-%d");

	if(!g_SQLOperate.CalculateInstallCount(strDate))
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s %s ",time.Format("%Y-%m-%d %H:%M:%S"),strDate,"CalculateInstallCount ʧ��");
	}

	/*
	�ر����ݿ�
	*/
	g_SQLOperate.Close();

	/*�ͷ��ڴ�*/
	if(!g_GarbageCollector.RemoveAll())
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s ",time.Format("%Y-%m-%d %H:%M:%S"),"m_GarbageCollector RemoveAll ʧ��");
	}

	}catch(...)
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"TreateDataByDay�쳣");
	}
}
/*  �Ƚ�LastDate,���������ʱ�䲻�ȣ��͵����ϴε���־�͵�ǰʱ�����־���������ż������뵱ǰ��־   */

CEventLock eventLock("EVENT_NAME");

BOOL AutoToDB()
{
	OutputDebugString("AutoToDB()");
	try
	{
		eventLock.Lock();

		/*
		��װ·��
		*/
		TCHAR szFileName[_MAX_PATH];
		::GetModuleFileName(NULL, szFileName, _MAX_PATH);
		TCHAR	*p	= ::_tcsrchr(szFileName, _T('\\'));
		if(!p)
			return FALSE;
		*p	= 0;

		/*��־·��*/
		_snprintf(g_szServiveLogFile,_MAX_PATH,"%s\\IGALogService.log",szFileName);

		/*IGA.INI�����ļ�·��*/
		g_strServiceINIPath.Format("%s\\IGA.INI",szFileName);
		if(_access((char*)(LPCSTR)g_strServiceINIPath,0)==-1)
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),g_strServiceINIPath,"�ļ�������");
			return FALSE;
		}

		/*
		��������������
		*/
		TCHAR szData[_MAX_PATH]={0};
		
		/*
		��ý��쵱ǰʱ��
		*/
		CTime	  timeToday			 = CTime::GetCurrentTime();
		CString   strCurrentTime = timeToday.Format("%Y%m%d");

		GetPrivateProfileString("Common","VirtualDirectory","",szData,sizeof(szData),g_strServiceINIPath);
		g_strVDPath = szData;

		if(g_strVDPath.IsEmpty())
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t%s ",time.Format("%Y-%m-%d %H:%M:%S"),"VirtualDirectoryû������");
			return FALSE;
		}
		if(g_strVDPath.Right(1)!="\\")
			g_strVDPath+="\\";

		/*���ݿ��������*/
		GetPrivateProfileString("Sql","ServerName",	"",szData,sizeof(szData),g_strServiceINIPath);
		g_strDateBase= szData;
		GetPrivateProfileString("Sql","ServerIP",	"",szData,sizeof(szData),g_strServiceINIPath);
		g_strServer= szData;
		GetPrivateProfileString("Sql","User",		"",szData,sizeof(szData),g_strServiceINIPath);
		g_strUser= szData;
		GetPrivateProfileString("Sql","Password",	"",szData,sizeof(szData),g_strServiceINIPath);
		g_strPassword= szData;

		GetPrivateProfileString("Common","LastDate",strCurrentTime,szData,sizeof(szData),g_strServiceINIPath);
		g_strLastDate= szData;
		GetPrivateProfileString("Common","Time","10",szData,sizeof(szData),g_strServiceINIPath);
		g_strTimer= szData;


		GetPrivateProfileString("Common","PlayInfoNum","0",szData,sizeof(szData),g_strServiceINIPath);
		g_iLastPlayInfoNum= atoi(szData);
		GetPrivateProfileString("Common","SDKNum","0",szData,sizeof(szData),g_strServiceINIPath);
		g_iLastUpdateInfoNum= atoi(szData);
		GetPrivateProfileString("Common","HardwareInfoNum","0",szData,sizeof(szData),g_strServiceINIPath);
		g_iLastHardwareInfoNum= atoi(szData);
		GetPrivateProfileString("Common","FocusInfoNum","0",szData,sizeof(szData),g_strServiceINIPath);
		g_iLastFocusInfoNum= atoi(szData);

		/*
		���EMMOA_IIS.INI�Ƿ����
		*/
		CString strINIpath;
		strINIpath.Format("%sEMMOA_IIS.INI",g_strVDPath);
		if(_access((char*)(LPCSTR)strINIpath,0)==-1)
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),strINIpath,"�ļ�������");
			return FALSE;
		}

		/*
		����ϴε�������
		*/
		int nYear,nMonth,nDay;
		if(!GetTimeFromIni(&nYear,&nMonth,&nDay,g_strLastDate))
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),g_strLastDate,"LastDate ���ô���");
			return FALSE;
		}

		/*
		���б�Ҫ��ѭ������֮ǰ��
		*/
		CTime LastTime(nYear,nMonth,nDay,0,0,0);
		CTimeSpan tsDay( 1, 0, 0, 0 );
		for(; (timeToday - LastTime).GetDays()>0 ; LastTime+=tsDay)
		{

			CTime	tmpTime = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t%s %s",
				tmpTime.Format("%Y-%m-%d %H:%M:%S"),"<><><>�ϴ�<><><>",	LastTime.Format("%Y-%m-%d"));

			/*���ô���ʱ����*/	
			WritePrivateProfileString("Common","LastDate",(LPCTSTR)LastTime.Format("%Y%m%d"),g_strServiceINIPath);

			/*
			������һ���
			*/
			TreateDataByDay(strINIpath,LastTime);

			/*
			��� ����
			*/
			WritePrivateProfileString("Common","SDKNum",			"0",g_strServiceINIPath);
			WritePrivateProfileString("Common","HardwareInfoNum",	"0",g_strServiceINIPath);
			WritePrivateProfileString("Common","FocusInfoNum",		"0",g_strServiceINIPath);
			WritePrivateProfileString("Common","PlayInfoNum",		"0",g_strServiceINIPath);

			/*
			���� ����
			*/
			g_iLastUpdateInfoNum = 0;
			g_iLastHardwareInfoNum=0;
			g_iLastPlayInfoNum=0;
			g_iLastFocusInfoNum=0;

			Log(g_szServiveLogFile,"%s","\r\n");
		}

		/*���ô���ʱ����*/	
		WritePrivateProfileString("Common","LastDate",(LPCTSTR)strCurrentTime,g_strServiceINIPath);

		/*
		����ǰ��
		*/
		TreateDataByDay(strINIpath,timeToday);

		eventLock.UnLock();
		return TRUE;
	}
	catch (_com_error)
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s ",time.Format("%Y-%m-%d %H:%M:%S"),"COM ����");
		return FALSE;
	}
	catch (...)
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s ",time.Format("%Y-%m-%d %H:%M:%S"),"���� ����");
		return FALSE;
	}

}

CString GetGameVersion(DWORD dwGameID)
{
	for (int index=0;index<g_Games.GetSize();index++)
	{
		GameInfo  *pGameInfo = g_Games[index];
		if (pGameInfo&&pGameInfo->dwGameID==dwGameID)
		{
			return CString(pGameInfo->szGameVersion);
		}

	}
	return CString("0.0.0.0");

}	



BOOL GetSdkInfoFormLog(CString strFilePath)
{
	PROTECT(GetSdkInfoFormLog(CString strFilePath))
	OutputDebugString("GetSdkInfoFormLog(CString strFilePath)");

	g_UpdateInfos.RemoveAll();

	if(_access(strFilePath,0)==-1)
	{
		//CTime	time = CTime::GetCurrentTime();
		//Log(m_szLogFilePath,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),strFilePath,"�޷��ҵ�SDK��־�ļ�");
		return FALSE;
	}

	CStdioFile sf;
	if (!sf.Open(strFilePath, CFile::modeRead|CFile::typeText|CFile::shareDenyNone))
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),strFilePath,"�޷����ļ�");
		return FALSE;
	}

	while(TRUE)
	{
		/*
		���ļ���ȡһ������
		*/
		CHAR	szUpdateInfo[512]	= { 0 };
		char* Ret =sf.ReadString(szUpdateInfo, sizeof(szUpdateInfo));
		if( !Ret)
		{
			break;//��ȡ����
		}

		/*
		�����ڴ�
		*/
		CHAR	*q	= szUpdateInfo;
		CHAR	*p	= strchr(q, '|');
		UpdateInfo	*pUpdateInfo	= new UpdateInfo();
		if (pUpdateInfo == NULL)
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"new ʧ��");
			break;
		}
		ZeroMemory(pUpdateInfo, sizeof(UpdateInfo));

		/*
		����һ��
		*/

		CHAR	*pszItem[]	=
		{
				pUpdateInfo->szUpDateTime,
				pUpdateInfo->szGameName,
				pUpdateInfo->szGameVersion,
				pUpdateInfo->szSDKName,
				pUpdateInfo->szAddress,
				pUpdateInfo->szMacID,
				NULL,
				NULL,
		};
		DWORD	*dwItem[]	=
		{
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				&pUpdateInfo->dwType,
				&pUpdateInfo->dwChnID,
		};

		INT	nIndex	= 0 ;
		TCHAR	szKey[_MAX_PATH]	= { 0 };
		while( p != NULL )
		{
			memcpy(szKey, q, p - q);
			szKey[p - q]	= 0;
			CHAR* c=szKey;
			while (*c)
			{
				while (*c && isspace(*c)) 
					c++;
				if (!isspace(*c))
					break;
			}
			if(nIndex ==6 || nIndex == 7)
			{
				*dwItem[nIndex]	= atol(c);
			}
			else
			{
				lstrcpyA(pszItem[nIndex], c);
			}
			q	= p + 1;
			p	= strchr(q, '|');
			nIndex++;
		}

		/*
		�жϸ����ڱ��ļ����Ƿ����µ�
		*/

		BOOL bNew=TRUE;
		for (int i=0;i<g_UpdateInfos.GetSize();i++)
		{
			UpdateInfo	*pTmpInfo = g_UpdateInfos[i];

			if(strcmp(pTmpInfo->szMacID,pUpdateInfo->szMacID)!=0)
				continue;
			if(strcmp(pTmpInfo->szGameName,pUpdateInfo->szGameName)!=0)
				continue;
            
			if(strcmp(pTmpInfo->szAddress,pUpdateInfo->szAddress)!=0)
				continue;
			if(strcmp(pTmpInfo->szSDKName,pUpdateInfo->szSDKName)!=0)
				continue;

			/*
			�Ѿ�������ͬ��
			*/
			bNew  = FALSE;
			break;
		}

		//�µ�����뵽�б���
		if(bNew)
		{
			g_GarbageCollector.AddPtr(pUpdateInfo);
			g_UpdateInfos.Add(pUpdateInfo);
		}
		else
		{
			delete pUpdateInfo;
		}

		//��ʼ��һ��
	}
	sf.Close();
	UNPROTECT
	return TRUE;
}

BOOL GetHardwareInfoFromLog(CString strFilePath)
{
	PROTECT(GetHardwareInfoFromLog(CString strFilePath))
	OutputDebugString("GetHardwareInfoFromLog(CString strFilePath)");

	g_HardwareInfos.RemoveAll();

	if(_access(strFilePath,0)==-1)
	{
		//CTime	time = CTime::GetCurrentTime();
		//Log(m_szLogFilePath,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),strFilePath,"�޷��ҵ�HardwareInfo��־�ļ�");
		return FALSE;
	}

	CStdioFile sf;
	if (!sf.Open(strFilePath, CFile::modeRead|CFile::typeText|CFile::shareDenyNone))
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),strFilePath,"�޷����ļ�");
		return FALSE;
	}

	while(TRUE)
	{ 
		/*
		���ļ���ȡһ������
		*/
		CHAR	szHardwareInfo[512]	= { 0 };
		char* Ret			= NULL;
		Ret  =sf.ReadString(szHardwareInfo,sizeof(szHardwareInfo));
		if( !Ret )
		{
			break;//������
		}

		/*
		�����ڴ�
		*/
		CHAR	*q	= szHardwareInfo;
		CHAR	*p	= strchr(q, '|');
		HardwareInfo	*pHardwareInfo	= new HardwareInfo();
		if (pHardwareInfo == NULL)
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"new ʧ��");
			break;
		}
		ZeroMemory(pHardwareInfo, sizeof(HardwareInfo));

		/*
		����һ��
		*/

		CHAR	*pszItem[]	=
		{
				pHardwareInfo->szUpdataTime,
				NULL,
				pHardwareInfo->szMacID,
				NULL,
				pHardwareInfo->szOperationSystem,
				pHardwareInfo->szAddress,
				pHardwareInfo->szCPU,
				pHardwareInfo->szDisplay,
				pHardwareInfo->szType,
				NULL,
				pHardwareInfo->szLocalAddress,
		};
		DWORD	*dwItem[]	=
		{
				NULL,
				&pHardwareInfo->dwGameID,
				NULL,
				&pHardwareInfo->dwMemorySize,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				&pHardwareInfo->dwChnID,
				NULL
		};

		INT	nIndex	= 0 ;
		TCHAR	szKey[_MAX_PATH]	= { 0 };
		while( p != NULL )
		{
			memcpy(szKey, q, p - q);
			szKey[p - q]	= 0;
			CHAR* c=szKey;
			while (*c)
			{
				while (*c && isspace(*c)) 
					c++;
				if (!isspace(*c))
					break;
			}
			if(nIndex ==3 || nIndex == 9||nIndex ==1)
			{
				*dwItem[nIndex]	= atol(c);
			}
			else
			{
				lstrcpyA(pszItem[nIndex], c);
			}
			q	= p + 1;
			p	= strchr(q, '|');
			nIndex++;

			/*if(nIndex>=11)
				break;*/
		}
		/*
		�жϸ����ڱ��ļ����Ƿ����µ�
		*/

		BOOL bNew=TRUE;
		for (int i=0;i<g_HardwareInfos.GetSize();i++)
		{
			HardwareInfo	*pTmpInfo = g_HardwareInfos[i];

			if(strcmp(pTmpInfo->szMacID,pHardwareInfo->szMacID)!=0)
				continue;
			if(pTmpInfo->dwGameID!=pHardwareInfo->dwGameID)
				continue;

			if(pTmpInfo->dwMemorySize!=pHardwareInfo->dwMemorySize)
				continue;

			/*
			��sql��䱣��һ��,ע�͵�����Ĵ���
			*/
			//if(strcmp(pTmpInfo->szCPU,pHardwareInfo->szCPU)!=0)
			//	continue;

			/*
			�Ѿ�������ͬ��
			*/
			bNew  = FALSE;
			break;
		}

		//�µ�����뵽�б���
		if(bNew)
		{
			g_GarbageCollector.AddPtr(pHardwareInfo);
			g_HardwareInfos.Add(pHardwareInfo);
		}
		else
		{
			delete pHardwareInfo;
		}

	}
	sf.Close();
	UNPROTECT
	return TRUE;
}

BOOL GetFocusInfoFormLog(CString strFilePath,int  LastUpdateInfoNum,int* lpRecordTreatedCount)
{
	int nIndex=0;
	*lpRecordTreatedCount =0;
	PROTECT(GetFocusInfoFormLog(CString strFilePath,int  LastUpdateInfoNum,int* lpRecordTreatedCount))
	OutputDebugString("GetFocusInfoFormLog");

	if(_access(g_strFocusInfoPath,0)==-1)
	{
		//CTime	time = CTime::GetCurrentTime();
		//Log(m_szLogFilePath,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),strFilePath,"�޷��ҵ�ָ��FocusInfo��־�ļ�");
		return FALSE;
	}

	HANDLE hFile ;
	hFile = ::CreateFile(g_strFocusInfoPath, 
		GENERIC_READ, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, 
		NULL,
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),strFilePath,"�޷����ļ�");
		return FALSE;

	}


	g_FocusInfos.RemoveAll();

	CTime	  time			 = CTime::GetCurrentTime();
	CString   strCurrentTime = time.Format("%Y%m%d");
	//CString   strDate = time.Format("%Y-%m-%d");

	int len = 0;
	tagPacketHeader packetHeader;

	typedef	FOCUSREQUEST_T<tagRequestHeader>	FOCUSREQUEST;
	FOCUSREQUEST * pRequestHeader = NULL;

	while(TRUE)
	{
		memset(&packetHeader, 0, sizeof(tagPacketHeader));
		len = sizeof(tagPacketHeader);

		/*
		����ͷ
		*/ 
		DWORD dwRet			= 0;
		BOOL	bRet		= ::ReadFile(hFile,&packetHeader, len, &dwRet, NULL);
		if (!bRet || dwRet != len)
		{
			//CTime	time = CTime::GetCurrentTime();
			//Log(m_szLogFilePath,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"��ȡ ʧ��");
			break;
		}

		len = packetHeader.dwLength - len;
		if (len < sizeof(FOCUSREQUEST)) 
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"�������ݴ���");
			break;
		}

		char * buffer = new char [len];
		if(!buffer)
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"New ʧ��");
			break;

		}
		memset(buffer, 0, len);

		/*
		������
		*/ 
		bRet		= ::ReadFile(hFile,buffer, len, &dwRet, NULL);
		if (!bRet || dwRet != len)
		{
			if (buffer != NULL)
			{
				delete []buffer;
				buffer = NULL;
			}

			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"��ȡ ʧ��");
			break;
		}

		/*
		�����Ѿ�����İ��������н���
		*/
		nIndex++;
		if(nIndex<=LastUpdateInfoNum)
		{
			if (buffer != NULL)
			{
				delete []buffer;
				buffer = NULL;
			}
			continue;
		}

		/*
		���н���
		*/

		pRequestHeader = (FOCUSREQUEST *)buffer;
		
		/*
		��ù�����
		*/
		DWORD dwAdCount = pRequestHeader->dwAdCount;

		if (dwAdCount == 0)
		{
			if (buffer != NULL)
			{
				delete []buffer;
				buffer = NULL;
			}
			continue;//���������Ϊ�㣬����������һ����

		}
	
		DWORD dwTargetSize = dwAdCount * sizeof(tagFocus)*100;
		char * pp = new char [dwTargetSize];
		if(!pp)
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"New ʧ��");

			if (buffer != NULL)
			{
				delete []buffer;
				buffer = NULL;
			}
			break;
		}
		memset(pp, 0, dwTargetSize);

		DWORD dwSourceSize = len - sizeof(FOCUSREQUEST);

		if (packetHeader.byFlags == 1) // ��ʾfocus���ݾ�����ѹ������
		{
			// �����ݽ�ѹ��pp
			int ret = uncompress((Bytef*)pp, (uLongf*)&dwTargetSize, (Bytef*)(buffer + sizeof(FOCUSREQUEST)), (uLongf)dwSourceSize);
			if(ret!=0)
			{
				CTime	time = CTime::GetCurrentTime();
				Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"uncompress ������");
			}

			/*
			������ݴ�С�Ƿ���ȷ
			*/
			if (dwTargetSize != dwAdCount * sizeof(tagFocus)) 
			{
				if (pp != NULL)
				{
					delete []pp;
					pp = NULL;
				}

				if (buffer != NULL)
				{
					delete []buffer;
					buffer = NULL;
				}

				continue;
			}
		}
		else
		{
			//û��ѹ����ֱ�Ӹ���
			memcpy(pp, buffer + sizeof(FOCUSREQUEST), dwSourceSize);
		}

		CString strTemp = _T("");

		LPFOCUS pFocus =(LPFOCUS)pp;
		for(int i=0; i<(int)dwAdCount; i++,pFocus++)
		{
			//pFocus = (LPFOCUS)(pp+i*sizeof(FOCUS));

			FocusInfo* pFocusInfo = new FocusInfo;
			if (pFocusInfo ==NULL)
			{
				CTime	time = CTime::GetCurrentTime();
				Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"New ʧ��");
				break;
			}

			pFocusInfo->dwGameID = pRequestHeader->dwGameID;
			pFocusInfo->dwAreaID = pRequestHeader->dwAreaID;
			pFocusInfo->dwAddress = pRequestHeader->dwAddress;
			_stprintf(pFocusInfo->szMacID,"%s",pRequestHeader->szMacID);
			pFocusInfo->dwAdID = pFocus->dwAdID;
			pFocusInfo->dwLoadCount = pFocus->dwLoadCount;
			pFocusInfo->dwClickCount = pFocus->dwClickCount;
			
			pFocusInfo->pi[0].dwPlayCount = pFocus->pi[0].dwPlayCount;
			pFocusInfo->pi[0].dwPlayTime = pFocus->pi[0].dwPlayTime;
			pFocusInfo->pi[0].dwFullPlayCount = pFocus->pi[0].dwFullPlayCount;
			
			pFocusInfo->pi[1].dwPlayCount = pFocus->pi[1].dwPlayCount;
			pFocusInfo->pi[1].dwPlayTime = pFocus->pi[1].dwPlayTime;
			pFocusInfo->pi[1].dwFullPlayCount = pFocus->pi[1].dwFullPlayCount;
			
			pFocusInfo->pi[2].dwPlayCount = pFocus->pi[2].dwPlayCount;
			pFocusInfo->pi[2].dwPlayTime = pFocus->pi[2].dwPlayTime;
			pFocusInfo->pi[2].dwFullPlayCount = pFocus->pi[2].dwFullPlayCount;

			pFocusInfo->pi[3].dwPlayCount = pFocus->pi[3].dwPlayCount;
			pFocusInfo->pi[3].dwPlayTime = pFocus->pi[3].dwPlayTime;
			pFocusInfo->pi[3].dwFullPlayCount = pFocus->pi[3].dwFullPlayCount;

			BOOL bNew=TRUE;
			for(int j=0; j< g_FocusInfos.GetSize();j++ )
			{
				FocusInfo *pTmp = g_FocusInfos[j];

				if(pTmp->dwGameID!=pFocusInfo->dwGameID)
					continue;
				if(pTmp->dwAdID!=pFocusInfo->dwAdID)
					continue;

				if(strcmp(pTmp->szMacID,pFocusInfo->szMacID)!=0)
					continue;

				//�����б����иù��λ����������
				pTmp->dwLoadCount +=pFocusInfo->dwLoadCount;
				pFocusInfo->dwClickCount += pFocusInfo->dwClickCount;

				pTmp->pi[0].dwPlayCount += pFocusInfo->pi[0].dwPlayCount;
				pTmp->pi[0].dwPlayTime += pFocusInfo->pi[0].dwPlayTime;
				pTmp->pi[0].dwFullPlayCount += pFocusInfo->pi[0].dwFullPlayCount;

				pTmp->pi[1].dwPlayCount += pFocusInfo->pi[1].dwPlayCount;
				pTmp->pi[1].dwPlayTime += pFocusInfo->pi[1].dwPlayTime;
				pTmp->pi[1].dwFullPlayCount += pFocusInfo->pi[1].dwFullPlayCount;

				pTmp->pi[2].dwPlayCount += pFocusInfo->pi[2].dwPlayCount;
				pTmp->pi[2].dwPlayTime += pFocusInfo->pi[2].dwPlayTime;
				pTmp->pi[2].dwFullPlayCount += pFocusInfo->pi[2].dwFullPlayCount;

				pTmp->pi[3].dwPlayCount += pFocusInfo->pi[3].dwPlayCount;
				pTmp->pi[3].dwPlayTime += pFocusInfo->pi[3].dwPlayTime;
				pTmp->pi[3].dwFullPlayCount += pFocusInfo->pi[3].dwFullPlayCount;

				bNew = FALSE;
				break;

			}

			/*
			�µ��ն˵Ĺ��λ�����
			*/
			if(bNew)
			{
				g_GarbageCollector.AddPtr(pFocusInfo);
				g_FocusInfos.Add(pFocusInfo);

			}
			else
			{
				delete pFocusInfo;
			}

		}

		if (pp != NULL)
		{
			delete []pp;
			pp = NULL;
		}

		if (buffer != NULL)
		{
			delete []buffer;
			buffer = NULL;
		}

	}

	CloseHandle(hFile);
	UNPROTECT
	/*
	���ñ��ν����ļ�¼����
	*/
	*lpRecordTreatedCount = nIndex - LastUpdateInfoNum;
	return TRUE;
}

BOOL GetPlayInfoFormLog(CString strFilePath)
{
	PROTECT(GetPlayInfoFormLog(CString strFilePath))
	OutputDebugString("GetPlayInfoFormLog(CString strFilePath)");

	if(_access(strFilePath,0)==-1)
	{
		//CTime	time = CTime::GetCurrentTime();
		//Log(m_szLogFilePath,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),strFilePath,"�޷��ҵ�ָ��PlayInfo��־�ļ�");
		return FALSE;
	}

	HANDLE hFile=::CreateFile(strFilePath,
							GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s %s",time.Format("%Y-%m-%d %H:%M:%S"),strFilePath,"�޷����ļ�");
		return FALSE;
	}

	DWORD dwFileSize	= ::GetFileSize(hFile, NULL);
	DWORD dwRet			= 0;

	CHAR  *p			= new CHAR[dwFileSize+1];
	if(!p)
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"New ʧ��");
		CloseHandle(hFile);
		return FALSE;
	}


	BOOL	bRet		= ::ReadFile(hFile,p, dwFileSize, &dwRet, NULL);
	if( !bRet || dwRet != dwFileSize )
	{
		if(p)
            delete []p;

		CloseHandle(hFile);
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t %s %d %s",time.Format("%Y-%m-%d %H:%M:%S"),__FILE__,__LINE__ ,"��ȡ ʧ��");
		return FALSE;
	}

	g_GarbageCollector.AddTempBuffer(p);
	//m_pFile				= p;

	g_PlayInfos.RemoveAll();

	PlayInfo *playInfo	= (PlayInfo *)p;
	for (int index = 0; index<(int)(dwFileSize/sizeof(PlayInfo)); index++,playInfo++)
	{
		g_PlayInfos.Add(playInfo);
	}
	CloseHandle(hFile);
	UNPROTECT
	return TRUE;
}

void OnSdkInfo(CString strUpdatePath,int LastUpdateInfoNum, BOOL bWrite,BOOL bCheck) 
{
	PROTECT(OnSdkInfo(CString strUpdatePath,int LastUpdateInfoNum, BOOL bWrite,BOOL bCheck))
	OutputDebugString("OnSdkInfo()  ");

	/*
	����־�ļ���ȡ����ȥ�ص�����
	*/
	if (!GetSdkInfoFormLog(strUpdatePath))
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"SDK ������");
		return;
	}

	CTime	time = CTime::GetCurrentTime();
	//Log(m_szLogFilePath,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"���ڵ���SDK��־...");

	CString			strCurrentTime;

	DWORD dwYear	= time.GetYear(); 
	DWORD dwMon		= time.GetMonth();      
	DWORD dwDay		= time.GetDay();
	DWORD dwHour	= time.GetHour();
	DWORD dwMin		= time.GetMinute();
	DWORD dwSecond	= time.GetSecond();
	strCurrentTime.Format("%d-%d-%d %d:%d:%d",dwYear,dwMon,dwDay,dwHour,dwMin,dwSecond);

	//ͳ�����ɹ��ĸ���
	int nOK=0;

	for (int index=LastUpdateInfoNum;index<g_UpdateInfos.GetSize();index++)
	{
		UpdateInfo	*pUpdateInfo = g_UpdateInfos[index];

		if(!pUpdateInfo)
		{
			Log(g_szServiveLogFile,"%s\t %s",time.Format("%Y-%m-%d %H:%M:%S"),"pUpdateInfoΪ��");
			continue;
		}

		/*
		���ʱ�����"ʱ����"��Ϣ
		*/
		CString strUpdateTime = pUpdateInfo->szUpDateTime;
		strUpdateTime = strUpdateTime.Left(strUpdateTime.Find(' '));

		/*
		��װSQL���
		*/
		CString strSQL = _T("");
		if(bCheck)
		{
			strSQL.Format(" IF not EXISTS \
							(SELECT id FROM dbo.w_SDKQUERY  WHERE UpDateTime = '%s' and GameName = '%s' and SDKName='%s'and IPAddress='%s' and MACAddress='%s')\
						begin\
						insert into dbo.w_SDKQUERY (UpDateTime,GameName,GameVersion,SDKName,IPAddress,MACAddress,Type,ChnID,AddDate)\
						values('%s','%s','%s','%s','%s','%s','%d','%d','%s')\
							end\
							",
							strUpdateTime,
							pUpdateInfo->szGameName,
							pUpdateInfo->szSDKName,
							pUpdateInfo->szAddress,
							pUpdateInfo->szMacID,

							strUpdateTime,
							pUpdateInfo->szGameName,
							"1.0.0.1",//!!!!!!!!!!!!!!!!!һֱû���õ�
							pUpdateInfo->szSDKName,
							pUpdateInfo->szAddress,
							pUpdateInfo->szMacID,
							pUpdateInfo->dwType,
							pUpdateInfo->dwChnID,
							strCurrentTime);
		}
		else
		{
			strSQL.Format(" insert into dbo.w_SDKQUERY (UpDateTime,GameName,GameVersion,SDKName,IPAddress,MACAddress,Type,ChnID,AddDate)\
							values('%s','%s','%s','%s','%s','%s','%d','%d','%s')",

								strUpdateTime,
								pUpdateInfo->szGameName,
								"1.0.0.1",//!!!!!!!!!!!!!!!!!һֱû���õ�
								pUpdateInfo->szSDKName,
								pUpdateInfo->szAddress,
								pUpdateInfo->szMacID,
								pUpdateInfo->dwType,
								pUpdateInfo->dwChnID,
								strCurrentTime);

			//Log(m_szLogFilePath,"%s\t %s",time.Format("%Y-%m-%d %H:%M:%S"),strSQL);

		}

		if(g_SQLOperate.ExecuteSQL(strSQL)==1)
		{
			nOK++;
		}
		else
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d",time.Format("%Y-%m-%d %H:%M:%S"),"SDK ����ʧ��",index);

			/*
			���ʧ�ܣ�ֹͣ�������ݵ����
			*/
			break;
		}

	}

	if (bWrite)
	{
		CString strTemp;
		strTemp.Format("%d",nOK+LastUpdateInfoNum);
		WritePrivateProfileString("Common","SDKNum",(LPCTSTR)strTemp,g_strServiceINIPath);
	}

	if(nOK>0)
	{
		time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s %d��",time.Format("%Y-%m-%d %H:%M:%S"),"SDK����ɹ�",nOK);
	}

	UNPROTECT

}

void OnHarewareInfo(CString strHardwareInfoPath,int LastHardwareInfoNum ,BOOL bWrite,BOOL bCheck) 
{
	PROTECT(OnHarewareInfo(CString strHardwareInfoPath,int LastHardwareInfoNum ,BOOL bWrite) )
	OutputDebugString("OnHarewareInfo ");

	/*
	����־�ļ���ȡ����ȥ�ص�����
	*/
	if (!GetHardwareInfoFromLog(strHardwareInfoPath))
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"Hardware ������");
		return;
	}


	CTime	time = CTime::GetCurrentTime();
	//Log(m_szLogFilePath,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"���ڵ���HardwareInfo��־...");

	CString			strCurrentTime;

	DWORD dwYear	= time.GetYear(); 
	DWORD dwMon		= time.GetMonth();      
	DWORD dwDay		= time.GetDay();
	DWORD dwHour	= time.GetHour();
	DWORD dwMin		= time.GetMinute();
	DWORD dwSecond	= time.GetSecond();
	strCurrentTime.Format("%d-%d-%d %d:%d:%d",dwYear,dwMon,dwDay,dwHour,dwMin,dwSecond);

	//ͳ�����ɹ��ĸ���
	int nOK=0;

	int index = 0;
	for ( index=LastHardwareInfoNum;index<g_HardwareInfos.GetSize();index++)
	{
		HardwareInfo	*pHardwareInfo = g_HardwareInfos[index];
		if(!pHardwareInfo)
			continue;

		CString strSQL;

		strSQL.Format(" IF not EXISTS (SELECT id FROM dbo.w_HardwareInfo  WHERE  MACAddress='%s' and MemorySize ='%d'and GameID = '%d')\
						  begin\
						  insert into dbo.w_HardwareInfo (UpdateTime,MACAddress,MemorySize,OperateSystem,IPAddress,CPU,DisplayCard,Type,ChnID,AddDate,GameID,LANIPAddress)\
						  values('%s','%s','%d','%s','%s','%s','%s','%s','%d','%s',%d,'%s')\
						  end\
						  ",
						  pHardwareInfo->szMacID,
						  pHardwareInfo->dwMemorySize,/*����һ�ָ���ָ��*/
						  pHardwareInfo->dwGameID,

						  pHardwareInfo->szUpdataTime,
						  pHardwareInfo->szMacID,
						  pHardwareInfo->dwMemorySize,
						  pHardwareInfo->szOperationSystem,
						  pHardwareInfo->szAddress,
						  pHardwareInfo->szCPU,
						  pHardwareInfo->szDisplay,
						  pHardwareInfo->szType,
						  pHardwareInfo->dwChnID,
						  strCurrentTime,
						  pHardwareInfo->dwGameID,
						  pHardwareInfo->szLocalAddress);

		/*
		����ط��ܺķ���Դ
		*/
		if(g_SQLOperate.ExecuteSQL(strSQL)==1)
		{
			nOK++;
		}
		else
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d",time.Format("%Y-%m-%d %H:%M:%S"),"HardwareInfo ����ʧ��",index);

			/*
			���ʧ�ܣ�ֹͣ�������ݵ����
			*/
			break;
		}
	}

	if (bWrite)
	{
		CString strTemp;
		strTemp.Format("%d",nOK+LastHardwareInfoNum);
		WritePrivateProfileString("Common","HardwareInfoNum",(LPCTSTR)strTemp,g_strServiceINIPath);
	}

	if(nOK>0)
	{
		time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s %d��",time.Format("%Y-%m-%d %H:%M:%S"),"HardwaerInfo�ύ���",nOK);
	}


	/*
	����ip��Ĵ���
	*/
	time = CTime::GetCurrentTime();
	//Log(m_szLogFilePath,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"���ڵ���IndependenceIP��־...");

	nOK=0;
	for (index =LastHardwareInfoNum;index<g_HardwareInfos.GetSize();index++)
	{
		HardwareInfo	*pHardwareInfo = g_HardwareInfos[index];
		CString strSQL = _T("");
		CString strUpdateTime = pHardwareInfo->szUpdataTime;
		strUpdateTime = strUpdateTime.Left(strUpdateTime.Find(' '));
		strSQL.Format(" IF not EXISTS (SELECT id FROM dbo.w_IndependenceIP where   UpdateTime='%s' and IPAddress ='%s'and GameID = '%d')\
						begin\
						insert into dbo.w_IndependenceIP (UpdateTime,GameID,IPAddress,AddDate)\
						values('%s','%d','%s','%s')\
						end\
						",
						strUpdateTime,
						pHardwareInfo->szAddress,
						pHardwareInfo->dwGameID,

						strUpdateTime,
						pHardwareInfo->dwGameID,
						pHardwareInfo->szAddress,
						strCurrentTime);
		//Log(m_szLogFilePath,"%s\r\n",strSQL);

		/*
		����ط��ܺķ���Դ
		*/

		if(g_SQLOperate.ExecuteSQL(strSQL)==1)
		{
			nOK++;
		}
		else
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d",time.Format("%Y-%m-%d %H:%M:%S"),"IndependenceIP ����ʧ��",index);

			/*
			���ʧ�ܣ�ֹͣ�������ݵ����
			*/
			break;
		}
	}
	
	if(nOK>0)
	{
		time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s %d��",time.Format("%Y-%m-%d %H:%M:%S"),"IndependenceIP�ύ���", nOK);
	}

	UNPROTECT
}

void OnFocusInfo(CString strFocusInfoPath,int LastFocusInfoNum,CTime timeDay,BOOL bWrite) 
{
	PROTECT(OnFocusInfo(CString strFocusInfoPath,int LastFocusInfoNum,CTime timeDay,BOOL bWrite) )
	OutputDebugString("OnFocusInfo");

	int nTreatedCount=0;

	/*
	���ļ���ȡ����,��ȡ������ݱ��ϲ�
	*/
	if (!GetFocusInfoFormLog(strFocusInfoPath,LastFocusInfoNum,&nTreatedCount))
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"Focus ������");
		return;

	}

	CTime	time;// = CTime::GetCurrentTime();
	//Log(m_szLogFilePath,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"���ڵ���FocusInfo��־...");

	CString   strCurrentTime = timeDay.Format("%Y-%m-%d");


	int nOK=0;
	for (int index= 0;index<g_FocusInfos.GetSize();index++)
	{
		FocusInfo *pFocusInfo = g_FocusInfos[index];

		CString strSQL=_T("");

		TCHAR szArea[9] = {0};
		sprintf(szArea,"%08x",pFocusInfo->dwAreaID);

		strSQL.Format("if not exists (select id from dbo.w_ADFluxInfo where MACAddress='%s' and GameID ='%d' and AdPosID ='%d' and SendDate = '%s')\
						begin insert into dbo.w_ADFluxInfo \
						(SendDate,GameID,GameEdition,PlayerArea,AdPosID,AdResLoadTimes,AdResClickTimes,\
						SuperClearViewTime,SuperClearViewLength,SuperClearFullViewTime,\
						MuchClearViewTime,MuchClearViewLength,MuchClearFullViewTime,\
						ClearViewTime,ClearViewLength,ClearFullViewTime,\
						BlurViewTime,BlurViewLength,BlurFullViewTime,\
						MACAddress,IPAddress)\
						values('%s','%d','%s','%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%s')\
						end\
						else\
						begin\
						update dbo.w_ADFluxInfo\
						set AdResLoadTimes =AdResLoadTimes+'%d',AdResClickTimes=AdResClickTimes+'%d',\
						SuperClearViewTime=SuperClearViewTime+'%d',SuperClearViewLength=SuperClearViewLength+'%d',SuperClearFullViewTime=SuperClearFullViewTime+'%d',\
						MuchClearViewTime=MuchClearViewTime+'%d',MuchClearViewLength=MuchClearViewLength+'%d',MuchClearFullViewTime=MuchClearViewLength+'%d',\
						ClearViewTime=ClearViewTime+'%d',ClearViewLength=ClearViewLength+'%d',ClearFullViewTime=ClearFullViewTime+'%d',\
						BlurViewTime=BlurViewTime+'%d',BlurViewLength=BlurViewLength+'%d',BlurFullViewTime=BlurFullViewTime+'%d' \
						where MACAddress='%s' and GameID ='%d'  and AdPosID ='%d'and SendDate = '%s'\
						end", 
						
						pFocusInfo->szMacID,
						pFocusInfo->dwGameID,
						pFocusInfo->dwAdID,
						strCurrentTime,
						
						strCurrentTime,
						(int)pFocusInfo->dwGameID,
						GetGameVersion(pFocusInfo->dwGameID),
						szArea,
						(int)pFocusInfo->dwAdID,
						(int)pFocusInfo->dwLoadCount,
						(int)pFocusInfo->dwClickCount,
						(int)pFocusInfo->pi[0].dwPlayCount,
						(int)pFocusInfo->pi[0].dwPlayTime,
						(int)pFocusInfo->pi[0].dwFullPlayCount,
						(int)pFocusInfo->pi[1].dwPlayCount,
						(int)pFocusInfo->pi[1].dwPlayTime,
						(int)pFocusInfo->pi[1].dwFullPlayCount,
						(int)pFocusInfo->pi[2].dwPlayCount,
						(int)pFocusInfo->pi[2].dwPlayTime,
						(int)pFocusInfo->pi[2].dwFullPlayCount,
						(int)pFocusInfo->pi[3].dwPlayCount,
						(int)pFocusInfo->pi[3].dwPlayTime,
						(int)pFocusInfo->pi[3].dwFullPlayCount,
						pFocusInfo->szMacID,
						dwIP2csIP(pFocusInfo->dwAddress),

						(int)pFocusInfo->dwLoadCount,
						(int)pFocusInfo->dwClickCount,
						(int)pFocusInfo->pi[0].dwPlayCount,
						(int)pFocusInfo->pi[0].dwPlayTime,
						(int)pFocusInfo->pi[0].dwFullPlayCount,
						(int)pFocusInfo->pi[1].dwPlayCount,
						(int)pFocusInfo->pi[1].dwPlayTime,
						(int)pFocusInfo->pi[1].dwFullPlayCount,
						(int)pFocusInfo->pi[2].dwPlayCount,
						(int)pFocusInfo->pi[2].dwPlayTime,
						(int)pFocusInfo->pi[2].dwFullPlayCount,
						(int)pFocusInfo->pi[3].dwPlayCount,
						(int)pFocusInfo->pi[3].dwPlayTime,
						(int)pFocusInfo->pi[3].dwFullPlayCount,

						pFocusInfo->szMacID,
						pFocusInfo->dwGameID,
						pFocusInfo->dwAdID,
						strCurrentTime);

		if(g_SQLOperate.ExecuteSQL(strSQL)!=1)
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d",time.Format("%Y-%m-%d %H:%M:%S"),"FocusInfo ����ʧ��",index);

			/*
			���ʧ�ܣ�ֹͣʣ�����ݵ����
			*/
			break;
		}
		else
		{
			nOK++;
		}
	}

	if (bWrite)
	{
		CString strTemp ;
		strTemp.Format("%d",LastFocusInfoNum+nTreatedCount);
		WritePrivateProfileString("Common","FocusInfoNum",(LPCTSTR)strTemp,g_strServiceINIPath);
	}

	time = CTime::GetCurrentTime();
	Log(g_szServiveLogFile,"%s\t%s %d�� �����ļ�¼����:%d",
		time.Format("%Y-%m-%d %H:%M:%S"),
		"FocusInfo����ɹ�",
		nOK,
		nTreatedCount
		);
	UNPROTECT
	return ;
}	

void OnPlayInfor(CString strPlayInfoPath,int LastPlayInfoNum,BOOL bWrite) 
{
	PROTECT(OnPlayInfor(CString strPlayInfoPath,int LastPlayInfoNum,BOOL bWrite) )
	OutputDebugString("OnPlayInfor() ");

	if (!GetPlayInfoFormLog(strPlayInfoPath))
	{
		CTime	time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"PlayInfo ������");
		return;
	}

	//Log(m_szLogFilePath,"%s\t%s",time.Format("%Y-%m-%d %H:%M:%S"),"���ڵ���PlayInfo��־�ļ�");
	CTime			time = CTime::GetCurrentTime();

	CString			strCurrentTime;
	DWORD	dwYear	= time.GetYear(); 
	DWORD	dwMon	= time.GetMonth();      
	DWORD	dwDay   = time.GetDay();
	DWORD	dwHour	= time.GetHour();
	DWORD	dwMin	= time.GetMinute();
	DWORD	dwSecond= time.GetSecond();
	strCurrentTime.Format("%d-%d-%d %d:%d:%d",dwYear,dwMon,dwDay,dwHour,dwMin,dwSecond);

	int nOK=0;
	for (int i=LastPlayInfoNum;i<g_PlayInfos.GetSize();i++)
	{
		PlayInfo*	playInfo=g_PlayInfos[i];

		CString strPlayTime;
		CString strCloseTime;
		CTime timeEnd(playInfo->dwCloseTime);
		CTimeSpan ts(playInfo->dwPlayTime%1000);
		CTime timeStart = timeEnd - ts;
		strPlayTime= timeStart.Format(_T("%Y-%m-%d %H:%M:%S"));
		strCloseTime= timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString strSQL = _T("");
		TCHAR szArea[9] = {0};
		sprintf(szArea,"%08x",playInfo->dwAreadID);
		strSQL.Format("insert into dbo.w_PlayerConnGameInfo (GameID,GameEdition,PlayerArea,SDKEdition,ConnStartDate,ConnEndDate,Ip,MACAddress,Chn,AddDate) \
						values('%d','%s','%s','0.0.0.0','%s','%s','%s','%s','%d','%s')",playInfo->dwGameID,GetGameVersion(playInfo->dwGameID),szArea,strPlayTime,strCloseTime,playInfo->szAddress,playInfo->szMacID,playInfo->dwChnID,strCurrentTime);

		if(g_SQLOperate.ExecuteSQL(strSQL)!=1)
		{
			CTime	time = CTime::GetCurrentTime();
			Log(g_szServiveLogFile,"%s\t %s %d",time.Format("%Y-%m-%d %H:%M:%S"),"PlayInfo ����ʧ��",i);

			/*
			���ʧ�ܣ�ֹͣʣ�����ݵ����
			*/
			break;
		}
		else
		{
			nOK++;
		}

	}

	if (bWrite)
	{
		CString strTemp;
		strTemp.Format("%d",g_PlayInfos.GetSize());
		WritePrivateProfileString("Common","PlayInfoNum",(LPCTSTR)strTemp,g_strServiceINIPath);
	}

	if(nOK>0)
	{
		time = CTime::GetCurrentTime();
		Log(g_szServiveLogFile,"%s\t%s %d��",time.Format("%Y-%m-%d %H:%M:%S"),"PlayInfo����",nOK);
	}

	UNPROTECT
}

BOOL Log(LPCTSTR lpLogFilePath,LPCTSTR lpszFormat, ...)
{
	try
	{
		TCHAR	szText[1024*2]		= { 0 };
		va_list	ap;
		va_start(ap, lpszFormat);
		::_vsntprintf(szText, 1024*2, lpszFormat, ap);
		va_end(ap);	

		FILE	*pFile	= NULL;	
		pFile	= ::_tfopen(lpLogFilePath,  _T("a+b"));
		if ( pFile != NULL )
		{
			::_ftprintf(pFile, _T("%s\r\n"), szText);
			fclose(pFile);
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
