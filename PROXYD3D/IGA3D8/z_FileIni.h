#pragma once 

class CINIFile  
{
	enum{ MAX_PATH_ = 256	};
public:
	CINIFile();
	~CINIFile();

	VOID Attach(LPCTSTR szFileTotalName);

	BOOL GetString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal = _T("") );
	BOOL GetInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal = 0 );

	BOOL WriteString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal = _T("") );
	BOOL WriteInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal = 0 );

private:
	TCHAR   szFileName[MAX_PATH_];

public:
    static LPCTSTR GetAppPath();
	static LPCTSTR GetAppPath(LPCTSTR szFileName);
};

extern CINIFile g_ZFileIni;



