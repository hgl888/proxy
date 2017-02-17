//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Empty starting point for new Direct3D applications
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"
#include "resource.h"
#include <WinInet.h>	//Import for INTERNET_MAX_URL_LENGTH
#include <DXUTmisc.h>
#include "D3D9Search.h"
#include "ToolFunction.h"
#include "ShowPic.h"
#include "locale.h"
#include "DefineConst.h"
#include "HookerMsg.h"
#include "UTF8ToGB.h"

//#include "D3D9IME.h"
//CD3D9IME	g_D3D9Ime;
//TCHAR		g_szStr[ MAX_PATH ];

//////////////////////////////////////////////////////////////////////////

RECT						g_rectIMEEdit;			//	�����Ĵ�С
RECT						g_rectBtnSearch;		//	������ť�Ĵ�С
RECT						g_rectBtnShowSearch;	//  ��ʾ��������İ�ť
CDXUTDialogResourceManager	g_DialogResourceManager;//�Ի�����Դ������
CDXUTDialog					g_SampleUI;             //�Ի���

DWORD						g_dwGameID = 0;			//��ϷID��ͨ�������ӿڻ��
HWND						g_hGameWnd = NULL;		//��Ϸ���ھ����ͨ�������ӿڻ��

char						g_szSearchURLHead[MAX_PATH]; //����url���ӵ�ַ����search.ini�ļ���ȡ

D3D9SEARCHSETTINGS			g_D3D9SearchSetting;		 //�Զ����������ż��ر�����ͨ���ӿڻ�ȡ����

LPWNDPROC					g_pFunOldMsgProc = NULL;		//���SetWindowLong��Ϣ��������ַ 

LPCallBack3DFocus			g_pFun3DFocus;		//���ͳ�ƴ����ĺ���ָ�룬ͨ�������ӿڻ��
DWORD						g_dwAdID;			//���ģ��ID��ͨ�������ӿڻ��

//#define	IDS_DEFAULT_SEARCHTIPSW	L"��������"

TCHAR		g_wszDefaultText[MAX_PATH];
TCHAR		g_wszB_text[50];				 //������ť��ʾ���壬��Search.ini�ļ���ȡ

//bool g_bCOMPOSITION = false;

/*
* ��MyIE�߳�
*/
DWORD WINAPI OpenIEBrowserThreadProc(void *p)
{
	tagThreadParam	*pThreadParam	= (tagThreadParam*)p;

	//{{smallfool
	CHAR	szFileName[_MAX_PATH]	= { 0 };
	GetModuleFileNameA(NULL, szFileName, _MAX_PATH);
	CHAR	*q	= strrchr(szFileName, '\\');
	if(q == NULL)
		q	= strrchr(szFileName, '/');
	*++q	= 0;
	CHAR	szModule[_MAX_PATH]	= { 0 };
	wsprintfA(szModule, "%s%s", szFileName, "EmmoaIE.exe");
	HANDLE	h	= ::CreateFileA(szModule, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h == INVALID_HANDLE_VALUE)
	{
		*szModule	= 0;
		HKEY	hKey	= NULL;
		if(::RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\EMMOA\\IGXExplorer", &hKey) == ERROR_SUCCESS)
		{
			long	dwSize	= _MAX_PATH;
			::RegQueryValueA(hKey, "", szModule, &dwSize);
			::RegCloseKey(hKey);
		}
	}
	else
	{
		::CloseHandle(h);
	}

	if(lstrlenA(szModule))
	{
		CHAR szCmdLine[INTERNET_MAX_URL_LENGTH]	= { 0 };
		wsprintfA(szCmdLine, "%s %s",szModule, pThreadParam->szURL);
		PROCESS_INFORMATION	pi	= { 0 };
		STARTUPINFOA	si	= { sizeof(STARTUPINFOA)	};
		si.wShowWindow	= SW_SHOWNORMAL;
		si.dwFlags		= STARTF_USESHOWWINDOW;

		if(!CreateProcessA(NULL, (LPSTR)szCmdLine, NULL, NULL,FALSE, 0, NULL, NULL, &si, &pi))
		{
			//	DXUT_ERR(L"CreateProcessA failed for error.", GetLastError());
			goto __Exit;
		}
	}

	//smallfool}}
__Exit:
	::HeapFree(::GetProcessHeap(), 0, pThreadParam);
	return	0;
}

/************************************************************************/
/* ����������ť����Ϣ                                                   */
/************************************************************************/
BOOL HandleSearchButtonMsg()
{
	//OutputDebugStringA("HandleSearchButtonMsg");
	WCHAR lpszSearchText[ INTERNET_MAX_URL_LENGTH ]	=	{ 0 };
	HRESULT		hr	= g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->GetTextCopy( lpszSearchText, INTERNET_MAX_URL_LENGTH );
	if(hr != S_OK )
		return	FALSE;
	//////////////////////////////////////////////////////////////////////////
	if(lpszSearchText == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return	FALSE;
	}

	int len = ::wcslen(lpszSearchText);
	if(len == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return	FALSE;
	}
	char	szText[INTERNET_MAX_URL_LENGTH]	= { 0 };
	int	nRet	= ::WideCharToMultiByte(CP_ACP, 0, lpszSearchText, -1, szText, sizeof(szText), NULL, NULL);
	szText[nRet]	= 0;
	CHAR	szFix[4]	= { 0 };
	CHAR	szBaseCodeText[INTERNET_MAX_URL_LENGTH]	= { 0 };
	for(int nIndex = 0; nIndex < nRet; nIndex++)
	{
		StringCchPrintfA(szFix, 4, "%s%02X", "%", (BYTE)szText[nIndex]);
		StringCchCatA(szBaseCodeText, INTERNET_MAX_URL_LENGTH, szFix);
	}


	StringCchPrintfA(szText, INTERNET_MAX_URL_LENGTH, "%s%s", g_szSearchURLHead, szBaseCodeText);

	//////////////////////////////////////////////////////////////////////////

	tagThreadParam	*pThreadParam	= (tagThreadParam*)HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(tagThreadParam));
	pThreadParam->hWndToMonitor	= g_hGameWnd;
	strcpy(pThreadParam->szURL, szText );
	DWORD	dwID	= 0;
	HANDLE	h = ::CreateThread(NULL, 0, OpenIEBrowserThreadProc, (void*)pThreadParam, 0, &dwID);
	::CloseHandle(h);

	return	true;

}

//-----------------------------------------------------------------------------
// Desc: ������ֿؼ���Ϣ
//-----------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, 
						 void* pUserContext )
{
	switch( nControlID )
	{
	case IDC_BUTTON_ENABLE_IME:   //����IME���뷨
		CDXUTIMEEditBox::EnableImeSystem( true );
		break;

	case IDC_BUTTON_DISABLE_IME:  //����IME���뷨
		CDXUTIMEEditBox::EnableImeSystem( false );
		break;
	case IDC_BUTTON_SEARCH:
		if (  g_pShowPic->GetbShowSearch()  )
		{
			g_pFun3DFocus( g_dwAdID, NULL );
			HandleSearchButtonMsg();
		}
		break;
	case IDC_BUTTON_CLOSE:
		{
			g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->SetVisible(true);
			g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetVisible(true);
		}
		break;
	case IDC_IME_EDITBOX:
		{
			switch(nEvent)
			case EVENT_EDITBOX_CHANGE:
				 break;
		}
		break;
	}
}

//--------------------------------------------------------------------------------------
// Rejects any devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	//OutputDebugStringA("IsDeviceAcceptable");
	//����̨��������ʽ�Ƿ�֧��Alpha��ϵȲ���(post pixel blending operations)
    IDirect3D9* pD3D = DXUTGetD3DObject(); 
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
	//OutputDebugStringA("ModifyDeviceSettings");
  //�����֧��Ӳ�����㴦����ʹ��������㴦��
    if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0)
    {
        pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

	//���ʹ�òο��豸,�򵯳�����Ի���
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning();
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3DPOOL_MANAGED resources here 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	//OutputDebugStringA("OnCreateDevice");
	HRESULT hr;
	V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3DPOOL_DEFAULT resources here 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;
	V_RETURN( g_DialogResourceManager.OnResetDevice() );
	//������ť�ؼ�λ��

	g_pShowPic->SetDevice9( pd3dDevice );
	g_pShowPic->InitPic();

	g_SampleUI.SetLocation( 0, 0 );
    g_SampleUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );


    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	return;
}


//--------------------------------------------------------------------------------------
// Render the scene 
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	//SHOWOUTPUTDEBUG(begin_OnFrameRender);
	HRESULT hr = S_OK;

	g_pShowPic->InitTranslationMatrix();
	g_pShowPic->RenderPic();

	DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" ); 
	if ( g_pShowPic->GetbShowSearch()  )
	{
		V( g_SampleUI.OnRender( fElapsedTime ) );
	}
	DXUT_EndPerfEvent(); 
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
						 bool* pbNoFurtherProcessing, void* pUserContext )
{
	if ( ! g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->m_bHasFocus && g_pFunOldMsgProc != NULL )
	{
		g_pFunOldMsgProc( hWnd, uMsg, wParam, lParam);
	}
	g_pShowPic->MsgProc( hWnd, uMsg, wParam, lParam );
	
	//g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->SetLocation(g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2, g_pShowPic->GetiyPos() );
	//g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetLocation(g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2 + g_GUISetting.iSearchBoxWidth , g_pShowPic->GetiyPos() );

	switch(uMsg)
	{
	case WM_KEYUP:
		{
			if(wParam == g_D3D9SearchSetting.wKey)
			{
				if(g_SampleUI.GetVisible())
				{
					g_SampleUI.SetVisible(false);
				}
				else
				{
					g_SampleUI.SetVisible(true);
				}
				*pbNoFurtherProcessing	= false;
				return	0;
			}
		}
		break;
	}


    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

	*pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
	
}

/*
* ʹ��SetWindowLong�������Ϣ����
*/
LRESULT CALLBACK MsgProcProxy( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//int wmId, wmEvent;
	//switch (uMsg)
	//{
	//case WM_KEYDOWN:
	//	switch( wParam )
	//	{
	//	case VK_F1:
	//		g_D3D9Ime.NextIme();
	//		break;
	//	case VK_F2:
	//		g_D3D9Ime.SharpIme( hWnd );
	//		break;
	//	case VK_F3:
	//		g_D3D9Ime.SymbolIme( hWnd );
	//		break;
	//	case VK_F5:
	//		g_D3D9Ime.EnableIme();
	//		break;
	//	case VK_F6:
	//		g_D3D9Ime.DisableIme();
	//		break;
	//	case VK_ESCAPE:
	//		PostMessage( hWnd, WM_CLOSE, 0, 0 );
	//	}
	//	break;



	//case WM_INPUTLANGCHANGEREQUEST:
	//	// ѡ���µ����뷨ʱ��������Ϣ����ֱ��ʹ��Ctrl+Space�򿪻�ر����뷨ʱ����������Ϣ
	//	return g_D3D9Ime.OnWM_INPUTLANGCHANGEREQUEST()? 0 : DefWindowProc( hWnd, uMsg, wParam, lParam );
	//	break;

	//case WM_INPUTLANGCHANGE:
	//	// ���뷨����ʱ����
	//	g_D3D9Ime.OnWM_INPUTLANGCHANGE( hWnd );
	//	break;

	//case WM_IME_NOTIFY:
	//	// ͨ�����뷨��״̬
	//	g_D3D9Ime.OnWM_IME_NOTIFY( hWnd, wParam );
	//	return 0;

	//case WM_IME_COMPOSITION:
	//	// ���뷨����ʱ��������Ϣ
	//	g_D3D9Ime.OnWM_IME_COMPOSITION( hWnd, lParam, g_szStr );
	//	return 0;
	//	

	//case WM_IME_SETCONTEXT: 
	//	// ������ڴ��������뷨���󣬴��ڼ���ʱ�Զ���������Ϣ��ͨ������DefWindowProc���������ﲻ��Ҫ��
	//case WM_IME_STARTCOMPOSITION:
	//	//�����û�ʹ�����뷨����ʱ���Ȳ�������Ϣ������֪ͨϵͳ�������ͨ������DefWindowProc���������ﲻ��Ҫ��
	//case WM_IME_ENDCOMPOSITION:
	//	// ���������ͨ������DefWindowProc���������ﲻ��Ҫ��
	//	OutputDebugStringW( g_szStr );
	//	memset(g_szStr, 0, MAX_PATH);
	//	break;
	//}

	//////////////////////////////////////////////////////////////////////////
	if (  uMsg == WM_KEYUP )
	{
		//SHOWOUTPUTDEBUG( MsgProc );
		uMsg = WM_IME_COMPOSITION;
	}
	g_pShowPic->MsgProc( hWnd, uMsg, wParam, lParam );
	
	//g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->SetLocation(g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2, g_pShowPic->GetiyPos() );
	//g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetLocation(g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2 + g_GUISetting.iSearchBoxWidth , g_pShowPic->GetiyPos() );

	
	if ( g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam ) )
	{
		return 0;
	}

	if (g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam ))
	{
		return 0;
	}

	if (g_dwGameID == IGA_WEIWUDUZUN )
	{
		if (  g_pFunOldMsgProc != NULL )
		{
			return g_pFunOldMsgProc( hWnd, uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}
	}

	if ( ! g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->m_bHasFocus && g_pFunOldMsgProc != NULL )
	{
		return g_pFunOldMsgProc( hWnd, uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

}



//--------------------------------------------------------------------------------------
// Release resources created in the OnResetDevice callback here 
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
	//OutputDebugStringA("OnLostDevice");
	g_DialogResourceManager.OnLostDevice();
}


//--------------------------------------------------------------------------------------
// Release resources created in the OnCreateDevice callback here
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
	//OutputDebugStringA("OnDestroyDevice");
	g_DialogResourceManager.OnDestroyDevice();
}


/************************************************************************/
/* �������ļ��л�ȡ����ɫֵ����Ϊ�������ɫ����                         */
/************************************************************************/
D3DCOLOR ColorFromText(LPCSTR lpszText)
{
	CHAR	szText[32]	= { 0 };
	strncpy(szText, lpszText, 32);
	const	char szSeparators[]	= ",. |;\r\n";
	DWORD	dwValue[4]	= { 0 };
	int		nIndex	= 0;
	CHAR	*p	= strtok(szText,szSeparators);
	while( p != NULL && nIndex < 4)
	{
		if(lstrlenA(p))
		{
			dwValue[nIndex++] = atoi(p);
		}
		p = strtok(NULL, szSeparators);
	}
	return	D3DCOLOR_ARGB(dwValue[0], dwValue[1], dwValue[2], dwValue[3]);
}

/************************************************************************/
/* �������ļ��м���GUI��������Ϣ                                        */
/************************************************************************/
BOOL LoadGUIDefaultFromConfig()
{
	CHAR szConfigFile[_MAX_PATH]	= { 0 };
	CHAR szBuffer[512]	= { 0 };

	::GetModuleFileNameA(NULL, szBuffer, _MAX_PATH);
	CHAR * p = strrchr(szBuffer, '\\');
	if(p == NULL)
		p = strrchr(szBuffer, '/');
	*++p = 0;
	sprintf(szConfigFile, "%s%s%s", szBuffer, ADINFO_EMMOA_RES_FLASH, "search.ini");

	*szBuffer	= 0;

	MultiByteToWideChar(CP_ACP, 0, "����", -1, g_D3D9SearchSetting.szFontName1, sizeof(g_D3D9SearchSetting.szFontName1));
	g_D3D9SearchSetting.nFontSize1	= 16;			
	
	GetPrivateProfileStringA("SearchBox", "BackColorARGB","128, 255, 255, 255", szBuffer, sizeof(szBuffer), szConfigFile);
	g_D3D9SearchSetting.clrTexture	= ColorFromText( szBuffer );	
	g_D3D9SearchSetting.clrText	= ColorFromText("255, 64, 64, 64");
	g_D3D9SearchSetting.clrCaret	= ColorFromText("255, 64, 64, 64");
	g_D3D9SearchSetting.clrTextSelected	= ColorFromText("255, 0, 255, 255");
	g_D3D9SearchSetting.clrBackSelected	= ColorFromText("255, 72, 72, 72");
	
	GetPrivateProfileStringA("url", "SearchURLHead", "http://www.baidu.com/s?wd=", g_szSearchURLHead, sizeof(g_szSearchURLHead), szConfigFile);
	char szBtnText[MAX_PATH];
	memset(g_wszB_text, 0, sizeof(g_wszB_text) );
	GetPrivateProfileStringA("SearchBox", "b_text", "Search", szBtnText, sizeof(szBtnText), szConfigFile);
	MultiByteToWideChar( CP_UTF8,0, szBtnText, strlen(szBtnText)+1, g_wszB_text,	MAX_PATH );

	char szDefaultText[MAX_PATH];
	memset(g_wszDefaultText, 0, sizeof(g_wszDefaultText) );
	GetPrivateProfileStringA("SearchBox", "DefaultText", "��������", szDefaultText,sizeof(szDefaultText), szConfigFile);
	MultiByteToWideChar( CP_UTF8,0, szDefaultText, strlen(szDefaultText)+1, g_wszDefaultText,	MAX_PATH );


	g_D3D9SearchSetting.iSearchBoxWidth = GetPrivateProfileIntA("SearchBox", "width",320, szConfigFile);
	g_D3D9SearchSetting.iSearchBoxHeight = GetPrivateProfileIntA("SearchBox", "height",50, szConfigFile);
	g_D3D9SearchSetting.iSearchBtnWidth = GetPrivateProfileIntA("SearchBox", "b_width",60, szConfigFile);
	g_D3D9SearchSetting.iSearchBtnHeight = GetPrivateProfileIntA("SearchBox", "b_height",50, szConfigFile);
	
	return	TRUE;

}

/************************************************************************/
/* �����½ӿڣ���֧�ָ��๦��                                           */
/************************************************************************/
HRESULT WINAPI D3D9SearchInit(LPDIRECT3DDEVICE9 pDevice, LPARAM lParam)
{
	setlocale(LC_ALL,"Chinese-simplified");
	D3DSEARCH_PARAM	*pInitializeParam	= (D3DSEARCH_PARAM*)lParam;
	assert(pDevice != NULL);
	assert(pInitializeParam != NULL);

	if ( g_pShowPic != NULL )
	{
		delete g_pShowPic;
		g_pShowPic = NULL; 
	}
	g_pShowPic = new CShowPic();
	g_pShowPic->SetDevice9( pDevice);
	g_pShowPic->InitPic();
	
	g_dwGameID = pInitializeParam->dwGameID;
	g_hGameWnd = pInitializeParam->hWndFocus;
	g_pShowPic->SetGameWnd( g_hGameWnd );

	g_pFun3DFocus = pInitializeParam->pFun3DFocus;
	g_dwAdID	  = pInitializeParam->dwAdID;

	if ( g_pFunOldMsgProc == NULL )
	{
		if(IsWindowUnicode(g_hGameWnd))
		{
			g_pFunOldMsgProc = (LPWNDPROC)SetWindowLongW( g_hGameWnd, GWL_WNDPROC, (LONG)MsgProcProxy );
		}
		else
		{
			g_pFunOldMsgProc = (LPWNDPROC)SetWindowLongA( g_hGameWnd, GWL_WNDPROC, (LONG)MsgProcProxy );
		}
	}

	char szOutputString[MAX_PATH ];
	sprintf( szOutputString, "Handle = %08x, Proc = %08x", g_hGameWnd, g_pFunOldMsgProc );
	OutputDebugStringA( szOutputString );


	IDirect3D9*	pD3D9	= NULL;
	if(FAILED(pDevice->GetDirect3D(&pD3D9)))
		return	E_FAIL;

	DXUTSetD3D9(pD3D9);

	// Set the callback functions
	DXUTSetCallbackDeviceCreated( OnCreateDevice );
	DXUTSetCallbackDeviceReset( OnResetDevice );
	DXUTSetCallbackDeviceLost( OnLostDevice );
	DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackFrameRender( OnFrameRender );
	DXUTSetCallbackFrameMove( OnFrameMove );

	DXUTInit( true, true, true );			// Parse the command line, handle the default hotkeys, and show msgboxes
	DXUTSetCursorSettings( true, true );	// Show the cursor and clip it when in full screen
	DXUTSetWindow( pInitializeParam->hWndFocus, pInitializeParam->hWndDeviceFullscreen, pInitializeParam->hWndDeviceWindowed, true );

	g_D3D9SearchSetting.dwGameID		= pInitializeParam->dwGameID;
	g_D3D9SearchSetting.dwAdID			= pInitializeParam->dwAdID;
	//g_pfnSearchStateCallback	= pInitializeParam->pfnStatCallback;

	// TODO: Perform any application-level initialization here
	g_SampleUI.Init( &g_DialogResourceManager );

	//Ϊg_SampleUI�Ի���������Ϣ������
	g_SampleUI.SetCallback( OnGUIEvent );

	//	���������ļ�����GUI����Ҫ��Ϣ
	LoadGUIDefaultFromConfig();

	//Ϊg_SampleUI�Ի�����������
	g_SampleUI.SetFont( 1, g_D3D9SearchSetting.szFontName1, g_D3D9SearchSetting.nFontSize1, FW_NORMAL );

	int iyPox =  g_pShowPic->GetiHeight() / 2 - g_D3D9SearchSetting.iSearchBoxHeight / 2;
	g_rectIMEEdit.left	= g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() +2;
	g_rectIMEEdit.top	= g_pShowPic->GetiyPos() + iyPox;
	g_rectIMEEdit.right = g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2 + g_D3D9SearchSetting.iSearchBoxWidth;
	g_rectIMEEdit.bottom= g_pShowPic->GetiyPos() + iyPox + g_D3D9SearchSetting.iSearchBoxHeight; 

	g_rectBtnSearch.left	= g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2 + g_D3D9SearchSetting.iSearchBoxWidth ;
	g_rectBtnSearch.top		= g_pShowPic->GetiyPos();
	g_rectBtnSearch.right	= g_pShowPic->GetixPos() + 2 + g_pShowPic->GetiWidth() + g_D3D9SearchSetting.iSearchBoxWidth + g_D3D9SearchSetting.iSearchBtnWidth;
	g_rectBtnSearch.bottom	= g_pShowPic->GetiyPos() + g_D3D9SearchSetting.iSearchBtnHeight; 

	//���IME�༭��ؼ�
	
	CDXUTIMEEditBox *pIMEEdit;
	if( SUCCEEDED( g_SampleUI.AddIMEEditBox( IDC_IME_EDITBOX, g_wszDefaultText, g_rectIMEEdit.left, g_rectIMEEdit.top, g_D3D9SearchSetting.iSearchBoxWidth, g_D3D9SearchSetting.iSearchBoxHeight, false, &pIMEEdit ) ) )
	{
		pIMEEdit->GetElement( 0 )->iFont = 1;
		pIMEEdit->GetElement( 1 )->iFont = 1;
		pIMEEdit->GetElement( 9 )->iFont = 1;
		pIMEEdit->GetElement( 0 )->TextureColor.Init( g_D3D9SearchSetting.clrTexture );  // Transparent center�� ������ɫ
		pIMEEdit->SetBorderWidth( 3 );
		pIMEEdit->SetTextColor( g_D3D9SearchSetting.clrText );
		pIMEEdit->SetCaretColor( g_D3D9SearchSetting.clrCaret );
		pIMEEdit->SetSelectedTextColor( g_D3D9SearchSetting.clrTextSelected );
		pIMEEdit->SetSelectedBackColor( g_D3D9SearchSetting.clrBackSelected );
		pIMEEdit->SetVisible(true);
	}

	//WCHAR szBtnSearch[50];
	//MultiByteToWideChar (CP_ACP, 0, g_szB_text, -1, szBtnSearch, sizeof(szBtnSearch)/sizeof(szBtnSearch[0]));  
	g_SampleUI.AddButton( IDC_BUTTON_SEARCH, g_wszB_text, g_rectBtnSearch.left, g_rectBtnSearch.top,g_D3D9SearchSetting.iSearchBtnWidth , g_D3D9SearchSetting.iSearchBtnHeight);
	g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetVisible(true);
	
	//if( g_nSearchEngine == SEARCHENGINE_BAIDU )
	//{
	//	g_SampleUI.AddButton( IDC_BUTTON_CLOSE,	L"�ٶ�...", g_rectBtnShowSearch.left, g_rectBtnShowSearch.top, g_rectBtnShowSearch.right - g_rectBtnShowSearch.left , g_rectBtnShowSearch.bottom - g_rectBtnShowSearch.top );
	//}
	//else if( g_nSearchEngine == SEARCHENGINE_GOOGLE )
	//{
	//	g_SampleUI.AddButton( IDC_BUTTON_CLOSE,	L"google...", g_rectBtnShowSearch.left, g_rectBtnShowSearch.top, g_rectBtnShowSearch.right - g_rectBtnShowSearch.left , g_rectBtnShowSearch.bottom - g_rectBtnShowSearch.top );
	//}

	// Initialize DXUT and create the desired Win32 window and Direct3D device for the application
	
	//////////////////////////////////////////////////////////////////////////
	//�ӹ���Ϣ���� 
	/*g_HookerMsg.SetWndHook( g_hGameWnd );
	if( g_HookerMsg.GetHook() == NULL)
	{
		HHOOK hHook	= SetWindowsHookExW(WH_GETMESSAGE, HookGetMsg, NULL, ::GetCurrentThreadId());
		if(hHook != NULL)
		{
			g_HookerMsg.SetHook( hHook );
		}
		else
		{
			DXUT_ERR(L"SetWindowsHookEx", ::GetLastError());
		}
	}*/
	//SHOWOUTPUTDEBUG(End_GUISDK_InitEx);

	//PostMessage(g_hGameWnd, WM_INPUTLANGCHANGE,0, NULL );

	return	DXUTSetDevice(pDevice);
}



void WINAPI D3D9SearchRender()
{
	DXUTRender3DEnvironment();
	//SHOWOUTPUTDEBUG( GUISDK_Render );
}

void WINAPI D3D9SearchDestory()
{
	if ( g_pFunOldMsgProc != NULL)
	{
		if(IsWindowUnicode(g_hGameWnd) )
		{
			SetWindowLongW( g_hGameWnd, GWL_WNDPROC, (LONG)g_pFunOldMsgProc );
		}
		else
		{
			SetWindowLongA( g_hGameWnd, GWL_WNDPROC, (LONG)g_pFunOldMsgProc );
		}
	}
	
	g_pFunOldMsgProc = NULL;

	if ( g_pShowPic != NULL )
	{
		delete g_pShowPic;
		g_pShowPic = NULL; 
	}
	DXUTShutdown(0);
}
