

#pragma  once

#define IDC_LABEL_TIPS	         5    //��ǩ�ؼ�, ��ʾ�û�����˵��
#define IDC_BUTTON_ENABLE_IME   18   //	����IME��ť
#define IDC_BUTTON_DISABLE_IME	19   //	����IME��ť
#define IDC_IME_EDITBOX	        21   //	IME�༭��
#define	IDC_BUTTON_SEARCH		22	//	ȷ��
#define	IDC_BUTTON_CLOSE		23	//	�رհ�ť


typedef int		(WINAPI *LPFN_SEARCHSTATCALLBACK)(DWORD dwGameID, DWORD dwAdID, WPARAM wParam, LPARAM lParam);
typedef LRESULT	(CALLBACK *LPWNDPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ���弸����������          
typedef enum eSearchEngine
{
	SEARCHENGINE_BAIDU,
	SEARCHENGINE_GOOGLE,
	SEARCHENGINE_SOUGOO,
	SEARCHENGINE_YAHOO,
	SEARCHENGINE_OTHERS,
}ESearchEngine;


//ESearchEngine����

#define	IDS_MAIN_DEFAULT_CONFIG	"SEARCH.INI"
#define	IDS_DEFAULT_SEARCHTIPSW	L"��������������"
#define	IDS_DEFAULT_SEARCH_TEXT	L"����"
#define	IDS_BAIDU_URLA		"http://www.baidu.com/s?wd="
#define	IDS_GOOGLE_URLA		"http://www.google.com//search?hl=zh-CN&q="


#define IDC_LABEL_TIPS	         5    //	��ǩ�ؼ�, ��ʾ�û�����˵��
#define IDC_BUTTON_ENABLE_IME   18   //	����IME��ť
#define IDC_BUTTON_DISABLE_IME	19   //	����IME��ť
#define IDC_IME_EDITBOX	        21   //	IME�༭��
#define	IDC_BUTTON_SEARCH		22	//	ȷ��
#define	IDC_BUTTON_CLOSE		23	//	�رհ�ť



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



typedef struct tagXGUIInitializeParam
{
	union
	{
		DWORD	dwAdID;
		DWORD	dwEventID;
	};

	DWORD	dwGameID;
	int		nSearchType;
	union
	{
		LPFN_SEARCHSTATCALLBACK	pfnStatCallback;
		FARPROC pfnStat;	//	ͳ�Ƶ�������Ļص�����
	};

	HWND	hWndFocus;
	HWND	hWndDeviceFullscreen;
	HWND	hWndDeviceWindowed;


	WPARAM	wParam;
	LPARAM	lParam;

}XGUI_INITIALIZEPARAM, *LPXGUI_INITIALIZEPARAM;


/************************************************************************/
/* GUI������Ϣ                                                          */
/************************************************************************/
typedef	struct tagGUISettings
{
	DWORD		dwSearchType;
	DWORD		dwGameID;
	DWORD		dwAdID;

	WCHAR		szFontName1[256];
	WCHAR		szFontName2[256];
	UINT		nFontSize1;
	UINT		nFontSize2;
	RECT		rectSearchEdit;
	D3DCOLOR	clrTexture;

	D3DCOLOR	clrText;
	D3DCOLOR	clrCaret;
	D3DCOLOR	clrTextSelected;
	D3DCOLOR	clrBackSelected;
	RECT		rectSearchButton;
	RECT		rectShowSearchButton;

	WPARAM		wKey;

	tagGUISettings()
	{
		ZeroMemory(this, sizeof(*this));
	}
	static tagGUISettings	*Instance()
	{
		static tagGUISettings	theSettings;
		return	&theSettings;
	}
}GUISETTINGS, *LPGUISETTINGS;


