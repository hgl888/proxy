// KFlash.cpp: implementation of the KFlash class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KFlash.h"



//////////////////////////////////////////////////////////////////////

KFlash::KFlash()
{    
	 
}

KFlash::~KFlash()
{
}

BOOL KFlash::Create(HWND hParent)
{
	BOOL  bResult = FALSE;
	
	if  ( !KWindow::Create(hParent) ) 
		return FALSE;
    
	RECT  rc;
    GetClientRect(m_hWnd, &rc );	
	HWND hwnd    = m_container.Create( m_hWnd, rc, LPCTSTR("ShockwaveFlash.ShockwaveFlash.1"), WS_CHILD|WS_VISIBLE );		    	
	if   ( hwnd == NULL )   return FALSE;	
	
	m_container.QueryControl(&m_IFlashPlayer);	
	
	return	TRUE;

}


BOOL KFlash::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
	{
		case WM_SIZE:
			{
				if  ( m_container != NULL )
				{				
					RECT  rc;
					GetClientRect(m_hWnd, &rc );
					m_container.MoveWindow(&rc,TRUE);
				}
			}      
			break;
		case WM_TIMER :
			{
				if  ( m_nMinute == 0 )
				{
					m_bStop = TRUE;					
				}
				else
				{
					m_nMinute--;
				}

			}
            break;
		default:
			break;
	}

	return	TRUE;
}
BOOL    KFlash::SetAlpah(BYTE  nAlpah)
{
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	if ( hInst )
	{
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC    fun = NULL; 

        fun=(MYFUNC)GetProcAddress(hInst,_T("SetLayeredWindowAttributes"));	
		if(fun)  
		{
			::SetWindowLong(m_hWnd,GWL_EXSTYLE,::GetWindowLong(m_hWnd,GWL_EXSTYLE)^0x80000); 
			fun(m_hWnd,RGB(255,255,255),nAlpah,2); 		
		}
		FreeLibrary(hInst); 
	}

	return TRUE;
}

//��ʾ����	    
BOOL    KFlash::CenterWindow(int  nWidht , int nHeight)
{
	
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);     
    int nScreenWidth  = GetSystemMetrics(SM_CXSCREEN);  

	//HWND pwnd=FindWindow("ProgMan",NULL);
	//SetParent(m_hWnd, NULL);

	SetWindowPos(m_hWnd, HWND_TOPMOST,(nScreenWidth- nWidht)/2,(nScreenHeight- nHeight)/2,nWidht,nHeight,SWP_NOMOVE|SWP_NOSIZE); 
	MoveWindow(m_hWnd,(nScreenWidth- nWidht)/2,(nScreenHeight- nHeight)/2,nWidht,nHeight,TRUE);

	TCHAR  szBuf [ 640 ];
	wsprintf(szBuf,"width =%u  nHeight = %u  ",nWidht,nHeight);
	OutputDebugString(szBuf);

	ShowWindow(m_hWnd,SW_SHOW);

	//SetForegroundWindow(m_hWnd);
	
	return FALSE;
}


/*����Container*/
BOOL  KFlash::Play(LPCTSTR lpszName,int nMinute   )
{
	if ( m_IFlashPlayer != NULL )
	{	
		_bstr_t aTrans = "Transparent";
		
        
		m_IFlashPlayer->put_WMode(aTrans);
		
		m_IFlashPlayer->Movie         = lpszName;
		m_IFlashPlayer->ScaleMode     = 2;		
		
		m_IFlashPlayer->Loop          = (VARIANT_BOOL)0xFFFF;
		m_IFlashPlayer->Menu          = (VARIANT_BOOL)0xFFFF;
		
		
		m_IFlashPlayer->Play();

		m_bStop = FALSE;
		m_nMinute = nMinute;
		::SetTimer(m_hWnd,1,1000,NULL);
		
		return TRUE;
	}
	
	return FALSE;
}

VOID  KFlash::Stop()
{
	if ( m_IFlashPlayer != NULL )
	{	
		m_IFlashPlayer->Stop();
	}
	
}

BOOL KFlash::DestroyWindow()
{

	::DestroyWindow(m_hWnd);
	return TRUE;
}

