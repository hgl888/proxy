#pragma once

#include "IGACommon.h"

DECLARE_DETOUR_CLASS(IDirect3DTexture9)
{
	/*** IUnknown methods ***/

	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, QueryInterface, (REFIID riid, void** ppvObj));
	DECLARE_DETOUR_MEMBER_V_STDCALL(ULONG, AddRef, (void));
	DECLARE_DETOUR_MEMBER_V_STDCALL(ULONG, Release, (void));

	/*** IDirect3DBaseTexture9 methods ***/

	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, GetDevice, (IDirect3DDevice9** ppDevice));
	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, SetPrivateData, (REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags));
	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, GetPrivateData, (REFGUID refguid,void* pData,DWORD* pSizeOfData));
	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, FreePrivateData, (REFGUID refguid));

	DECLARE_DETOUR_MEMBER_V_STDCALL(DWORD, SetPriority, (DWORD PriorityNew));
	DECLARE_DETOUR_MEMBER_V_STDCALL(DWORD, GetPriority, (void));

	DECLARE_DETOUR_MEMBER_V_STDCALL(void, PreLoad, (void));
	DECLARE_DETOUR_MEMBER_V_STDCALL(D3DRESOURCETYPE, GetType, (void));

	DECLARE_DETOUR_MEMBER_V_STDCALL(DWORD, SetLOD, (DWORD LODNew));
	DECLARE_DETOUR_MEMBER_V_STDCALL(DWORD, GetLOD, (void));
	DECLARE_DETOUR_MEMBER_V_STDCALL(DWORD, GetLevelCount, (void));

	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, SetAutoGenFilterType, (D3DTEXTUREFILTERTYPE FilterType));
	DECLARE_DETOUR_MEMBER_V_STDCALL(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType, (void));

	DECLARE_DETOUR_MEMBER_V_STDCALL(void, GenerateMipSubLevels, (void));

	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, GetLevelDesc, (UINT Level,D3DSURFACE_DESC *pDesc));

	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, GetSurfaceLevel, (UINT Level,IDirect3DSurface9** ppSurfaceLevel));

	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, LockRect, (UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags));
	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, UnlockRect, (UINT Level));
	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, AddDirtyRect, (CONST RECT* pDirtyRect));

	virtual void IDirect3DTexture9_END() {}
};

void InitIDirect3DTexture9Proxy(IDirect3DTexture9* pTexture);
