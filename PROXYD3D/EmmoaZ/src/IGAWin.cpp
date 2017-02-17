#include "stdafx.h"

// #pragma warning (disable: 4786)
// #include <deque>

#include "z_FileIni.h"
#include "AppHelp.h"
#include "IGAWin.h"


namespace IGAWIN
{	
	VOID   CloseVideo         ( );
	VOID   CloseFlash         ( );
	VOID   CloseActiveFlash   ( );

    //  IAG��������dll
	static   HWND       s_hHookWnd     =  NULL;
	static   WNDPROC    s_winOldProc   =  NULL;
	const DWORD   IGA_WM_SHOW_IGA_WIN  = ::RegisterWindowMessage(_T("IGA_CRETE_FLASH_HAHAHA_ZZF"));	   
	const DWORD   IGA_WM_CLOSE_IGA_WIN = ::RegisterWindowMessage(_T("IGA_CLOSE_FLASH_HAHAHA_ZZF"));	   

	struct   IGA_WINDOW_MSG
	{
        int    nType;         //����   1:flash��2��video
	
		int    dwADID;       //���id
		int    nWaitTime;    //��ʼ�ȴ�ʱ��s
		int    nPlayMode;    //����ģʽ 
		int    nPlayTime;    //����ʱ��
		
       
		int    x0;
		int    y0;
		int    x1;
		int    y1;
		
		TCHAR  szFileName[ _MAX_PATH ];
	};

	typedef BOOL    (WINAPI *LP_FUN_CLOSEIGAWINDOW)(DWORD  dwWnd);
	typedef BOOL    (WINAPI *LP_PFUN_SHOWIGAWINDOW)(HWND  hParent, int nLeft,int nTop,int nRight,int nBottom,LPCTSTR  szName , DWORD dwType ,LPVOID lpReserve);
	typedef BOOL    (WINAPI *LP_PFUN_UpdateRes)(DWORD dwWnd,LPCTSTR lpszName,int nLeft,int nTop,int nRight,int nBottom);

	//�ص�	
	typedef BOOL     (WINAPI *LP_FUN_CALLBACKPLAYTIME)(DWORD  dwWin,DWORD dwType,DWORD  dwCurTime,LPVOID  lpReserve );		
	typedef BOOL     (WINAPI *LP_FUN_INITIGA)(LP_FUN_CALLBACKPLAYTIME,LPVOID );


    static   HINSTANCE s_hIGA_WindowDll = NULL;
	static   LP_FUN_CLOSEIGAWINDOW    s_pFunclose  = NULL;
	static   LP_PFUN_SHOWIGAWINDOW    s_pFunShow   = NULL;
	static   LP_PFUN_UpdateRes        s_pFunUpdate = NULL;
	static   LP_FUN_INITIGA           s_pFunInitIGA = NULL;


	HANDLE   s_hShowWindowEvent   = NULL;    //��ʾ�����¼�
	HANDLE   s_hEndThreadEvent    = NULL;

	HANDLE   s_hFlashTime         = NULL ;
	HANDLE   s_hActiveFlashTime   = NULL ;
	HANDLE   s_hVideTime          = NULL ;

	//�ȴ���ʾ�Ĵ���
	CRITICAL_SECTION      s_cr;
	CPtrList            s_msg_list;

	//���洰���־
    #define         FLASH_WND_INDEX            0  
	#define         ACTIVE_FLASH_WND_INDEX     1
	#define         VIDEO_WND_INDEX            2
	static DWORD    s_hWndCook[3] ;
	static DWORD    s_ADID    [3] ;


	//�����߳̾��
	static  HANDLE  s_hSendThread     = NULL;
	static  DWORD   s_dwSendThreadID  = 0;


	//�ص�
    static  LP_FUN_PLAY_FOCUS   s_PlayFocus  = NULL;
	BOOL  WINAPI IGAWIN_CallBack (DWORD  dwWin,DWORD dwType,DWORD  dwCurTime,LPVOID  lpReserve )
	{
		if  ( NULL == s_PlayFocus )  return FALSE;

		int i;
		//��ʼ��ʾ
		if  (dwType == 1)
		{
			i= (int) lpReserve;
			if  ( i >=3 )   return FALSE;
        
		    s_PlayFocus(s_ADID[i],dwType,dwCurTime,NULL);			
			return TRUE;
		}

		//�ر�
		for ( i = 0 ;i<3 ;i++)
		{
			if  (dwWin == s_hWndCook[i] )
			{
               s_PlayFocus(s_ADID[i],dwType,dwCurTime,NULL);			
			}
		}


		return TRUE;
	}
    DWORD WINAPI SendThread(LPVOID lp);
	//������ʾ���� dll

	BOOL   InitWindow(LP_FUN_PLAY_FOCUS  playFocus )
	{
		if ( s_hIGA_WindowDll != NULL )  return TRUE;

		TCHAR   TmpBuf[256];
		TCHAR   szTmpPath[256];
		::GetModuleFileName(NULL,TmpBuf,256);
		TCHAR* p = _tcsrchr(TmpBuf,'\\');
		*p = 0;
		
		wsprintf(szTmpPath,_T("%s\\EmmoaW.IAF"),TmpBuf);
		s_hIGA_WindowDll = ::LoadLibrary(szTmpPath);
		
		if  ( s_hIGA_WindowDll != NULL )
		{		
			s_pFunShow    = (LP_PFUN_SHOWIGAWINDOW)::GetProcAddress(s_hIGA_WindowDll,_T("ShowIGAWindow"));
			s_pFunclose   = (LP_FUN_CLOSEIGAWINDOW)::GetProcAddress(s_hIGA_WindowDll,_T("CloseIGAWindow"));
			s_pFunUpdate  = (LP_PFUN_UpdateRes)::GetProcAddress(s_hIGA_WindowDll,_T("UpdateRes"));
			s_pFunInitIGA = (LP_FUN_INITIGA) ::GetProcAddress(s_hIGA_WindowDll,_T("InitIga"));
		    
			if  ( NULL != s_pFunInitIGA )
			{
                 s_pFunInitIGA(IGAWIN_CallBack,NULL);
			}
			s_PlayFocus = playFocus;
		}

		s_hWndCook [ 0  ]   =0;
		s_hWndCook [ 1  ]   =0;
		s_hWndCook [ 2  ]   =0;

		s_hShowWindowEvent  = ::CreateEvent(NULL,TRUE,FALSE,NULL);
        s_hEndThreadEvent   = ::CreateEvent(NULL,TRUE,FALSE,NULL);

		s_hFlashTime        =  ::CreateWaitableTimer(NULL,FALSE,NULL);
		s_hActiveFlashTime  =  ::CreateWaitableTimer(NULL,FALSE,NULL);
        s_hVideTime         =  ::CreateWaitableTimer(NULL,FALSE,NULL);

		::InitializeCriticalSection(&s_cr);

		//
		//����һ���̣߳����̵߳ȴ�5���¼����߳̿�ʼʱ����Щ�¼���Ϊ��
		//
        s_hSendThread = ::CreateThread(NULL,0,SendThread,0,0,&s_dwSendThreadID);

		return (s_hIGA_WindowDll!=NULL);
		
	}
	
	VOID   UnInitWindow()
	{
		

		if  (NULL != s_pFunInitIGA )
		    s_pFunInitIGA(NULL,NULL);		

		ASSERT(s_hEndThreadEvent != NULL );
		if  ( NULL != s_hEndThreadEvent )
		    ::SetEvent(s_hEndThreadEvent);
		
		if  ( s_hSendThread != NULL )
		{	
			START_LOG ;
			if  ( WAIT_OBJECT_0 == ::WaitForSingleObject(s_hSendThread,1000))
			{
                LOG  << "�������������߳�";
			}
			else
			{
				LOG  << "ǿ�ƽ��������߳�";
                ::TerminateThread(s_hSendThread,11);
			}
			END_LOG;
			s_hSendThread      = NULL;
			s_dwSendThreadID   = 0;
		}

		::CloseHandle(s_hEndThreadEvent);
		s_hEndThreadEvent = NULL;

		::CloseHandle(s_hShowWindowEvent);		
		s_hShowWindowEvent = NULL;

		::CancelWaitableTimer(s_hFlashTime);
		::CloseHandle(s_hFlashTime);
		s_hFlashTime = NULL;

		
		::CancelWaitableTimer(s_hActiveFlashTime);
		::CloseHandle(s_hActiveFlashTime);
		s_hActiveFlashTime = NULL;

		::CancelWaitableTimer(s_hVideTime);
		::CloseHandle(s_hVideTime);
		s_hVideTime = NULL;

		s_pFunShow   = NULL;
		s_pFunclose  = NULL;
		s_pFunUpdate = NULL;
		
		if( s_hIGA_WindowDll )		
			::FreeLibrary(s_hIGA_WindowDll);
		s_hIGA_WindowDll      = NULL;
		::DeleteCriticalSection(&s_cr);		

		//
		//
		//
		if  ( s_hHookWnd != NULL && s_winOldProc != NULL)
		    ::SetWindowLong(s_hHookWnd,GWL_WNDPROC,(LONG)s_winOldProc);				
	}   
	

	//���庯��
	LRESULT CALLBACK   IGAWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if      ( msg == WM_SETCURSOR )
		{			
			CallWindowProc(s_winOldProc,hWnd, msg, wParam, lParam);
			return 0;
		}
		else if ( msg == WM_DESTROY  )
		{
			//������������ظ�����Դ

// 			CloseFlash();			
// 			CloseVideo();			
// 			CloseActiveFlash();
// 			s_winOldProc  = NULL;
// 			s_hHookWnd    = NULL;
		}
		else if ( msg == IGA_WM_SHOW_IGA_WIN)
		{		
	       IGA_WINDOW_MSG*   pMsg  =  (IGA_WINDOW_MSG* )wParam;
		   if  ( NULL == pMsg)    return ::DefWindowProc(hWnd, msg, wParam, lParam);

		   //
		   //��ʾ���Ŵ�����߸��²��ŵ��ļ�
		   //
		   if  ( s_hWndCook[pMsg->nType] == 0 )
		   {        
			   if ( s_pFunShow != NULL )
			   {
				   
				  s_ADID    [pMsg->nType] =  pMsg->dwADID;
				  if ( pMsg->nType == VIDEO_WND_INDEX )
					   s_hWndCook[pMsg->nType] =  s_pFunShow(hWnd,	
											pMsg->x0,pMsg->y0,pMsg->x1,pMsg->y1,
											pMsg->szFileName,
											1,//��Ƶ����
											(LPVOID)pMsg->nType);				  
				  else
					   s_hWndCook[pMsg->nType] =  s_pFunShow(hWnd,	
											pMsg->x0,pMsg->y0,pMsg->x1,pMsg->y1,
											pMsg->szFileName,
											0,//flash���� 
											(LPVOID)pMsg->nType);				  
				  TRACE("��ʾ����  = %d",pMsg->nType);
			   }
		   }
		   else
		   {
			   //���²��ŵ��ļ�
			   if  ( s_pFunUpdate != NULL)
			   {
				   s_ADID    [pMsg->nType] =  pMsg->dwADID;
				   s_pFunUpdate(s_hWndCook[pMsg->nType],pMsg->szFileName,	pMsg->x0,pMsg->y0,pMsg->x1,pMsg->y1);				  
				   TRACE("���´���  = %d",pMsg->nType);
			   }
		   }
		   
		   //
		   //���������ʱ��
		   //
		   if  ( pMsg->nPlayTime != 0 )
		   {
			   //���¶�ʱ��
			   const LONGLONG nTimerUnitsPerSecond = -10000000;
			   LARGE_INTEGER   li;
			   li.QuadPart = nTimerUnitsPerSecond * (LONGLONG)pMsg->nPlayTime;
			   switch ( pMsg->nType )
			   {
				   case VIDEO_WND_INDEX:
					   {   
						   ::SetWaitableTimer(s_hVideTime,&li,0,NULL,NULL,FALSE);
					   }
					   break;
				   case FLASH_WND_INDEX:
					   {   
						   ::SetWaitableTimer(s_hFlashTime,&li,0,NULL,NULL,FALSE);
					   }

					   break;
				   case ACTIVE_FLASH_WND_INDEX:
					   {   
						   ::SetWaitableTimer(s_hActiveFlashTime,&li,0,NULL,NULL,FALSE);
					   }

					   break;
				   default:
					   break;
			   }
		   }
			  
		   delete pMsg;		   
		   return ::DefWindowProc(hWnd, msg, wParam, lParam);
		}
		else if  ( msg == IGA_WM_CLOSE_IGA_WIN)
		{
			
			switch ( wParam )
			{
				case FLASH_WND_INDEX:
					CloseFlash();
					TRACE("�رմ��� flash");
					break;
				case VIDEO_WND_INDEX:
					CloseVideo();
					TRACE("�رմ��� video");
					break;
				case ACTIVE_FLASH_WND_INDEX:
					CloseActiveFlash();
					TRACE("�رմ��� �flash");
					break;
			}	
			return ::DefWindowProc(hWnd, msg, wParam, lParam);
		}
		
		//
		//����ԭ����
		//
		LRESULT lResult = ::CallWindowProc(s_winOldProc,hWnd, msg, wParam, lParam);			
		if ( lResult )
			lResult = ::DefWindowProc(hWnd, msg, wParam, lParam);		
		return lResult;
	}

	void TreateShowEvent(DWORD dwStart)
	{
		//
		//����HOOK
		//
		if  ( s_winOldProc == NULL )
		{
			DWORD dwProcessID = 0;
			HWND  hForeWnd    = ::GetForegroundWindow();
			if  ( NULL == hForeWnd )   
				return;
			
			//����ID���
			::GetWindowThreadProcessId(hForeWnd,&dwProcessID);
			if  ( dwProcessID != ::GetCurrentProcessId())
				return;

			//���ڴ�С���
			RECT  rc;
			::GetWindowRect(hForeWnd,&rc);
			if  ( rc.bottom - rc.top  < 300 || rc.right - rc.left <400)
				return;

			//
			//�����滻
			//
			s_winOldProc = (WNDPROC)::SetWindowLong(hForeWnd,GWL_WNDPROC,(LONG) IGAWndProc);

			//��־����
			START_LOG;
			char szTmpText[200];
			wsprintf(szTmpText,
				"[HOOK  HWND]: %08X,[OLD PROC]: %08X,[OUR PROC]: %08X \r\n",
				(DWORD)hForeWnd ,
				(DWORD)s_winOldProc,
				(DWORD) IGAWndProc);
			LOG  << szTmpText;
			END_LOG;
			
			s_hHookWnd      = hForeWnd;
		}
		
		if  ( s_winOldProc == NULL )
		{
			::Sleep(1000);
			return;
		}
				
		//
		//�滻�ɹ�,������Ϣ
		//
		EnterCriticalSection(&s_cr);
		if  ( s_msg_list.GetCount() == 0 )
		{
			LeaveCriticalSection(&s_cr);
			::ResetEvent(s_hShowWindowEvent);
			return;
		}
		
		POSITION  pos = s_msg_list.GetHeadPosition();
		while(pos)
		{
			IGA_WINDOW_MSG* pMSG =(IGA_WINDOW_MSG*) s_msg_list.GetAt(pos);
			DWORD   dwTmp = ::GetTickCount() - dwStart;
			if  ( dwTmp>=(DWORD) pMSG->nWaitTime)
			{								
				::SendMessage(s_hHookWnd,IGA_WM_SHOW_IGA_WIN,(WPARAM)pMSG,0);
				s_msg_list.RemoveAt(pos);
				LeaveCriticalSection(&s_cr);
				return;
			}						
			s_msg_list.GetNext(pos);								
		}						
		
		LeaveCriticalSection(&s_cr);
	}

    //�����̺߳���    
	DWORD WINAPI SendThread(LPVOID lp)
	{	
        HANDLE   h[] = { s_hEndThreadEvent,s_hShowWindowEvent ,s_hFlashTime,s_hVideTime,s_hActiveFlashTime};
        BOOL     bRun = TRUE;
		DWORD    dwStart = ::GetTickCount();
		while(bRun)
		{	
			switch(::WaitForMultipleObjects(5,h,FALSE,INFINITE))
			{
				case  WAIT_OBJECT_0:
					  bRun = FALSE;
					  break;					   
				case  WAIT_OBJECT_0+1:
					   TreateShowEvent(dwStart);
		               break;
			    case  WAIT_OBJECT_0+2:
					  ::SendMessage(s_hHookWnd,IGA_WM_CLOSE_IGA_WIN,(WPARAM)FLASH_WND_INDEX,0);					  
					  break;
                case  WAIT_OBJECT_0+3:
					  ::SendMessage(s_hHookWnd,IGA_WM_CLOSE_IGA_WIN,(WPARAM)VIDEO_WND_INDEX,0);					  
					  break;
				case  WAIT_OBJECT_0+4:
					  ::SendMessage(s_hHookWnd,IGA_WM_CLOSE_IGA_WIN,(WPARAM)ACTIVE_FLASH_WND_INDEX,0);					  
					  break;
				default:
					::Sleep(1000);

			}
		}
		return 0;
	}

	VOID  ShowFlash   ( LPCTSTR lpszCfg )
	{
		//
		//�����ļ������ڣ�����
		//
		CFile file;
		if(!file.Open(lpszCfg,CFile::modeRead))
			return;
		else
			file.Close();
		
		IGA_WINDOW_MSG*   pMsg = new IGA_WINDOW_MSG;
		if  ( pMsg == NULL ) return;
		
		pMsg->nType   = FLASH_WND_INDEX;
		
		ZFileIni   iniCfg(lpszCfg);
		iniCfg.GetInt(_T("comm"),_T("playtime"),pMsg->nWaitTime,60);
		pMsg->nWaitTime = pMsg->nWaitTime* 1000;
		
		iniCfg.GetInt(_T("comm"),_T("playMode"),pMsg->nPlayMode,0);
		pMsg->nPlayTime =  pMsg->nPlayMode;
		
		iniCfg.GetInt(_T("comm"),_T("id"),pMsg->dwADID,1);
		
		TCHAR  szFlashDir[ _MAX_PATH +1 ];
		::_tcscpy(szFlashDir,lpszCfg);
		TCHAR*  p = ::_tcsrchr(szFlashDir,'\\');
		*p = 0;
		
		TCHAR  szName    [ _MAX_PATH +1 ];
		iniCfg.GetString(_T("comm"),_T("name"),szName,_MAX_PATH);   
		
		wsprintf(pMsg->szFileName,_T("%s\\%s"),szFlashDir,szName);
		
		//
		//��Դ�ļ������ڣ��ͷ��ڴ棬����
		//
		if(!file.Open(pMsg->szFileName,CFile::modeRead))
		{
			delete pMsg;
			pMsg = NULL;
			return;
		}
		else
			file.Close();
		
		
		iniCfg.GetInt(_T("pos"),_T("x0"),pMsg->x0,0);
		iniCfg.GetInt(_T("pos"),_T("x1"),pMsg->x1,200);
		iniCfg.GetInt(_T("pos"),_T("y0"),pMsg->y0,0);
		iniCfg.GetInt(_T("pos"),_T("y1"),pMsg->y1,200);   
		
        
		EnterCriticalSection(&s_cr);
		//
		//����Ѿ��������Ϣ���޸ĵ���
		//
		POSITION  pos = s_msg_list.GetHeadPosition();
		while(pos)
		{
			IGA_WINDOW_MSG* pMSG1 =(IGA_WINDOW_MSG*) s_msg_list.GetAt(pos);
			if  ( pMSG1->nType == pMsg->nType)
			{
				memcpy(pMSG1,pMsg,sizeof(IGA_WINDOW_MSG));
				delete pMsg;
				break;
			} 			
			s_msg_list.GetNext(pos);								
		}
		
		//û�У�׷�Ӹ���Ϣ
		if  ( !pos )
			s_msg_list.AddTail(pMsg);
		
		LeaveCriticalSection(&s_cr);
		
		::SetEvent(s_hShowWindowEvent);
	}
	
	VOID  ShowVideo   ( LPCTSTR lpszCfg )
	{
		//
		//�����ļ������ڣ�����
		//
		CFile file;
		if(!file.Open(lpszCfg,CFile::modeRead))
			return;
		else
			file.Close();
        
		IGA_WINDOW_MSG*   pMsg = new IGA_WINDOW_MSG;
		if  ( pMsg == NULL ) return;
		
		pMsg->nType   = VIDEO_WND_INDEX;
		
		ZFileIni   iniCfg(lpszCfg);
		iniCfg.GetInt(_T("comm"),_T("playtime"),pMsg->nWaitTime,60);
		pMsg->nWaitTime = pMsg->nWaitTime* 1000;
		
		iniCfg.GetInt(_T("comm"),_T("playMode"),pMsg->nPlayMode,0);
		pMsg->nPlayTime =  pMsg->nPlayMode;

		iniCfg.GetInt(_T("comm"),_T("id"),pMsg->dwADID,1);
		
		TCHAR  szFlashDir[ _MAX_PATH +1 ];
		::_tcscpy(szFlashDir,lpszCfg);
		TCHAR*  p = ::_tcsrchr(szFlashDir,'\\');
		*p = 0;
		
		TCHAR  szName    [ _MAX_PATH +1 ];
		iniCfg.GetString(_T("comm"),_T("name"),szName,_MAX_PATH);   
		
		wsprintf(pMsg->szFileName,_T("%s\\%s"),szFlashDir,szName);

		//
		//��Դ�ļ������ڣ��ͷ��ڴ棬����
		//
		if(!file.Open(pMsg->szFileName,CFile::modeRead))
		{
			delete pMsg;
			pMsg = NULL;
			return;
		}
		else
			file.Close();

		iniCfg.GetInt(_T("pos"),_T("x0"),pMsg->x0,0);
		iniCfg.GetInt(_T("pos"),_T("x1"),pMsg->x1,200);
		iniCfg.GetInt(_T("pos"),_T("y0"),pMsg->y0,0);
		iniCfg.GetInt(_T("pos"),_T("y1"),pMsg->y1,200);   
		
        
		EnterCriticalSection(&s_cr);	

 		POSITION  pos = s_msg_list.GetHeadPosition();
 		while(pos)
 		{
 			IGA_WINDOW_MSG* pMSG1 =(IGA_WINDOW_MSG*) s_msg_list.GetAt(pos);
			if  ( pMSG1->nType == pMsg->nType)
			{
				memcpy(pMSG1,pMsg,sizeof(IGA_WINDOW_MSG));
				delete pMsg;
				break;
			} 			
 			s_msg_list.GetNext(pos);								
 		}						
							
		if  ( !pos )
		    s_msg_list.AddTail(pMsg);


		LeaveCriticalSection(&s_cr);
		
		::SetEvent(s_hShowWindowEvent);

	}

	VOID  ShowActiveFlash   ( LPCTSTR lpszCfg )
	{
		//
		//�����ļ������ڣ�����
		//
		CFile file;
		if(!file.Open(lpszCfg,CFile::modeRead))
			return;
		else
			file.Close();
        
		IGA_WINDOW_MSG*   pMsg = new IGA_WINDOW_MSG;
		if  ( pMsg == NULL ) return;
		
		pMsg->nType   = ACTIVE_FLASH_WND_INDEX;
		
		ZFileIni   iniCfg(lpszCfg);
		iniCfg.GetInt(_T("comm"),_T("playtime"),pMsg->nWaitTime,10);
		pMsg->nWaitTime = pMsg->nWaitTime* 1000;
		
		iniCfg.GetInt(_T("comm"),_T("playMode"),pMsg->nPlayMode,0);		
		pMsg->nPlayTime =  pMsg->nPlayMode;

		iniCfg.GetInt(_T("comm"),_T("id"),pMsg->dwADID,1);

		TCHAR  szFlashDir[ _MAX_PATH +1 ];
		::_tcscpy(szFlashDir,lpszCfg);
		TCHAR*  p = ::_tcsrchr(szFlashDir,'\\');
		*p = 0;
		
		TCHAR  szName    [ _MAX_PATH +1 ];
		iniCfg.GetString(_T("comm"),_T("name"),szName,_MAX_PATH);   
		
		wsprintf(pMsg->szFileName,_T("%s\\%s"),szFlashDir,szName);

		//
		//��Դ�ļ������ڣ��ͷ��ڴ棬����
		//
		if(!file.Open(pMsg->szFileName,CFile::modeRead))
		{
			delete pMsg;
			pMsg = NULL;
			return;
		}
		else
			file.Close();

		iniCfg.GetInt(_T("pos"),_T("x0"),pMsg->x0,0);
		iniCfg.GetInt(_T("pos"),_T("x1"),pMsg->x1,200);
		iniCfg.GetInt(_T("pos"),_T("y0"),pMsg->y0,0);
		iniCfg.GetInt(_T("pos"),_T("y1"),pMsg->y1,200);   
		
        
		EnterCriticalSection(&s_cr);
		POSITION  pos = s_msg_list.GetHeadPosition();
		while(pos)
		{
			IGA_WINDOW_MSG* pMSG1 =(IGA_WINDOW_MSG*) s_msg_list.GetAt(pos);
			if  ( pMSG1->nType == pMsg->nType)
			{
				memcpy(pMSG1,pMsg,sizeof(IGA_WINDOW_MSG));
				delete pMsg;
				break;
			} 			
			s_msg_list.GetNext(pos);								
		}						
		
		if  ( !pos )
			s_msg_list.AddTail(pMsg);

		LeaveCriticalSection(&s_cr);
		
		::SetEvent(s_hShowWindowEvent);
		
	}

	VOID   CloseVideo         ( )
	{		
		if  ( s_hWndCook[VIDEO_WND_INDEX ] != 0)
		{
			if  ( s_pFunclose != NULL )
				s_pFunclose(s_hWndCook[VIDEO_WND_INDEX ]);
			s_hWndCook[VIDEO_WND_INDEX ] = 0;
			
		}
	}
	VOID   CloseFlash         ( )
	{
		if  ( s_hWndCook[FLASH_WND_INDEX ] != 0)
		{
			if  ( s_pFunclose != NULL )
			{
				s_pFunclose(s_hWndCook[FLASH_WND_INDEX ]);
			}
			s_hWndCook[FLASH_WND_INDEX ] = 0;
		}
	}
	VOID   CloseActiveFlash   ( )
	{
		if  ( s_hWndCook[ACTIVE_FLASH_WND_INDEX ] != 0)
		{
			if  ( s_pFunclose != NULL )
				s_pFunclose(s_hWndCook[ACTIVE_FLASH_WND_INDEX ]);
			s_hWndCook[ACTIVE_FLASH_WND_INDEX ] = 0;
		}
	}
}

