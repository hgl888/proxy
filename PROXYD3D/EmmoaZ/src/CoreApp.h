#if !defined(_COREAPP_H)
#define _COREAPP_H

#pragma once

class KCoreApp 
{
public:
	KCoreApp();	
	virtual ~KCoreApp();    
	
public:	
	virtual BOOL    Start ();
	virtual void    Stop  ();

protected:
	virtual INT     OnInitThread();
	virtual INT     OnExit    ();	
	virtual INT     OnUpdateByXML      ();
	virtual INT     OnTimeOut ();

	INT     DoUpdate        ();
	INT     UnDo      ();
    
private:
    static DWORD WINAPI ThreadProc(void *p);

public:
    inline  VOID  SetTimeOut(DWORD  dwTimeOut) { m_dwTimeOut = dwTimeOut;}
protected:
	HANDLE    m_hWorkingThread;       //�����߳̾��
	DWORD     m_dwWorkingThreadID;    //�����߳�id
    
	HANDLE    m_hEventStop;           //ֹͣ�߳��¼�
	HANDLE    m_hEventDo ;            //�����¼�
    
	DWORD     m_dwWait;
	DWORD     m_dwTimeOut;
};

#endif 
