#ifndef _APPHELP1_H
#define _APPHELP1_H

#pragma once

/*��Ϸ�����Ϣ*/
struct GameInfo
{
	DWORD					m_dwAreaID;                     //  ����
	TCHAR				    m_GameName    [ 128 ];				//	��Ϸ����
	TCHAR				    m_GameVersion [ 128 ];				//	��Ϸ�汾
	DWORD					m_dwGameID;						//	��ϷID��
	DWORD					m_dwGameVer;					//	��Ϸ�汾��Ϣ	
	DWORD					m_dwChannelID;					//	���������ɱ��
};

/* ·�����*/
class IGAPath
{
public:
	IGAPath ();
	~IGAPath();

public:
	//	ȡ����·��
	static BOOL	GetWorkingPath( LPTSTR lpszPath,DWORD   dwLen ,HANDLE hModel = NULL);

public:
	BOOL GetPluginsPath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetCachePath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetUnzippedPath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetDownloadPath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetDefaultTargetPath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetDefaultTempPath(LPTSTR pszPathName,DWORD   dwLen);
	
    /*ȡϵͳ�����ļ�����*/
	VOID GetSysCfgName(LPTSTR pszName,DWORD   dwLen);

	/*ȡ��������ļ�*/
	VOID GetAdCfgName(LPTSTR pszName,DWORD   dwLen);

	/* ȡƥ���ļ�·��*/
	VOID GetMatchFilePath(LPTSTR pszName,DWORD   dwLen);

	/* ȡ����ļ�·��*/
	VOID GetADFilePath(LPTSTR pszName,DWORD   dwLen);

	/* ȡflash�ļ�·��*/
	VOID GetFlashPath(LPTSTR pszName,DWORD   dwLen);
	VOID GetFlashCfgName(LPTSTR pszName,DWORD   dwLen);

	/* ȡvideo�ļ�·��*/
	VOID GetVideoPath(LPTSTR pszName,DWORD   dwLen);
	VOID GetVideoCfgName(LPTSTR pszName,DWORD   dwLen);

	/* ȡactive flash�ļ�·��*/
	VOID GetActiveFlashPath(LPTSTR pszName,DWORD   dwLen);
	VOID GetActiveFlashCfgName(LPTSTR pszName,DWORD   dwLen);

	/* ȡ3d�ļ�·��*/
	VOID Get3DPath(LPTSTR pszName,DWORD   dwLen);
	VOID Get3DCfgName(LPTSTR pszName,DWORD   dwLen);

	//ȡ��ȫ�����ļ�·��
	VOID GetSecurityFilePath(LPTSTR pszName,DWORD   dwLen);

	/*ȡSDK�ļ�·��*/
	VOID GetSDKZIPFileName(LPTSTR pszName,DWORD   dwLen);
private:	
	TCHAR	m_WorkingPath[_MAX_PATH +1 ];					//	DLL�Ĺ���·����Ŀ¼Ϊ��Ϸ��ִ���ļ����ڵ�Ŀ¼
};

class  IP_AREA
{
public:
	IP_AREA();
	~IP_AREA();

    DWORD   GetAreaID(LPCTSTR  lpszIP);

private:
    HINSTANCE  m_hAreaDll ;
};

#endif
