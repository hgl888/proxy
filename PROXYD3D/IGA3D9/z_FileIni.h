/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : z_FileIni.h
*	INI�ļ���ȡ��д���װ��, add by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#pragma  once 


class CFileIni  
{
	//enum{ MAX_PATH_ = 256	};
public:
	CFileIni();
	~CFileIni();

	VOID Attach(LPCTSTR szFileTotalName);

	BOOL GetString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal = _T("") );
	BOOL GetInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal = 0 );

	BOOL WriteString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal = _T("") );
	BOOL WriteInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal = 0 );

private:
	TCHAR   m_szFileName[MAX_PATH];

public:
    static LPCTSTR GetAppPath();
	static LPCTSTR GetAppPath(LPCTSTR szFileName);
};

extern CFileIni g_CFileIni;
