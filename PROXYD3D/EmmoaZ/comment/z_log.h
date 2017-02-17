#ifndef _Z_LOG_H
#define _Z_LOG_H
//************************************************************************************************************
// class      :  ZLog
// purpose    :  дlog��Ϣ  �Ӵ���̳еĺ�������Ҫ��дlog()������Ĭ�ϴ˺�����д��Ϣ��output���ڵ�
// Date       :  2008-5-22  10:03:57
// author     :  3o3afe1
//************************************************************************************************************
#include "base.h"


class ZLog : private nocopystruct
{	
public:
	ZLog() ;
	virtual ~ZLog();

public:
	void  Printf(LPCTSTR  lpFormat , ...)        ;    
 	ZLog& operator<<(LPCTSTR  lpStr     )        ;
	ZLog& operator<<(char           n    )       ;
	ZLog& operator<<(unsigned char  n    )       ;
	ZLog& operator<<(short          n    )       ;
	ZLog& operator<<(unsigned short n    )       ;
	ZLog& operator<<(int            n    )       ;
	ZLog& operator<<(unsigned int   n    )       ;
	ZLog& operator<<(long           n    )       ;
	ZLog& operator<<(unsigned long  n    )       ;
	ZLog& operator<<(bool           n    )       ;
    
	//�������ַ���
	ZLog& LogHex    (const unsigned char* d ,size_t len )       ;

public:
	/* ����ֵ��ʾдlog�Ƿ�ɹ� */
	virtual  BOOL Log(LPCTSTR  lpStr)      ;

	//�п�ʼ  ����
	virtual  VOID Start();
	virtual  VOID End();
};




//************************************************************************************************************
// class      :  ZLogFile
// purpose    :  дlog��Ϣ���ļ���
// Date       :  2008-5-22  11:38:10
// author     :  3o3afe1
//************************************************************************************************************
class ZLogFile :public ZLog
{
public:
	ZLogFile ();
	ZLogFile (LPCTSTR lpszName) ;	
	ZLogFile (HINSTANCE  hDll,LPCTSTR lpszName) ;	
	virtual ~ZLogFile() ;

public:
    BOOL          Attach(LPCTSTR lpszName);
	
	virtual  BOOL Log(LPCTSTR  lpStr)   ;

	//�п�ʼ  ����
	virtual  VOID Start();
	virtual  VOID End();

private:
	BOOL      _createFile(LPCTSTR lpszFileName);
	VOID      _closeFile();
	
private:
	TCHAR       m_szFileName[260];
	OVERLAPPED  ol;
	HANDLE      m_hFile;
	DWORD       m_dwSize;
};


#define  LOG               g_533DA38C_CCAC_483f_BFC0_93718BE7893B_log
#define  START_LOG 	       LOG.Start()    ;
#define  END_LOG 	       LOG.End()      ;

#if   defined(ENABLE_FILE_LOG)
   extern ZLogFile   g_533DA38C_CCAC_483f_BFC0_93718BE7893B_log;
#elif defined(ENABLE_AUTO_FILE_LOG)
   extern ZLogFile   g_533DA38C_CCAC_483f_BFC0_93718BE7893B_log;
#else
   extern ZLog       g_533DA38C_CCAC_483f_BFC0_93718BE7893B_log;
#endif

 #endif