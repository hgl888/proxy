

#pragma  once

#include "DXUT.h"
#include "DXUTgui.h"
#include "WinInet.h"


//typedef int		(WINAPI *LPFN_SEARCHSTATCALLBACK)(DWORD dwGameID, DWORD dwAdID, WPARAM wParam, LPARAM lParam);
typedef LRESULT	(CALLBACK *LPWNDPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


extern DWORD			g_dwAdID;
//ESearchEngine����

#define IDC_LABEL_TIPS	         5   // ��ǩ�ؼ�, ��ʾ�û�����˵��
#define IDC_BUTTON_ENABLE_IME   18   // ����IME��ť
#define IDC_BUTTON_DISABLE_IME	19   // ����IME��ť
#define IDC_IME_EDITBOX	        21   // IME�༭��
#define	IDC_BUTTON_SEARCH		22	 // ������ť
#define	IDC_BUTTON_CLOSE		23	 // �رհ�ť



bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

LRESULT CALLBACK MsgProcProxy( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

BOOL HandleSearchButtonMsg();

typedef VOID(WINAPI *LPCallBack3DFocus)(DWORD,LPVOID);
extern LPCallBack3DFocus g_pFun3DFocus; //���ͳ�ƴ����ĺ���ָ��

typedef struct tagD3DSearchParam
{
	union
	{
		DWORD	dwAdID;
		DWORD	dwEventID;
	};

	DWORD	dwGameID;
	int		nSearchType;

	LPCallBack3DFocus pFun3DFocus;	//	ͳ�Ƶ�������Ļص�����
	HWND	hWndFocus;
	HWND	hWndDeviceFullscreen;
	HWND	hWndDeviceWindowed;

	WPARAM	wParam;
	LPARAM	lParam;

}D3DSEARCH_PARAM, *LPD3DSEARCH_PARAM;


typedef struct tagThreadParam
{
	HWND	hWndToMonitor;
	char	szURL[INTERNET_MAX_URL_LENGTH];
	WPARAM	wParam;
	LPARAM	lParam;
}THREADPARAM, *LPTHREADPARAM;

/************************************************************************/
/* GUI������Ϣ                                                          */
/************************************************************************/
typedef	struct tagD3D9SearchSettings
{
	DWORD		dwSearchType;
	DWORD		dwGameID;
	DWORD		dwAdID;

	WCHAR		szFontName1[256];
	UINT		nFontSize1;
	RECT		rectSearchEdit;
	D3DCOLOR	clrTexture;
	D3DCOLOR	clrText;
	D3DCOLOR	clrCaret;
	D3DCOLOR	clrTextSelected;
	D3DCOLOR	clrBackSelected;
	RECT		rectSearchButton;
	RECT		rectShowSearchButton;

	int iSearchBoxWidth;
	int iSearchBoxHeight;
	int iSearchBtnWidth;
	int iSearchBtnHeight;

	WPARAM		wKey;

	tagD3D9SearchSettings()
	{
		ZeroMemory(this, sizeof(*this));
	}

}D3D9SEARCHSETTINGS, *LPD3D9SEARCHSETTINGS;


