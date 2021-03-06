#include "stdafx.h"
#include "IGA_IDirectDrawSurface.h"


extern IGA_IDirectDrawSurface7		*g_pIDirectDrawSurface7;

IGA_IDirectDrawSurface::IGA_IDirectDrawSurface(IDirectDrawSurface*   pIDirectDrawSurface)
	:m_pIDirectDrawSurface( pIDirectDrawSurface )
{
}

IGA_IDirectDrawSurface::~IGA_IDirectDrawSurface(void)
{
}

HRESULT IGA_IDirectDrawSurface::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	HRESULT hr = m_pIDirectDrawSurface->QueryInterface (riid,  ppvObj);       
	if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface  )) 
	{		  
		*ppvObj = this;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface2 )) 
	{
		IGA_IDirectDrawSurface2* pIDirectSurface2 = new IGA_IDirectDrawSurface2((IGA_IDirectDrawSurface2*) *ppvObj );
		*ppvObj = pIDirectSurface2;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface3)) 
	{
		IGA_IDirectDrawSurface3*   pIDirectSurface3 = new IGA_IDirectDrawSurface3((IGA_IDirectDrawSurface3*)*ppvObj);
		*ppvObj = pIDirectSurface3;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface4)) 
	{
		IGA_IDirectDrawSurface4*   pIDirectSurface4 = new IGA_IDirectDrawSurface4((IGA_IDirectDrawSurface4*)*ppvObj);
		*ppvObj  = pIDirectSurface4;
	}
	else if ( ::IsEqualIID( riid, Proxy_IDirectDrawSurface7))
	{
		IGA_IDirectDrawSurface7* pIDirectSurface7 = new IGA_IDirectDrawSurface7((IGA_IDirectDrawSurface7 *) *ppvObj);
		*ppvObj = pIDirectSurface7;
	}

	return hr;
}
ULONG  IGA_IDirectDrawSurface::AddRef ()  
{
	return  m_pIDirectDrawSurface->AddRef()  ;
}
ULONG  IGA_IDirectDrawSurface::Release() 
{
	ULONG  ulRet =  m_pIDirectDrawSurface->Release() ;
	if ( ulRet == 0 )
	{
		delete this;
	}
	return ulRet;
}

HRESULT   IGA_IDirectDrawSurface::AddAttachedSurface( LPDIRECTDRAWSURFACE param)
{
	return m_pIDirectDrawSurface->AddAttachedSurface( param);
}

HRESULT   IGA_IDirectDrawSurface::AddOverlayDirtyRect( LPRECT param)
{
	return m_pIDirectDrawSurface->AddOverlayDirtyRect(param);
}
HRESULT IGA_IDirectDrawSurface::Blt(LPRECT param1, LPDIRECTDRAWSURFACE param2, LPRECT param3 , DWORD param4, LPDDBLTFX param5)
{
	return m_pIDirectDrawSurface->Blt(param1, param2, param3, param4, param5 );
}

HRESULT IGA_IDirectDrawSurface::BltBatch( LPDDBLTBATCH param1, DWORD param2, DWORD param3)
{
	return m_pIDirectDrawSurface->BltBatch(param1, param2, param3 );
}

HRESULT IGA_IDirectDrawSurface::BltFast(DWORD param1,DWORD param2,LPDIRECTDRAWSURFACE param3, LPRECT param4,DWORD param5)
{
	return m_pIDirectDrawSurface->BltFast(param1, param2, param3, param4, param5);
}

HRESULT IGA_IDirectDrawSurface::DeleteAttachedSurface( DWORD param1, LPDIRECTDRAWSURFACE param2)
{
	return m_pIDirectDrawSurface->DeleteAttachedSurface(param1, param2);
}
HRESULT IGA_IDirectDrawSurface::EnumAttachedSurfaces( LPVOID param1, LPDDENUMSURFACESCALLBACK param2)
{
	return m_pIDirectDrawSurface->EnumAttachedSurfaces(param1, param2);
}
HRESULT IGA_IDirectDrawSurface::EnumOverlayZOrders( DWORD param1, LPVOID param2, LPDDENUMSURFACESCALLBACK param3)
{
	return m_pIDirectDrawSurface->EnumOverlayZOrders( param1, param2, param3);
}
HRESULT IGA_IDirectDrawSurface::Flip(LPDIRECTDRAWSURFACE param1, DWORD param2)
{
	return m_pIDirectDrawSurface->Flip( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface::GetAttachedSurface( LPDDSCAPS param1, LPDIRECTDRAWSURFACE FAR * param2)
{
	return m_pIDirectDrawSurface->GetAttachedSurface( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface::GetBltStatus( DWORD param)
{
	return m_pIDirectDrawSurface->GetBltStatus(param);
}
HRESULT IGA_IDirectDrawSurface::GetCaps( LPDDSCAPS param)
{
	return m_pIDirectDrawSurface->GetCaps( param );
}
HRESULT IGA_IDirectDrawSurface::GetClipper( LPDIRECTDRAWCLIPPER FAR * param)
{
	return m_pIDirectDrawSurface->GetClipper( param );
}
HRESULT IGA_IDirectDrawSurface::GetColorKey( DWORD param1, LPDDCOLORKEY param2 )
{
	return m_pIDirectDrawSurface->GetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface::GetDC( HDC FAR * param )
{
	return m_pIDirectDrawSurface->GetDC( param);
}
HRESULT IGA_IDirectDrawSurface::GetFlipStatus( DWORD param )
{
	return m_pIDirectDrawSurface->GetFlipStatus( param );
}
HRESULT IGA_IDirectDrawSurface::GetOverlayPosition( LPLONG param1 , LPLONG param2 )
{
	return m_pIDirectDrawSurface->GetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface::GetPalette( LPDIRECTDRAWPALETTE FAR* param)
{
	return m_pIDirectDrawSurface->GetPalette( param );
}
HRESULT IGA_IDirectDrawSurface::GetPixelFormat( LPDDPIXELFORMAT param)
{
	return m_pIDirectDrawSurface->GetPixelFormat( param );
}
HRESULT IGA_IDirectDrawSurface::GetSurfaceDesc( LPDDSURFACEDESC param)
{
	return  m_pIDirectDrawSurface->GetSurfaceDesc( param );
}
HRESULT IGA_IDirectDrawSurface::Initialize( LPDIRECTDRAW param1, LPDDSURFACEDESC param2 )
{
	return m_pIDirectDrawSurface->Initialize( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface::IsLost()
{
	return m_pIDirectDrawSurface->IsLost();
}
HRESULT IGA_IDirectDrawSurface::Lock( LPRECT param1, LPDDSURFACEDESC param2, DWORD param3, HANDLE param4)
{
	return m_pIDirectDrawSurface->Lock(param1, param2, param3, param4 );
}
HRESULT IGA_IDirectDrawSurface::ReleaseDC( HDC param)
{
	return m_pIDirectDrawSurface->ReleaseDC(param );
}
HRESULT IGA_IDirectDrawSurface::Restore()
{
	return m_pIDirectDrawSurface->Restore();
}
HRESULT IGA_IDirectDrawSurface::SetClipper( LPDIRECTDRAWCLIPPER param)
{
	return m_pIDirectDrawSurface->SetClipper( param );
}
HRESULT IGA_IDirectDrawSurface::SetColorKey( DWORD param1, LPDDCOLORKEY param2)
{
	return m_pIDirectDrawSurface->SetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface::SetOverlayPosition( LONG param1, LONG param2)
{
	return m_pIDirectDrawSurface->SetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface::SetPalette( LPDIRECTDRAWPALETTE param)
{
	return m_pIDirectDrawSurface->SetPalette( param );
}
HRESULT IGA_IDirectDrawSurface::Unlock( LPVOID param)
{
	return m_pIDirectDrawSurface->Unlock(param);
}
HRESULT IGA_IDirectDrawSurface::UpdateOverlay( LPRECT param1, LPDIRECTDRAWSURFACE param2, LPRECT param3, DWORD param4, LPDDOVERLAYFX param5)
{
	return m_pIDirectDrawSurface->UpdateOverlay( param1, param2, param3, param4, param5 );
}
HRESULT IGA_IDirectDrawSurface::UpdateOverlayDisplay( DWORD param)
{
	return m_pIDirectDrawSurface->UpdateOverlayDisplay( param );
}
HRESULT IGA_IDirectDrawSurface::UpdateOverlayZOrder( DWORD param1, LPDIRECTDRAWSURFACE param2 )
{
	return m_pIDirectDrawSurface->UpdateOverlayZOrder( param1, param2 );
}

//////////////////////////////////////////////////////////////////////////

IGA_IDirectDrawSurface2::IGA_IDirectDrawSurface2(IDirectDrawSurface2* pIDirectDrawSurface2)
:m_pIDirectDrawSurface2( pIDirectDrawSurface2 )
{
}

IGA_IDirectDrawSurface2::~IGA_IDirectDrawSurface2(void)
{
}



HRESULT IGA_IDirectDrawSurface2::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	HRESULT hr = m_pIDirectDrawSurface2->QueryInterface (riid,  ppvObj);       
	if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface  )) 
	{		  
		IGA_IDirectDrawSurface* pIDirectSurface = new IGA_IDirectDrawSurface((IGA_IDirectDrawSurface*) *ppvObj );
		*ppvObj = pIDirectSurface;
		
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface2 )) 
	{
		*ppvObj = this;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface3)) 
	{
		IGA_IDirectDrawSurface3*   pIDirectSurface3 = new IGA_IDirectDrawSurface3((IGA_IDirectDrawSurface3*)*ppvObj);
		*ppvObj = pIDirectSurface3;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface4)) 
	{
		IGA_IDirectDrawSurface4*   pIDirectSurface4 = new IGA_IDirectDrawSurface4((IGA_IDirectDrawSurface4*)*ppvObj);
		*ppvObj  = pIDirectSurface4;
	}
	else if ( ::IsEqualIID( riid, Proxy_IDirectDrawSurface7))
	{
		IGA_IDirectDrawSurface7* pIDirectSurface7 = new IGA_IDirectDrawSurface7((IGA_IDirectDrawSurface7 *) *ppvObj);
		*ppvObj = pIDirectSurface7;
	}

	return hr;
}
ULONG  IGA_IDirectDrawSurface2::AddRef ()  
{
	return  m_pIDirectDrawSurface2->AddRef()  ;
}
ULONG  IGA_IDirectDrawSurface2::Release() 
{
	ULONG  ulRet =  m_pIDirectDrawSurface2->Release() ;
	if ( ulRet == 0 )
	{
		delete this;
	}
	return ulRet;
}

HRESULT   IGA_IDirectDrawSurface2::AddAttachedSurface( LPDIRECTDRAWSURFACE2 param)
{
	return m_pIDirectDrawSurface2->AddAttachedSurface( param);
}

HRESULT   IGA_IDirectDrawSurface2::AddOverlayDirtyRect( LPRECT param)
{
	return m_pIDirectDrawSurface2->AddOverlayDirtyRect(param);
}
HRESULT IGA_IDirectDrawSurface2::Blt(LPRECT param1, LPDIRECTDRAWSURFACE2 param2, LPRECT param3 , DWORD param4, LPDDBLTFX param5)
{
	return m_pIDirectDrawSurface2->Blt(param1, param2, param3, param4, param5 );
}

HRESULT IGA_IDirectDrawSurface2::BltBatch( LPDDBLTBATCH param1, DWORD param2, DWORD param3)
{
	return m_pIDirectDrawSurface2->BltBatch(param1, param2, param3 );
}

HRESULT IGA_IDirectDrawSurface2::BltFast(DWORD param1, DWORD param2, LPDIRECTDRAWSURFACE2 param3, LPRECT param4, DWORD param5 )
{
	return m_pIDirectDrawSurface2->BltFast(param1, param2, param3, param4, param5 );
}

HRESULT IGA_IDirectDrawSurface2::DeleteAttachedSurface( DWORD param1, LPDIRECTDRAWSURFACE2 param2)
{
	return m_pIDirectDrawSurface2->DeleteAttachedSurface(param1, param2);
}
HRESULT IGA_IDirectDrawSurface2::EnumAttachedSurfaces( LPVOID param1, LPDDENUMSURFACESCALLBACK param2)
{
	return m_pIDirectDrawSurface2->EnumAttachedSurfaces(param1, param2);
}
HRESULT IGA_IDirectDrawSurface2::EnumOverlayZOrders( DWORD param1, LPVOID param2, LPDDENUMSURFACESCALLBACK param3)
{
	return m_pIDirectDrawSurface2->EnumOverlayZOrders( param1, param2, param3);
}
HRESULT IGA_IDirectDrawSurface2::Flip(LPDIRECTDRAWSURFACE2 param1, DWORD param2)
{
	return m_pIDirectDrawSurface2->Flip( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface2::GetAttachedSurface( LPDDSCAPS param1, LPDIRECTDRAWSURFACE2 FAR * param2)
{
	return m_pIDirectDrawSurface2->GetAttachedSurface( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface2::GetBltStatus( DWORD param)
{
	return m_pIDirectDrawSurface2->GetBltStatus(param);
}
HRESULT IGA_IDirectDrawSurface2::GetCaps( LPDDSCAPS param)
{
	return m_pIDirectDrawSurface2->GetCaps( param );
}
HRESULT IGA_IDirectDrawSurface2::GetClipper( LPDIRECTDRAWCLIPPER FAR * param)
{
	return m_pIDirectDrawSurface2->GetClipper( param );
}
HRESULT IGA_IDirectDrawSurface2::GetColorKey( DWORD param1, LPDDCOLORKEY param2 )
{
	return m_pIDirectDrawSurface2->GetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface2::GetDC( HDC FAR * param )
{
	return m_pIDirectDrawSurface2->GetDC( param);
}
HRESULT IGA_IDirectDrawSurface2::GetFlipStatus( DWORD param )
{
	return m_pIDirectDrawSurface2->GetFlipStatus( param );
}
HRESULT IGA_IDirectDrawSurface2::GetOverlayPosition( LPLONG param1 , LPLONG param2 )
{
	return m_pIDirectDrawSurface2->GetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface2::GetPalette( LPDIRECTDRAWPALETTE FAR* param)
{
	return m_pIDirectDrawSurface2->GetPalette( param );
}
HRESULT IGA_IDirectDrawSurface2::GetPixelFormat( LPDDPIXELFORMAT param)
{
	return m_pIDirectDrawSurface2->GetPixelFormat( param );
}
HRESULT IGA_IDirectDrawSurface2::GetSurfaceDesc( LPDDSURFACEDESC param)
{
	return  m_pIDirectDrawSurface2->GetSurfaceDesc( param );
}
HRESULT IGA_IDirectDrawSurface2::Initialize( LPDIRECTDRAW param1, LPDDSURFACEDESC param2 )
{
	return m_pIDirectDrawSurface2->Initialize( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface2::IsLost()
{
	return m_pIDirectDrawSurface2->IsLost();
}
HRESULT IGA_IDirectDrawSurface2::Lock( LPRECT param1, LPDDSURFACEDESC param2, DWORD param3, HANDLE param4)
{
	return m_pIDirectDrawSurface2->Lock(param1, param2, param3, param4 );
}
HRESULT IGA_IDirectDrawSurface2::ReleaseDC( HDC param)
{
	return m_pIDirectDrawSurface2->ReleaseDC(param );
}
HRESULT IGA_IDirectDrawSurface2::Restore()
{
	return m_pIDirectDrawSurface2->Restore();
}
HRESULT IGA_IDirectDrawSurface2::SetClipper( LPDIRECTDRAWCLIPPER param)
{
	return m_pIDirectDrawSurface2->SetClipper( param );
}
HRESULT IGA_IDirectDrawSurface2::SetColorKey( DWORD param1, LPDDCOLORKEY param2)
{
	return m_pIDirectDrawSurface2->SetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface2::SetOverlayPosition( LONG param1, LONG param2)
{
	return m_pIDirectDrawSurface2->SetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface2::SetPalette( LPDIRECTDRAWPALETTE param)
{
	return m_pIDirectDrawSurface2->SetPalette( param );
}
HRESULT IGA_IDirectDrawSurface2::Unlock( LPVOID param)
{
	return m_pIDirectDrawSurface2->Unlock(param);
}
HRESULT IGA_IDirectDrawSurface2::UpdateOverlay( LPRECT param1, LPDIRECTDRAWSURFACE2 param2, LPRECT param3, DWORD param4, LPDDOVERLAYFX param5)
{
	return m_pIDirectDrawSurface2->UpdateOverlay( param1, param2, param3, param4, param5 );
}
HRESULT IGA_IDirectDrawSurface2::UpdateOverlayDisplay( DWORD param)
{
	return m_pIDirectDrawSurface2->UpdateOverlayDisplay( param );
}
HRESULT IGA_IDirectDrawSurface2::UpdateOverlayZOrder( DWORD param1, LPDIRECTDRAWSURFACE2 param2 )
{
	return m_pIDirectDrawSurface2->UpdateOverlayZOrder( param1, param2 );
}

HRESULT IGA_IDirectDrawSurface2::GetDDInterface(LPVOID FAR * param)
{
	return m_pIDirectDrawSurface2->GetDDInterface( param );
}

HRESULT IGA_IDirectDrawSurface2::PageLock(DWORD param )
{
	return m_pIDirectDrawSurface2->PageLock( param);
}

HRESULT IGA_IDirectDrawSurface2::PageUnlock(DWORD param)
{
	return m_pIDirectDrawSurface2->PageUnlock( param );
}

//////////////////////////////////////////////////////////////////////////

IGA_IDirectDrawSurface3::IGA_IDirectDrawSurface3(IDirectDrawSurface3*   pIDirectDrawSurface3)
:m_pIDirectDrawSurface3( pIDirectDrawSurface3 )
{
}

IGA_IDirectDrawSurface3::~IGA_IDirectDrawSurface3(void)
{
}



HRESULT IGA_IDirectDrawSurface3::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	HRESULT hr = m_pIDirectDrawSurface3->QueryInterface (riid,  ppvObj);       
	if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface  )) 
	{		  
		IGA_IDirectDrawSurface* pIDirectSurface = new IGA_IDirectDrawSurface((IGA_IDirectDrawSurface*) *ppvObj );
		*ppvObj = pIDirectSurface;

	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface2 )) 
	{
		IGA_IDirectDrawSurface2*   pIDirectSurface2 = new IGA_IDirectDrawSurface2((IGA_IDirectDrawSurface2*)*ppvObj);
		*ppvObj = pIDirectSurface2;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface3)) 
	{
		*ppvObj = this;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface4)) 
	{
		IGA_IDirectDrawSurface4*   pIDirectSurface4 = new IGA_IDirectDrawSurface4((IGA_IDirectDrawSurface4*)*ppvObj);
		*ppvObj  = pIDirectSurface4;
	}
	else if ( ::IsEqualIID( riid, Proxy_IDirectDrawSurface7))
	{
		IGA_IDirectDrawSurface7* pIDirectSurface7 = new IGA_IDirectDrawSurface7((IGA_IDirectDrawSurface7 *) *ppvObj);
		*ppvObj = pIDirectSurface7;
	}
	return hr;
}
ULONG  IGA_IDirectDrawSurface3::AddRef ()  
{
	return  m_pIDirectDrawSurface3->AddRef()  ;
}
ULONG  IGA_IDirectDrawSurface3::Release() 
{
	ULONG  ulRet =  m_pIDirectDrawSurface3->Release() ;
	if ( ulRet == 0 )
	{
		delete this;
	}
	return ulRet;
}

HRESULT   IGA_IDirectDrawSurface3::AddAttachedSurface( LPDIRECTDRAWSURFACE3 param)
{
	return m_pIDirectDrawSurface3->AddAttachedSurface( param);
}

HRESULT   IGA_IDirectDrawSurface3::AddOverlayDirtyRect( LPRECT param)
{
	return m_pIDirectDrawSurface3->AddOverlayDirtyRect(param);
}
HRESULT IGA_IDirectDrawSurface3::Blt(LPRECT param1, LPDIRECTDRAWSURFACE3 param2, LPRECT param3 , DWORD param4, LPDDBLTFX param5)
{
	return m_pIDirectDrawSurface3->Blt(param1, param2, param3, param4, param5 );
}

HRESULT IGA_IDirectDrawSurface3::BltBatch( LPDDBLTBATCH param1, DWORD param2, DWORD param3)
{
	return m_pIDirectDrawSurface3->BltBatch(param1, param2, param3 );
}
HRESULT IGA_IDirectDrawSurface3::BltFast(DWORD param1, DWORD param2, LPDIRECTDRAWSURFACE3 param3, LPRECT param4, DWORD param5)
{
	return m_pIDirectDrawSurface3->BltFast(param1, param2, param3, param4, param5 );
}

HRESULT IGA_IDirectDrawSurface3::DeleteAttachedSurface( DWORD param1, LPDIRECTDRAWSURFACE3 param2)
{
	return m_pIDirectDrawSurface3->DeleteAttachedSurface(param1, param2);
}

HRESULT IGA_IDirectDrawSurface3::EnumAttachedSurfaces( LPVOID param1, LPDDENUMSURFACESCALLBACK param2)
{
	return m_pIDirectDrawSurface3->EnumAttachedSurfaces(param1, param2);
}
HRESULT IGA_IDirectDrawSurface3::EnumOverlayZOrders( DWORD param1, LPVOID param2, LPDDENUMSURFACESCALLBACK param3)
{
	return m_pIDirectDrawSurface3->EnumOverlayZOrders( param1, param2, param3);
}
HRESULT IGA_IDirectDrawSurface3::Flip(LPDIRECTDRAWSURFACE3 param1, DWORD param2)
{
	return m_pIDirectDrawSurface3->Flip( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface3::GetAttachedSurface( LPDDSCAPS param1, LPDIRECTDRAWSURFACE3 FAR * param2)
{
	return m_pIDirectDrawSurface3->GetAttachedSurface( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface3::GetBltStatus( DWORD param)
{
	return m_pIDirectDrawSurface3->GetBltStatus(param);
}
HRESULT IGA_IDirectDrawSurface3::GetCaps( LPDDSCAPS param)
{
	return m_pIDirectDrawSurface3->GetCaps( param );
}
HRESULT IGA_IDirectDrawSurface3::GetClipper( LPDIRECTDRAWCLIPPER FAR * param)
{
	return m_pIDirectDrawSurface3->GetClipper( param );
}
HRESULT IGA_IDirectDrawSurface3::GetColorKey( DWORD param1, LPDDCOLORKEY param2 )
{
	return m_pIDirectDrawSurface3->GetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface3::GetDC( HDC FAR * param )
{
	return m_pIDirectDrawSurface3->GetDC( param);
}
HRESULT IGA_IDirectDrawSurface3::GetFlipStatus( DWORD param )
{
	return m_pIDirectDrawSurface3->GetFlipStatus( param );
}
HRESULT IGA_IDirectDrawSurface3::GetOverlayPosition( LPLONG param1 , LPLONG param2 )
{
	return m_pIDirectDrawSurface3->GetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface3::GetPalette( LPDIRECTDRAWPALETTE FAR* param)
{
	return m_pIDirectDrawSurface3->GetPalette( param );
}
HRESULT IGA_IDirectDrawSurface3::GetPixelFormat( LPDDPIXELFORMAT param)
{
	return m_pIDirectDrawSurface3->GetPixelFormat( param );
}
HRESULT IGA_IDirectDrawSurface3::GetSurfaceDesc( LPDDSURFACEDESC param)
{
	return  m_pIDirectDrawSurface3->GetSurfaceDesc( param );
}
HRESULT IGA_IDirectDrawSurface3::Initialize( LPDIRECTDRAW param1, LPDDSURFACEDESC param2 )
{
	return m_pIDirectDrawSurface3->Initialize( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface3::IsLost()
{
	return m_pIDirectDrawSurface3->IsLost();
}
HRESULT IGA_IDirectDrawSurface3::Lock( LPRECT param1, LPDDSURFACEDESC param2, DWORD param3, HANDLE param4)
{
	return m_pIDirectDrawSurface3->Lock(param1, param2, param3, param4 );
}
HRESULT IGA_IDirectDrawSurface3::ReleaseDC( HDC param)
{
	return m_pIDirectDrawSurface3->ReleaseDC(param );
}
HRESULT IGA_IDirectDrawSurface3::Restore()
{
	return m_pIDirectDrawSurface3->Restore();
}
HRESULT IGA_IDirectDrawSurface3::SetClipper( LPDIRECTDRAWCLIPPER param)
{
	return m_pIDirectDrawSurface3->SetClipper( param );
}
HRESULT IGA_IDirectDrawSurface3::SetColorKey( DWORD param1, LPDDCOLORKEY param2)
{
	return m_pIDirectDrawSurface3->SetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface3::SetOverlayPosition( LONG param1, LONG param2)
{
	return m_pIDirectDrawSurface3->SetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface3::SetPalette( LPDIRECTDRAWPALETTE param)
{
	return m_pIDirectDrawSurface3->SetPalette( param );
}
HRESULT IGA_IDirectDrawSurface3::Unlock( LPVOID param)
{
	return m_pIDirectDrawSurface3->Unlock(param);
}
HRESULT IGA_IDirectDrawSurface3::UpdateOverlay( LPRECT param1, LPDIRECTDRAWSURFACE3 param2, LPRECT param3, DWORD param4, LPDDOVERLAYFX param5)
{
	return m_pIDirectDrawSurface3->UpdateOverlay( param1, param2, param3, param4, param5 );
}
HRESULT IGA_IDirectDrawSurface3::UpdateOverlayDisplay( DWORD param)
{
	return m_pIDirectDrawSurface3->UpdateOverlayDisplay( param );
}
HRESULT IGA_IDirectDrawSurface3::UpdateOverlayZOrder( DWORD param1, LPDIRECTDRAWSURFACE3 param2 )
{
	return m_pIDirectDrawSurface3->UpdateOverlayZOrder( param1, param2 );
}

HRESULT IGA_IDirectDrawSurface3::GetDDInterface(LPVOID * param)
{
	return m_pIDirectDrawSurface3->GetDDInterface( param);
}

HRESULT IGA_IDirectDrawSurface3::PageLock( DWORD param )
{
	return m_pIDirectDrawSurface3->PageLock( param);
}

HRESULT IGA_IDirectDrawSurface3::PageUnlock(DWORD  param)
{
	return m_pIDirectDrawSurface3->PageUnlock(param );	
}

HRESULT IGA_IDirectDrawSurface3::SetSurfaceDesc(LPDDSURFACEDESC param1, DWORD param2)
{
	return m_pIDirectDrawSurface3->SetSurfaceDesc( param1, param2 );
}

//////////////////////////////////////////////////////////////////////////

IGA_IDirectDrawSurface4::IGA_IDirectDrawSurface4(IDirectDrawSurface4*   pIDirectDrawSurface4)
:m_pIDirectDrawSurface4( pIDirectDrawSurface4 )
{
}

IGA_IDirectDrawSurface4::~IGA_IDirectDrawSurface4(void)
{
}



HRESULT IGA_IDirectDrawSurface4::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	HRESULT hr = m_pIDirectDrawSurface4->QueryInterface (riid,  ppvObj);       
	if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface  )) 
	{		  
		IGA_IDirectDrawSurface* pIDirectSurface = new IGA_IDirectDrawSurface((IGA_IDirectDrawSurface*) *ppvObj );
		*ppvObj = pIDirectSurface;

	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface2 )) 
	{
		IGA_IDirectDrawSurface2*   pIDirectSurface2 = new IGA_IDirectDrawSurface2((IGA_IDirectDrawSurface2*)*ppvObj);
		*ppvObj = pIDirectSurface2;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface3)) 
	{
		IGA_IDirectDrawSurface3*   pIDirectSurface3 = new IGA_IDirectDrawSurface3((IGA_IDirectDrawSurface3*)*ppvObj);
		*ppvObj  = pIDirectSurface3;
		
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface4)) 
	{
		*ppvObj = this;
	}
	else if ( ::IsEqualIID( riid, Proxy_IDirectDrawSurface7))
	{
		IGA_IDirectDrawSurface7* pIDirectSurface7 = new IGA_IDirectDrawSurface7((IGA_IDirectDrawSurface7 *) *ppvObj);
		*ppvObj = pIDirectSurface7;
	}
	return hr;
}
ULONG  IGA_IDirectDrawSurface4::AddRef ()  
{
	return  m_pIDirectDrawSurface4->AddRef()  ;
}
ULONG  IGA_IDirectDrawSurface4::Release() 
{
	ULONG  ulRet =  m_pIDirectDrawSurface4->Release() ;
	if ( ulRet == 0 )
	{
		delete this;
	}
	return ulRet;
}

HRESULT   IGA_IDirectDrawSurface4::AddAttachedSurface( LPDIRECTDRAWSURFACE4 param)
{
	return m_pIDirectDrawSurface4->AddAttachedSurface( param);
}

HRESULT   IGA_IDirectDrawSurface4::AddOverlayDirtyRect( LPRECT param)
{
	return m_pIDirectDrawSurface4->AddOverlayDirtyRect(param);
}
HRESULT IGA_IDirectDrawSurface4::Blt(LPRECT param1, LPDIRECTDRAWSURFACE4 param2, LPRECT param3 , DWORD param4, LPDDBLTFX param5)
{
	return m_pIDirectDrawSurface4->Blt(param1, param2, param3, param4, param5 );
}

HRESULT IGA_IDirectDrawSurface4::BltBatch( LPDDBLTBATCH param1, DWORD param2, DWORD param3)
{
	return m_pIDirectDrawSurface4->BltBatch(param1, param2, param3 );
}

HRESULT IGA_IDirectDrawSurface4::BltFast(DWORD param1, DWORD param2, LPDIRECTDRAWSURFACE4 param3, LPRECT param4, DWORD param5)
{
	return m_pIDirectDrawSurface4->BltFast(param1, param2,param3, param4, param5 );
}

HRESULT IGA_IDirectDrawSurface4::DeleteAttachedSurface( DWORD param1, LPDIRECTDRAWSURFACE4 param2)
{
	return m_pIDirectDrawSurface4->DeleteAttachedSurface(param1, param2);
}
HRESULT IGA_IDirectDrawSurface4::EnumAttachedSurfaces( LPVOID param1, LPDDENUMSURFACESCALLBACK2 param2)
{
	return m_pIDirectDrawSurface4->EnumAttachedSurfaces(param1, param2);
}
HRESULT IGA_IDirectDrawSurface4::EnumOverlayZOrders( DWORD param1, LPVOID param2, LPDDENUMSURFACESCALLBACK2 param3)
{
	return m_pIDirectDrawSurface4->EnumOverlayZOrders( param1, param2, param3);
}
HRESULT IGA_IDirectDrawSurface4::Flip(LPDIRECTDRAWSURFACE4 param1, DWORD param2)
{
	return m_pIDirectDrawSurface4->Flip( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface4::GetAttachedSurface( LPDDSCAPS2 param1, LPDIRECTDRAWSURFACE4 FAR * param2)
{
	return m_pIDirectDrawSurface4->GetAttachedSurface( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface4::GetBltStatus( DWORD param)
{
	return m_pIDirectDrawSurface4->GetBltStatus(param);
}
HRESULT IGA_IDirectDrawSurface4::GetCaps( LPDDSCAPS2 param)
{
	return m_pIDirectDrawSurface4->GetCaps( param );
}
HRESULT IGA_IDirectDrawSurface4::GetClipper( LPDIRECTDRAWCLIPPER FAR * param)
{
	return m_pIDirectDrawSurface4->GetClipper( param );
}
HRESULT IGA_IDirectDrawSurface4::GetColorKey( DWORD param1, LPDDCOLORKEY param2 )
{
	return m_pIDirectDrawSurface4->GetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface4::GetDC( HDC FAR * param )
{
	return m_pIDirectDrawSurface4->GetDC( param);
}
HRESULT IGA_IDirectDrawSurface4::GetFlipStatus( DWORD param )
{
	return m_pIDirectDrawSurface4->GetFlipStatus( param );
}
HRESULT IGA_IDirectDrawSurface4::GetOverlayPosition( LPLONG param1 , LPLONG param2 )
{
	return m_pIDirectDrawSurface4->GetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface4::GetPalette( LPDIRECTDRAWPALETTE FAR* param)
{
	return m_pIDirectDrawSurface4->GetPalette( param );
}
HRESULT IGA_IDirectDrawSurface4::GetPixelFormat( LPDDPIXELFORMAT param)
{
	return m_pIDirectDrawSurface4->GetPixelFormat( param );
}
HRESULT IGA_IDirectDrawSurface4::GetSurfaceDesc( LPDDSURFACEDESC2 param)
{
	return  m_pIDirectDrawSurface4->GetSurfaceDesc( param );
}
HRESULT IGA_IDirectDrawSurface4::Initialize( LPDIRECTDRAW param1, LPDDSURFACEDESC2 param2 )
{
	return m_pIDirectDrawSurface4->Initialize( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface4::IsLost()
{
	return m_pIDirectDrawSurface4->IsLost();
}
HRESULT IGA_IDirectDrawSurface4::Lock( LPRECT param1, LPDDSURFACEDESC2 param2, DWORD param3, HANDLE param4)
{
	return m_pIDirectDrawSurface4->Lock(param1, param2, param3, param4 );
}
HRESULT IGA_IDirectDrawSurface4::ReleaseDC( HDC param)
{
	return m_pIDirectDrawSurface4->ReleaseDC(param );
}
HRESULT IGA_IDirectDrawSurface4::Restore()
{
	return m_pIDirectDrawSurface4->Restore();
}
HRESULT IGA_IDirectDrawSurface4::SetClipper( LPDIRECTDRAWCLIPPER param)
{
	return m_pIDirectDrawSurface4->SetClipper( param );
}
HRESULT IGA_IDirectDrawSurface4::SetColorKey( DWORD param1, LPDDCOLORKEY param2)
{
	return m_pIDirectDrawSurface4->SetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface4::SetOverlayPosition( LONG param1, LONG param2)
{
	return m_pIDirectDrawSurface4->SetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface4::SetPalette( LPDIRECTDRAWPALETTE param)
{
	return m_pIDirectDrawSurface4->SetPalette( param );
}
HRESULT IGA_IDirectDrawSurface4::Unlock( LPRECT param)
{
	return m_pIDirectDrawSurface4->Unlock(param);
}
HRESULT IGA_IDirectDrawSurface4::UpdateOverlay( LPRECT param1, LPDIRECTDRAWSURFACE4 param2, LPRECT param3, DWORD param4, LPDDOVERLAYFX param5)
{
	return m_pIDirectDrawSurface4->UpdateOverlay( param1, param2, param3, param4, param5 );
}
HRESULT IGA_IDirectDrawSurface4::UpdateOverlayDisplay( DWORD param)
{
	return m_pIDirectDrawSurface4->UpdateOverlayDisplay( param );
}
HRESULT IGA_IDirectDrawSurface4::UpdateOverlayZOrder( DWORD param1, LPDIRECTDRAWSURFACE4 param2 )
{
	return m_pIDirectDrawSurface4->UpdateOverlayZOrder( param1, param2 );
}

HRESULT IGA_IDirectDrawSurface4::GetDDInterface(LPVOID FAR* param)
{
	return m_pIDirectDrawSurface4->GetDDInterface(param);
}
HRESULT IGA_IDirectDrawSurface4::PageLock(DWORD  param)
{
	return m_pIDirectDrawSurface4->PageLock( param );
}
HRESULT IGA_IDirectDrawSurface4::PageUnlock(DWORD  param)
{
	return m_pIDirectDrawSurface4->PageUnlock( param );
}

HRESULT IGA_IDirectDrawSurface4::SetSurfaceDesc(LPDDSURFACEDESC2 param1, DWORD param2)
{
	return m_pIDirectDrawSurface4->SetSurfaceDesc( param1, param2);
}

HRESULT IGA_IDirectDrawSurface4::SetPrivateData(REFGUID param1, LPVOID param2 , DWORD param3, DWORD param4 )
{
	return m_pIDirectDrawSurface4->SetPrivateData( param1, param2, param3, param4 );
}

HRESULT IGA_IDirectDrawSurface4::GetPrivateData(REFGUID param1 , LPVOID param2 , LPDWORD param3 )
{
	return m_pIDirectDrawSurface4->GetPrivateData( param1, param2, param3 );
}

HRESULT IGA_IDirectDrawSurface4::FreePrivateData(REFGUID param)
{
	return m_pIDirectDrawSurface4->FreePrivateData( param );
}

HRESULT IGA_IDirectDrawSurface4::GetUniquenessValue(LPDWORD  param )
{
	return m_pIDirectDrawSurface4->GetUniquenessValue( param );
}

HRESULT IGA_IDirectDrawSurface4::ChangeUniquenessValue()
{
	return m_pIDirectDrawSurface4->ChangeUniquenessValue();
}

//////////////////////////////////////////////////////////////////////////

IGA_IDirectDrawSurface7::IGA_IDirectDrawSurface7(IDirectDrawSurface7*   pIDirectDrawSurface7)
:m_pIDirectDrawSurface7( pIDirectDrawSurface7 )
{
}

IGA_IDirectDrawSurface7::~IGA_IDirectDrawSurface7(void)
{
}



HRESULT IGA_IDirectDrawSurface7::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	HRESULT hr = m_pIDirectDrawSurface7->QueryInterface (riid,  ppvObj);       
	if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface  )) 
	{		  
		IGA_IDirectDrawSurface* pIDirectSurface = new IGA_IDirectDrawSurface((IGA_IDirectDrawSurface*) *ppvObj );
		*ppvObj = pIDirectSurface;

	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface2 )) 
	{
		IGA_IDirectDrawSurface2*   pIDirectSurface2 = new IGA_IDirectDrawSurface2((IGA_IDirectDrawSurface2*)*ppvObj);
		*ppvObj = pIDirectSurface2;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface3)) 
	{
		IGA_IDirectDrawSurface3*   pIDirectSurface3 = new IGA_IDirectDrawSurface3((IGA_IDirectDrawSurface3*)*ppvObj);
		*ppvObj  = pIDirectSurface3;

	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDrawSurface4)) 
	{
		IGA_IDirectDrawSurface4* pIDirectSurface4 = new IGA_IDirectDrawSurface4((IGA_IDirectDrawSurface4 *) *ppvObj);
		*ppvObj = pIDirectSurface4;
	}
	else if ( ::IsEqualIID( riid, Proxy_IDirectDrawSurface7))
	{
		*ppvObj = this;
	}
	return hr;
}
ULONG  IGA_IDirectDrawSurface7::AddRef ()  
{
	return  m_pIDirectDrawSurface7->AddRef()  ;
}
ULONG  IGA_IDirectDrawSurface7::Release() 
{
	ULONG  ulRet =  m_pIDirectDrawSurface7->Release() ;
	if ( ulRet == 0 )
	{
		delete this;
	}
	return ulRet;
}

HRESULT   IGA_IDirectDrawSurface7::AddAttachedSurface( LPDIRECTDRAWSURFACE7 param)
{
	return m_pIDirectDrawSurface7->AddAttachedSurface( param);
}

HRESULT   IGA_IDirectDrawSurface7::AddOverlayDirtyRect( LPRECT param)
{
	return m_pIDirectDrawSurface7->AddOverlayDirtyRect(param);
}
HRESULT IGA_IDirectDrawSurface7::Blt(LPRECT param1, LPDIRECTDRAWSURFACE7 param2, LPRECT param3 , DWORD param4, LPDDBLTFX param5)
{
	return m_pIDirectDrawSurface7->Blt(param1, param2, param3, param4, param5 );
}

HRESULT IGA_IDirectDrawSurface7::BltBatch( LPDDBLTBATCH param1, DWORD param2, DWORD param3)
{
	return m_pIDirectDrawSurface7->BltBatch(param1, param2, param3 );
}

HRESULT IGA_IDirectDrawSurface7::BltFast(DWORD param1, DWORD param2, LPDIRECTDRAWSURFACE7 param3, LPRECT param4, DWORD param5)
{
	return m_pIDirectDrawSurface7->BltFast(param1, param2,param3, param4, param5 );
}

HRESULT IGA_IDirectDrawSurface7::DeleteAttachedSurface( DWORD param1, LPDIRECTDRAWSURFACE7 param2)
{
	return m_pIDirectDrawSurface7->DeleteAttachedSurface(param1, param2);
}
HRESULT IGA_IDirectDrawSurface7::EnumAttachedSurfaces( LPVOID param1, LPDDENUMSURFACESCALLBACK7 param2)
{
	return m_pIDirectDrawSurface7->EnumAttachedSurfaces(param1, param2);
}
HRESULT IGA_IDirectDrawSurface7::EnumOverlayZOrders( DWORD param1, LPVOID param2, LPDDENUMSURFACESCALLBACK7 param3)
{
	return m_pIDirectDrawSurface7->EnumOverlayZOrders( param1, param2, param3);
}
HRESULT IGA_IDirectDrawSurface7::Flip(LPDIRECTDRAWSURFACE7 param1, DWORD param2)
{
	return m_pIDirectDrawSurface7->Flip( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface7::GetAttachedSurface( LPDDSCAPS2 param1, LPDIRECTDRAWSURFACE7 FAR * param2)
{
	return m_pIDirectDrawSurface7->GetAttachedSurface( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface7::GetBltStatus( DWORD param)
{
	return m_pIDirectDrawSurface7->GetBltStatus(param);
}
HRESULT IGA_IDirectDrawSurface7::GetCaps( LPDDSCAPS2 param)
{
	return m_pIDirectDrawSurface7->GetCaps( param );
}
HRESULT IGA_IDirectDrawSurface7::GetClipper( LPDIRECTDRAWCLIPPER FAR * param)
{
	return m_pIDirectDrawSurface7->GetClipper( param );
}
HRESULT IGA_IDirectDrawSurface7::GetColorKey( DWORD param1, LPDDCOLORKEY param2 )
{
	return m_pIDirectDrawSurface7->GetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface7::GetDC( HDC FAR * param )
{
	return m_pIDirectDrawSurface7->GetDC( param);
}
HRESULT IGA_IDirectDrawSurface7::GetFlipStatus( DWORD param )
{
	return m_pIDirectDrawSurface7->GetFlipStatus( param );
}
HRESULT IGA_IDirectDrawSurface7::GetOverlayPosition( LPLONG param1 , LPLONG param2 )
{
	return m_pIDirectDrawSurface7->GetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface7::GetPalette( LPDIRECTDRAWPALETTE FAR* param)
{
	return m_pIDirectDrawSurface7->GetPalette( param );
}
HRESULT IGA_IDirectDrawSurface7::GetPixelFormat( LPDDPIXELFORMAT param)
{
	return m_pIDirectDrawSurface7->GetPixelFormat( param );
}
HRESULT IGA_IDirectDrawSurface7::GetSurfaceDesc( LPDDSURFACEDESC2 param)
{
	return  m_pIDirectDrawSurface7->GetSurfaceDesc( param );
}
HRESULT IGA_IDirectDrawSurface7::Initialize( LPDIRECTDRAW param1, LPDDSURFACEDESC2 param2 )
{
	return m_pIDirectDrawSurface7->Initialize( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface7::IsLost()
{
	return m_pIDirectDrawSurface7->IsLost();
}
HRESULT IGA_IDirectDrawSurface7::Lock( LPRECT param1, LPDDSURFACEDESC2 param2, DWORD param3, HANDLE param4)
{
	return m_pIDirectDrawSurface7->Lock(param1, param2, param3, param4 );
}
HRESULT IGA_IDirectDrawSurface7::ReleaseDC( HDC param)
{
	return m_pIDirectDrawSurface7->ReleaseDC(param );
}
HRESULT IGA_IDirectDrawSurface7::Restore()
{
	return m_pIDirectDrawSurface7->Restore();
}
HRESULT IGA_IDirectDrawSurface7::SetClipper( LPDIRECTDRAWCLIPPER param)
{
	return m_pIDirectDrawSurface7->SetClipper( param );
}
HRESULT IGA_IDirectDrawSurface7::SetColorKey( DWORD param1, LPDDCOLORKEY param2)
{
	return m_pIDirectDrawSurface7->SetColorKey( param1, param2 );
}
HRESULT IGA_IDirectDrawSurface7::SetOverlayPosition( LONG param1, LONG param2)
{
	return m_pIDirectDrawSurface7->SetOverlayPosition(param1, param2 );
}
HRESULT IGA_IDirectDrawSurface7::SetPalette( LPDIRECTDRAWPALETTE param)
{
	return m_pIDirectDrawSurface7->SetPalette( param );
}
HRESULT IGA_IDirectDrawSurface7::Unlock( LPRECT param)
{
	DDBLTFX ddbltfx; // this contains the DDBLTFX structure
	// clear out the structure and set the size field 
	memset(&ddbltfx, 0, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);

	// set the dwfillcolor field to the desired color
	ddbltfx.dwFillColor = 0x00ff00ff; 

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 100;
	rect.bottom = 100;
	g_pIDirectDrawSurface7->Blt(&rect,     // ptr to dest rectangle
		NULL,       // ptr to source surface, NA            
		NULL,       // ptr to source rectangle, NA
		DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
		&ddbltfx);  // ptr to DDBLTFX structure

	g_pIDirectDrawSurface7->Flip(NULL, DDFLIP_WAIT);
	OutputDebugString("IGA_IDirectDrawSurface7::Unlock()");
	return m_pIDirectDrawSurface7->Unlock(param);
}
HRESULT IGA_IDirectDrawSurface7::UpdateOverlay( LPRECT param1, LPDIRECTDRAWSURFACE7 param2, LPRECT param3, DWORD param4, LPDDOVERLAYFX param5)
{
	return m_pIDirectDrawSurface7->UpdateOverlay( param1, param2, param3, param4, param5 );
}
HRESULT IGA_IDirectDrawSurface7::UpdateOverlayDisplay( DWORD param)
{
	return m_pIDirectDrawSurface7->UpdateOverlayDisplay( param );
}
HRESULT IGA_IDirectDrawSurface7::UpdateOverlayZOrder( DWORD param1, LPDIRECTDRAWSURFACE7 param2 )
{
	return m_pIDirectDrawSurface7->UpdateOverlayZOrder( param1, param2 );
}

HRESULT IGA_IDirectDrawSurface7::GetDDInterface(LPVOID FAR* param)
{
	return m_pIDirectDrawSurface7->GetDDInterface(param);
}
HRESULT IGA_IDirectDrawSurface7::PageLock(DWORD  param)
{
	return m_pIDirectDrawSurface7->PageLock( param );
}
HRESULT IGA_IDirectDrawSurface7::PageUnlock(DWORD  param)
{
	return m_pIDirectDrawSurface7->PageUnlock( param );
}

HRESULT IGA_IDirectDrawSurface7::SetSurfaceDesc(LPDDSURFACEDESC2 param1, DWORD param2)
{
	return m_pIDirectDrawSurface7->SetSurfaceDesc( param1, param2);
}

HRESULT IGA_IDirectDrawSurface7::SetPrivateData(REFGUID param1, LPVOID param2 , DWORD param3, DWORD param4 )
{
	return m_pIDirectDrawSurface7->SetPrivateData( param1, param2, param3, param4 );
}

HRESULT IGA_IDirectDrawSurface7::GetPrivateData(REFGUID param1 , LPVOID param2 , LPDWORD param3 )
{
	return m_pIDirectDrawSurface7->GetPrivateData( param1, param2, param3 );
}

HRESULT IGA_IDirectDrawSurface7::FreePrivateData(REFGUID param)
{
	return m_pIDirectDrawSurface7->FreePrivateData( param );
}

HRESULT IGA_IDirectDrawSurface7::GetUniquenessValue(LPDWORD  param )
{
	return m_pIDirectDrawSurface7->GetUniquenessValue( param );
}

HRESULT IGA_IDirectDrawSurface7::ChangeUniquenessValue()
{
	return m_pIDirectDrawSurface7->ChangeUniquenessValue();
}

HRESULT IGA_IDirectDrawSurface7::SetPriority(DWORD param)
{
	return m_pIDirectDrawSurface7->SetPriority( param );
}
HRESULT IGA_IDirectDrawSurface7::GetPriority( LPDWORD param )
{
	return m_pIDirectDrawSurface7->GetPriority( param );
}

HRESULT IGA_IDirectDrawSurface7::SetLOD(DWORD param )
{
	return m_pIDirectDrawSurface7->SetLOD(param );
}

HRESULT IGA_IDirectDrawSurface7::GetLOD( LPDWORD param)
{
	return m_pIDirectDrawSurface7->GetLOD( param);
}



