
#pragma once 
#ifndef DXUT_H
#define DXUT_H

#include "DXUTDefine.h"

extern CRITICAL_SECTION g_cs;  
extern bool				g_bThreadSafe;

//--------------------------------------------------------------------------------------
// Helper macros to build member functions that access member variables with thread safety
//--------------------------------------------------------------------------------------
#define SET_ACCESSOR( x, y )       inline void Set##y( x t )  { DXUTLock l; m_state.m_##y = t; };
#define GET_ACCESSOR( x, y )       inline x Get##y() { DXUTLock l; return m_state.m_##y; };
#define GET_SET_ACCESSOR( x, y )   SET_ACCESSOR( x, y ) GET_ACCESSOR( x, y )

#define SETP_ACCESSOR( x, y )      inline void Set##y( x* t )  { DXUTLock l; m_state.m_##y = *t; };
#define GETP_ACCESSOR( x, y )      inline x* Get##y() { DXUTLock l; return &m_state.m_##y; };
#define GETP_SETP_ACCESSOR( x, y ) SETP_ACCESSOR( x, y ) GETP_ACCESSOR( x, y )

//--------------------------------------------------------------------------------------
// Automatically enters & leaves the CS upon object creation/deletion
//--------------------------------------------------------------------------------------
class DXUTLock
{
public:
	inline DXUTLock()  { if( g_bThreadSafe ) EnterCriticalSection( &g_cs ); }
	inline ~DXUTLock() { if( g_bThreadSafe ) LeaveCriticalSection( &g_cs ); }
};


struct DXUTDeviceSettings
{
	UINT AdapterOrdinal;
	D3DDEVTYPE DeviceType;
	D3DFORMAT AdapterFormat;
	DWORD BehaviorFlags;
	D3DPRESENT_PARAMETERS pp;
};

typedef bool    (CALLBACK *LPDXUTCALLBACKISDEVICEACCEPTABLE)( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
typedef bool    (CALLBACK *LPDXUTCALLBACKMODIFYDEVICESETTINGS)( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
typedef HRESULT (CALLBACK *LPDXUTCALLBACKDEVICECREATED)( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
typedef HRESULT (CALLBACK *LPDXUTCALLBACKDEVICERESET)( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKDEVICEDESTROYED)( void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKDEVICELOST)( void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKFRAMEMOVE)( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKFRAMERENDER)( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKKEYBOARD)( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKMOUSE)( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );
typedef LRESULT (CALLBACK *LPDXUTCALLBACKMSGPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
typedef void    (CALLBACK *LPDXUTCALLBACKTIMER)( UINT idEvent, void* pUserContext );

//--------------------------------------------------------------------------------------
// Stores timer callback info
//--------------------------------------------------------------------------------------
struct DXUT_TIMER
{
	LPDXUTCALLBACKTIMER pCallbackTimer;
	void* pCallbackUserContext;
	float fTimeoutInSecs;
	float fCountdown;
	bool  bEnabled;
	UINT  nID;
};


//--------------------------------------------------------------------------------------
// Stores DXUT state and data access is done with thread safety (if g_bThreadSafe==true)
//--------------------------------------------------------------------------------------
class DXUTState
{
protected:
	struct STATE
	{
		IDirect3D9*          m_D3D;                     // the main D3D object
		IDirect3DDevice9*    m_D3DDevice;               // the D3D rendering device
		//CD3DEnumeration*     m_D3DEnumeration;          // CD3DEnumeration object

		DXUTDeviceSettings*  m_CurrentDeviceSettings;   // current device settings
		D3DSURFACE_DESC      m_BackBufferSurfaceDesc;   // back buffer surface description
		D3DCAPS9             m_Caps;                    // D3D caps for current device

		HWND  m_HWNDFocus;                  // the main app focus window
		HWND  m_HWNDDeviceFullScreen;       // the main app device window in fullscreen mode
		HWND  m_HWNDDeviceWindowed;         // the main app device window in windowed mode
		HMONITOR m_AdapterMonitor;          // the monitor of the adapter 
		HMENU m_Menu;                       // handle to menu

		UINT m_FullScreenBackBufferWidthAtModeChange;  // back buffer size of fullscreen mode right before switching to windowed mode.  Used to restore to same resolution when toggling back to fullscreen
		UINT m_FullScreenBackBufferHeightAtModeChange; // back buffer size of fullscreen mode right before switching to windowed mode.  Used to restore to same resolution when toggling back to fullscreen
		UINT m_WindowBackBufferWidthAtModeChange;  // back buffer size of windowed mode right before switching to fullscreen mode.  Used to restore to same resolution when toggling back to windowed mode
		UINT m_WindowBackBufferHeightAtModeChange; // back buffer size of windowed mode right before switching to fullscreen mode.  Used to restore to same resolution when toggling back to windowed mode
		DWORD m_WindowedStyleAtModeChange;  // window style
		WINDOWPLACEMENT m_WindowedPlacement; // record of windowed HWND position/show state/etc
		bool  m_TopmostWhileWindowed;       // if true, the windowed HWND is topmost 
		bool  m_Minimized;                  // if true, the HWND is minimized
		bool  m_Maximized;                  // if true, the HWND is maximized
		bool  m_MinimizedWhileFullscreen;   // if true, the HWND is minimized due to a focus switch away when fullscreen mode
		bool  m_IgnoreSizeChange;           // if true, DXUT won't reset the device upon HWND size change

		double m_Time;                      // current time in seconds
		double m_AbsoluteTime;              // absolute time in seconds
		float m_ElapsedTime;                // time elapsed since last frame

		HINSTANCE m_HInstance;              // handle to the app instance
		double m_LastStatsUpdateTime;       // last time the stats were updated
		DWORD m_LastStatsUpdateFrames;      // frames count since last time the stats were updated
		float m_FPS;                        // frames per second
		int   m_CurrentFrameNumber;         // the current frame number
		HHOOK m_KeyboardHook;               // handle to keyboard hook
		bool  m_AllowShortcutKeysWhenFullscreen; // if true, when fullscreen enable shortcut keys (Windows keys, StickyKeys shortcut, ToggleKeys shortcut, FilterKeys shortcut) 
		bool  m_AllowShortcutKeysWhenWindowed;   // if true, when windowed enable shortcut keys (Windows keys, StickyKeys shortcut, ToggleKeys shortcut, FilterKeys shortcut) 
		bool  m_AllowShortcutKeys;          // if true, then shortcut keys are currently disabled (Windows key, etc)
		bool  m_CallDefWindowProc;          // if true, DXUTStaticWndProc will call DefWindowProc for unhandled messages. Applications rendering to a dialog may need to set this to false.
		STICKYKEYS m_StartupStickyKeys;     // StickyKey settings upon startup so they can be restored later
		TOGGLEKEYS m_StartupToggleKeys;     // ToggleKey settings upon startup so they can be restored later
		FILTERKEYS m_StartupFilterKeys;     // FilterKey settings upon startup so they can be restored later

		bool  m_HandleDefaultHotkeys;       // if true, then DXUT will handle some default hotkeys
		bool  m_HandleAltEnter;             // if true, then DXUT will handle Alt-Enter
		bool  m_ShowMsgBoxOnError;          // if true, then msgboxes are displayed upon errors
		bool  m_NoStats;                    // if true, then DXUTGetFrameStats() and DXUTGetDeviceStats() will return blank strings
		bool  m_ClipCursorWhenFullScreen;   // if true, then DXUT will keep the cursor from going outside the window when full screen
		bool  m_ShowCursorWhenFullScreen;   // if true, then DXUT will show a cursor when full screen
		bool  m_ConstantFrameTime;          // if true, then elapsed frame time will always be 0.05f seconds which is good for debugging or automated capture
		float m_TimePerFrame;               // the constant time per frame in seconds, only valid if m_ConstantFrameTime==true
		bool  m_WireframeMode;              // if true, then D3DRS_FILLMODE==D3DFILL_WIREFRAME else D3DRS_FILLMODE==D3DFILL_SOLID 
		bool  m_AutoChangeAdapter;          // if true, then the adapter will automatically change if the window is different monitor
		bool  m_WindowCreatedWithDefaultPositions; // if true, then CW_USEDEFAULT was used and the window should be moved to the right adapter
		int   m_ExitCode;                   // the exit code to be returned to the command line

		bool  m_DXUTInited;                 // if true, then DXUTInit() has succeeded
		bool  m_WindowCreated;              // if true, then DXUTCreateWindow() or DXUTSetWindow() has succeeded
		bool  m_DeviceCreated;              // if true, then DXUTCreateDevice*() or DXUTSetDevice() has succeeded

		bool  m_DXUTInitCalled;             // if true, then DXUTInit() was called
		bool  m_WindowCreateCalled;         // if true, then DXUTCreateWindow() or DXUTSetWindow() was called
		bool  m_DeviceCreateCalled;         // if true, then DXUTCreateDevice*() or DXUTSetDevice() was called

		bool  m_DeviceObjectsCreated;       // if true, then DeviceCreated callback has been called (if non-NULL)
		bool  m_DeviceObjectsReset;         // if true, then DeviceReset callback has been called (if non-NULL)
		bool  m_InsideDeviceCallback;       // if true, then the framework is inside an app device callback
		bool  m_InsideMainloop;             // if true, then the framework is inside the main loop
		bool  m_Active;                     // if true, then the app is the active top level window
		bool  m_TimePaused;                 // if true, then time is paused
		bool  m_RenderingPaused;            // if true, then rendering is paused
		int   m_PauseRenderingCount;        // pause rendering ref count
		int   m_PauseTimeCount;             // pause time ref count
		bool  m_DeviceLost;                 // if true, then the device is lost and needs to be reset
		bool  m_NotifyOnMouseMove;          // if true, include WM_MOUSEMOVE in mousecallback
		bool  m_Automation;                 // if true, automation is enabled
		bool  m_InSizeMove;                 // if true, app is inside a WM_ENTERSIZEMOVE
		UINT  m_TimerLastID;               // last ID of the DXUT timer

		int   m_OverrideAdapterOrdinal;     // if != -1, then override to use this adapter ordinal
		bool  m_OverrideWindowed;           // if true, then force to start windowed
		bool  m_OverrideFullScreen;         // if true, then force to start full screen
		int   m_OverrideStartX;             // if != -1, then override to this X position of the window
		int   m_OverrideStartY;             // if != -1, then override to this Y position of the window
		int   m_OverrideWidth;              // if != 0, then override to this width
		int   m_OverrideHeight;             // if != 0, then override to this height
		bool  m_OverrideForceHAL;           // if true, then force to HAL device (failing if one doesn't exist)
		bool  m_OverrideForceREF;           // if true, then force to REF device (failing if one doesn't exist)
		bool  m_OverrideForcePureHWVP;      // if true, then force to use pure HWVP (failing if device doesn't support it)
		bool  m_OverrideForceHWVP;          // if true, then force to use HWVP (failing if device doesn't support it)
		bool  m_OverrideForceSWVP;          // if true, then force to use SWVP 
		bool  m_OverrideConstantFrameTime;  // if true, then force to constant frame time
		float m_OverrideConstantTimePerFrame; // the constant time per frame in seconds if m_OverrideConstantFrameTime==true
		int   m_OverrideQuitAfterFrame;     // if != 0, then it will force the app to quit after that frame
		int   m_OverrideForceVsync;         // if == 0, then it will force the app to use D3DPRESENT_INTERVAL_IMMEDIATE, if == 1 force use of D3DPRESENT_INTERVAL_DEFAULT
		bool  m_OverrideRelaunchMCE;          // if true, then force relaunch of MCE at exit

		LPDXUTCALLBACKISDEVICEACCEPTABLE    m_IsDeviceAcceptableFunc;   // is device acceptable callback
		LPDXUTCALLBACKMODIFYDEVICESETTINGS  m_ModifyDeviceSettingsFunc; // modify device settings callback
		LPDXUTCALLBACKDEVICECREATED         m_DeviceCreatedFunc;        // device created callback
		LPDXUTCALLBACKDEVICERESET           m_DeviceResetFunc;          // device reset callback
		LPDXUTCALLBACKDEVICELOST            m_DeviceLostFunc;           // device lost callback
		LPDXUTCALLBACKDEVICEDESTROYED       m_DeviceDestroyedFunc;      // device destroyed callback
		LPDXUTCALLBACKFRAMEMOVE             m_FrameMoveFunc;            // frame move callback
		LPDXUTCALLBACKFRAMERENDER           m_FrameRenderFunc;          // frame render callback
		LPDXUTCALLBACKKEYBOARD              m_KeyboardFunc;             // keyboard callback
		LPDXUTCALLBACKMOUSE                 m_MouseFunc;                // mouse callback
		LPDXUTCALLBACKMSGPROC               m_WindowMsgFunc;            // window messages callback

		void*                               m_IsDeviceAcceptableFuncUserContext;   // user context for is device acceptable callback
		void*                               m_ModifyDeviceSettingsFuncUserContext; // user context for modify device settings callback
		void*                               m_DeviceCreatedUserContext;            // user context for device created callback
		void*                               m_DeviceCreatedFuncUserContext;        // user context for device created callback
		void*                               m_DeviceResetFuncUserContext;          // user context for device reset callback
		void*                               m_DeviceLostFuncUserContext;           // user context for device lost callback
		void*                               m_DeviceDestroyedFuncUserContext;      // user context for device destroyed callback
		void*                               m_FrameMoveFuncUserContext;            // user context for frame move callback
		void*                               m_FrameRenderFuncUserContext;          // user context for frame render callback
		void*                               m_KeyboardFuncUserContext;             // user context for keyboard callback
		void*                               m_MouseFuncUserContext;                // user context for mouse callback
		void*                               m_WindowMsgFuncUserContext;            // user context for window messages callback

		bool                         m_Keys[256];                       // array of key state
		bool                         m_MouseButtons[5];                 // array of mouse states

		CGrowableArray<DXUT_TIMER>*  m_TimerList;                       // list of DXUT_TIMER structs
		WCHAR                        m_StaticFrameStats[256];           // static part of frames stats 
		WCHAR                        m_FPSStats[64];                    // fps stats
		WCHAR                        m_FrameStats[256];                 // frame stats (fps, width, etc)
		WCHAR                        m_DeviceStats[256];                // device stats (description, device type, etc)
		WCHAR                        m_WindowTitle[256];                // window title
	};

	STATE m_state;

public:
	DXUTState()  { Create(); }
	~DXUTState() { Destroy(); }

	void Create()
	{
		// Make sure these are created before DXUTState so they 
		// destroyed last because DXUTState cleanup needs them
		//g_CDXUTResourceCache;

		ZeroMemory( &m_state, sizeof(STATE) ); 
		g_bThreadSafe = true; 
		InitializeCriticalSection( &g_cs ); 
		m_state.m_OverrideStartX = -1; 
		m_state.m_OverrideStartY = -1; 
		m_state.m_OverrideAdapterOrdinal = -1; 
		m_state.m_OverrideForceVsync = -1;
		m_state.m_AutoChangeAdapter = true; 
		m_state.m_ShowMsgBoxOnError = true;
		m_state.m_AllowShortcutKeysWhenWindowed = true;
		m_state.m_Active = true;
		m_state.m_CallDefWindowProc = true;
	}

	void Destroy()
	{
		//DXUTShutdown();
		DeleteCriticalSection( &g_cs ); 
	}

	// Macros to define access functions for thread safe access into m_state 
	GET_SET_ACCESSOR( IDirect3D9*, D3D );

	GET_SET_ACCESSOR( IDirect3DDevice9*, D3DDevice );
	//GET_SET_ACCESSOR( CD3DEnumeration*, D3DEnumeration );   
	GET_SET_ACCESSOR( DXUTDeviceSettings*, CurrentDeviceSettings );   
	GETP_SETP_ACCESSOR( D3DSURFACE_DESC, BackBufferSurfaceDesc );
	GETP_SETP_ACCESSOR( D3DCAPS9, Caps );

	GET_SET_ACCESSOR( HWND, HWNDFocus );
	GET_SET_ACCESSOR( HWND, HWNDDeviceFullScreen );
	GET_SET_ACCESSOR( HWND, HWNDDeviceWindowed );
	GET_SET_ACCESSOR( HMONITOR, AdapterMonitor );
	GET_SET_ACCESSOR( HMENU, Menu );   

	GET_SET_ACCESSOR( UINT, FullScreenBackBufferWidthAtModeChange );
	GET_SET_ACCESSOR( UINT, FullScreenBackBufferHeightAtModeChange );
	GET_SET_ACCESSOR( UINT, WindowBackBufferWidthAtModeChange );
	GET_SET_ACCESSOR( UINT, WindowBackBufferHeightAtModeChange );
	GETP_SETP_ACCESSOR( WINDOWPLACEMENT, WindowedPlacement );
	GET_SET_ACCESSOR( DWORD, WindowedStyleAtModeChange );
	GET_SET_ACCESSOR( bool, TopmostWhileWindowed );
	GET_SET_ACCESSOR( bool, Minimized );
	GET_SET_ACCESSOR( bool, Maximized );
	GET_SET_ACCESSOR( bool, MinimizedWhileFullscreen );
	GET_SET_ACCESSOR( bool, IgnoreSizeChange );   

	GET_SET_ACCESSOR( double, Time );
	GET_SET_ACCESSOR( double, AbsoluteTime );
	GET_SET_ACCESSOR( float, ElapsedTime );

	GET_SET_ACCESSOR( HINSTANCE, HInstance );
	GET_SET_ACCESSOR( double, LastStatsUpdateTime );   
	GET_SET_ACCESSOR( DWORD, LastStatsUpdateFrames );   
	GET_SET_ACCESSOR( float, FPS );    
	GET_SET_ACCESSOR( int, CurrentFrameNumber );
	GET_SET_ACCESSOR( HHOOK, KeyboardHook );
	GET_SET_ACCESSOR( bool, AllowShortcutKeysWhenFullscreen );
	GET_SET_ACCESSOR( bool, AllowShortcutKeysWhenWindowed );
	GET_SET_ACCESSOR( bool, AllowShortcutKeys );
	GET_SET_ACCESSOR( bool, CallDefWindowProc );
	GET_SET_ACCESSOR( STICKYKEYS, StartupStickyKeys );
	GET_SET_ACCESSOR( TOGGLEKEYS, StartupToggleKeys );
	GET_SET_ACCESSOR( FILTERKEYS, StartupFilterKeys );

	GET_SET_ACCESSOR( bool, HandleDefaultHotkeys );
	GET_SET_ACCESSOR( bool, HandleAltEnter );
	GET_SET_ACCESSOR( bool, ShowMsgBoxOnError );
	GET_SET_ACCESSOR( bool, NoStats );
	GET_SET_ACCESSOR( bool, ClipCursorWhenFullScreen );   
	GET_SET_ACCESSOR( bool, ShowCursorWhenFullScreen );
	GET_SET_ACCESSOR( bool, ConstantFrameTime );
	GET_SET_ACCESSOR( float, TimePerFrame );
	GET_SET_ACCESSOR( bool, WireframeMode );   
	GET_SET_ACCESSOR( bool, AutoChangeAdapter );
	GET_SET_ACCESSOR( bool, WindowCreatedWithDefaultPositions );
	GET_SET_ACCESSOR( int, ExitCode );

	GET_SET_ACCESSOR( bool, DXUTInited );
	GET_SET_ACCESSOR( bool, WindowCreated );
	GET_SET_ACCESSOR( bool, DeviceCreated );
	GET_SET_ACCESSOR( bool, DXUTInitCalled );
	GET_SET_ACCESSOR( bool, WindowCreateCalled );
	GET_SET_ACCESSOR( bool, DeviceCreateCalled );
	GET_SET_ACCESSOR( bool, InsideDeviceCallback );
	GET_SET_ACCESSOR( bool, InsideMainloop );
	GET_SET_ACCESSOR( bool, DeviceObjectsCreated );
	GET_SET_ACCESSOR( bool, DeviceObjectsReset );
	GET_SET_ACCESSOR( bool, Active );
	GET_SET_ACCESSOR( bool, RenderingPaused );
	GET_SET_ACCESSOR( bool, TimePaused );
	GET_SET_ACCESSOR( int, PauseRenderingCount );
	GET_SET_ACCESSOR( int, PauseTimeCount );
	GET_SET_ACCESSOR( bool, DeviceLost );
	GET_SET_ACCESSOR( bool, NotifyOnMouseMove );
	GET_SET_ACCESSOR( bool, Automation );
	GET_SET_ACCESSOR( bool, InSizeMove );
	GET_SET_ACCESSOR( UINT, TimerLastID );

	GET_SET_ACCESSOR( int, OverrideAdapterOrdinal );
	GET_SET_ACCESSOR( bool, OverrideWindowed );
	GET_SET_ACCESSOR( bool, OverrideFullScreen );
	GET_SET_ACCESSOR( int, OverrideStartX );
	GET_SET_ACCESSOR( int, OverrideStartY );
	GET_SET_ACCESSOR( int, OverrideWidth );
	GET_SET_ACCESSOR( int, OverrideHeight );
	GET_SET_ACCESSOR( bool, OverrideForceHAL );
	GET_SET_ACCESSOR( bool, OverrideForceREF );
	GET_SET_ACCESSOR( bool, OverrideForcePureHWVP );
	GET_SET_ACCESSOR( bool, OverrideForceHWVP );
	GET_SET_ACCESSOR( bool, OverrideForceSWVP );
	GET_SET_ACCESSOR( bool, OverrideConstantFrameTime );
	GET_SET_ACCESSOR( float, OverrideConstantTimePerFrame );
	GET_SET_ACCESSOR( int, OverrideQuitAfterFrame );
	GET_SET_ACCESSOR( int, OverrideForceVsync );
	GET_SET_ACCESSOR( bool, OverrideRelaunchMCE );

	GET_SET_ACCESSOR( LPDXUTCALLBACKISDEVICEACCEPTABLE, IsDeviceAcceptableFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKMODIFYDEVICESETTINGS, ModifyDeviceSettingsFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKDEVICECREATED, DeviceCreatedFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKDEVICERESET, DeviceResetFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKDEVICELOST, DeviceLostFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKDEVICEDESTROYED, DeviceDestroyedFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKFRAMEMOVE, FrameMoveFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKFRAMERENDER, FrameRenderFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKKEYBOARD, KeyboardFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKMOUSE, MouseFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKMSGPROC, WindowMsgFunc );

	GET_SET_ACCESSOR( void*, IsDeviceAcceptableFuncUserContext );
	GET_SET_ACCESSOR( void*, ModifyDeviceSettingsFuncUserContext );
	GET_SET_ACCESSOR( void*, DeviceCreatedFuncUserContext );
	GET_SET_ACCESSOR( void*, DeviceResetFuncUserContext );
	GET_SET_ACCESSOR( void*, DeviceLostFuncUserContext );
	GET_SET_ACCESSOR( void*, DeviceDestroyedFuncUserContext );
	GET_SET_ACCESSOR( void*, FrameMoveFuncUserContext );
	GET_SET_ACCESSOR( void*, FrameRenderFuncUserContext );
	GET_SET_ACCESSOR( void*, KeyboardFuncUserContext );
	GET_SET_ACCESSOR( void*, MouseFuncUserContext );
	GET_SET_ACCESSOR( void*, WindowMsgFuncUserContext );

	GET_SET_ACCESSOR( CGrowableArray<DXUT_TIMER>*, TimerList );   
	GET_ACCESSOR( bool*, Keys );
	GET_ACCESSOR( bool*, MouseButtons );
	GET_ACCESSOR( WCHAR*, StaticFrameStats );
	GET_ACCESSOR( WCHAR*, FPSStats );
	GET_ACCESSOR( WCHAR*, FrameStats );
	GET_ACCESSOR( WCHAR*, DeviceStats );    
	GET_ACCESSOR( WCHAR*, WindowTitle );
};

extern DXUTState g_DXUTState;


class CDXUTTimer
{
public:
	CDXUTTimer();

	void Reset(); // resets the timer
	void Start(); // starts the timer
	void Stop();  // stop (or pause) the timer
	void Advance(); // advance the timer by 0.1 seconds
	double GetAbsoluteTime(); // get the absolute system time
	double GetTime(); // get the current time
	double GetElapsedTime(); // get the time that elapsed between Get*ElapsedTime() calls
	void GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime ); // get all time values at once
	bool IsStopped(); // returns true if timer stopped

	// Limit the current thread to one processor (the current one). This ensures that timing code runs
	// on only one processor, and will not suffer any ill effects from power management.
	void LimitThreadAffinityToCurrentProc();

protected:
	LARGE_INTEGER GetAdjustedCurrentTime();

	bool m_bUsingQPF;
	bool m_bTimerStopped;
	LONGLONG m_llQPFTicksPerSec;

	LONGLONG m_llStopTime;
	LONGLONG m_llLastElapsedTime;
	LONGLONG m_llBaseTime;
};

//CDXUTTimer* DXUTGetGlobalTimer();

extern CDXUTTimer g_CDXUTTimer;

#endif