// PopFlash.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "io.h"

#include "kFlash.h"
#include "KVideo.h"
#include "KIE.h"


CComModule      _Module;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{	
	switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
			{
				::CoInitialize(NULL);				

				AtlAxWinInit();
				IGAWinInit();
			}
			break;
		case DLL_PROCESS_DETACH:
			{				
                ::CoUninitialize();
			}
			break;
	}
    return TRUE;
}

typedef BOOL     (WINAPI *LP_FUN_CALLBACKPLAYTIMEFOCUS)(DWORD  dwWin,DWORD dwType,DWORD  dwCurTime,LPVOID  lpReserve );
LP_FUN_CALLBACKPLAYTIMEFOCUS   s_pFunCallback = NULL;

BOOL WINAPI InitIga(LP_FUN_CALLBACKPLAYTIMEFOCUS pCallback,LPVOID lpReserve)
{
	s_pFunCallback = pCallback;
	return TRUE;
}
/**********************************************************************************  
 *  ��������
 *  hParent : �����ھ��
 *  nLeft   : ���Ͻ�
 *  nTop
 *  nRight  :���½�
 *  nBottom
 *  szName  : ����ý������
*   dwType  ����������     0:f flash    1:video    2: ie
 *  ����ֵ   0��ʧ��
 ***********************************************************************************/

DWORD  WINAPI ShowIGAWindow(HWND  hParent, int nLeft,int nTop,int nRight,int nBottom,LPCTSTR  szName , DWORD dwType  ,LPVOID lpReserve)
{
	DWORD   dwResult = 0;
	try
	{

   
 	if ( szName == NULL || szName[0] == 0)       	return dwResult;

	KContainerCtl*   pWnd = NULL;
    switch(dwType)
	{
		case K_WINDOW_FLASH :
			{
				if ( _taccess(szName,0) == -1 ) 	return dwResult;

				KFlash*   pFlash = NULL;
                pWnd = KFlash::New(pFlash);
				if (NULL != pWnd)   
				{	
					/*
					����Flash�ؼ�
					*/

				    if ( !pWnd->Create( hParent))
					{
						KFlash::Delete(pWnd);
                        return dwResult;
					}

					/*ת��һ���ַ������� ?????????????*/
					((KFlash*)pWnd)->Play(szName,TRUE);
					dwResult = (DWORD)pWnd;					

					/*͸����  ???????????????????*/
					//pWnd->SetAlpah(200);

					if  ( NULL != s_pFunCallback )
					{
						s_pFunCallback(dwResult,1,::GetTickCount(),lpReserve);
					}
				}
			}
			break;
		case K_WINDOW_VIDEO:
			{
				 if ( _taccess(szName,0) == -1 ) 	return dwResult;

				 KVideo*   pVideo = NULL;
				 pWnd = KContainerCtl::New(pVideo);
				 if (NULL != pWnd)   
				 {		
					 if ( !pWnd->Create( hParent))
					 {
						 KVideo::Delete(pWnd);
						 return dwResult;
					 }			 
					 
					 ((KVideo*)pWnd)->Play(szName,TRUE);
					 dwResult = (DWORD)pWnd;
					 if  ( NULL != s_pFunCallback )
					 {
						 s_pFunCallback(dwResult,1,::GetTickCount(),lpReserve);
					 }

				 }
			}
			 break;
		case K_WINDOW_IE:			 
        default:
             dwType = K_WINDOW_IE;
			 return  dwResult;
	}

 	
	pWnd->ShowWindow(nLeft, nTop, nRight, nBottom);   
	}
	catch(...)
	{
		OutputDebugString("EMMOA: ShowIGAWindow Exception!!!");
	}
	return dwResult;
}


/**********************************************************************************  
 *  �رմ��岢���ڴ�
 *  dwWnd :  ��Show����ֵ
 *  ����ֵ  �ɹ�/shʧ��
 ***********************************************************************************/
BOOL  WINAPI CloseIGAWindow(DWORD  dwWnd)
{
    BOOL  bResult =FALSE;
    KContainerCtl* pWnd = (KContainerCtl*)dwWnd;
    
	if ( pWnd )
	{		
		pWnd->DestroyWindow();	
		if  ( NULL != s_pFunCallback )
		{
			s_pFunCallback(dwWnd,2,::GetTickCount(),NULL);
		}

		bResult = TRUE;
	}
	
	return bResult;
}

/*������ʾ����Դ*/
BOOL WINAPI UpdateRes(DWORD dwWnd,LPCTSTR lpszName, int nLeft,int nTop,int nRight,int nBottom)
{
    BOOL bResult  = FALSE;
    KContainerCtl* pWnd = (KContainerCtl*)dwWnd;


	switch (pWnd->GetWindowType())
	{
	   case K_WINDOW_FLASH:
		    pWnd->ShowWindow(nLeft, nTop, nRight, nBottom);
			((KFlash*)pWnd) ->Stop();
            bResult = ((KFlash*)pWnd) ->Play(lpszName,TRUE);
			break;
	   case K_WINDOW_VIDEO:
		    pWnd->ShowWindow(nLeft, nTop, nRight, nBottom);
			((KVideo*)pWnd) ->Stop();
            bResult = ((KVideo*)pWnd) ->Play(lpszName,TRUE);
			break;
	   case K_WINDOW_IE:
			bResult = FALSE;
			break;
	}

    return TRUE;
}


