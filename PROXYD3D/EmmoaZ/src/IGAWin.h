#pragma once

//ZZhenFei 2008.06.20 16:01 ����

#ifndef  _IGAWIN_H
#define  _IGAWIN_H

//���ŵĹ�ע�Ȼ������
typedef VOID (WINAPI *  LP_FUN_PLAY_FOCUS)(DWORD  dwID,DWORD dwType,DWORD  dwCurTime,LPVOID  lpReserve );
namespace IGAWIN
{
	
	BOOL   InitWindow(LP_FUN_PLAY_FOCUS  playFocus = NULL);
	VOID   UnInitWindow();

	VOID   ShowVideo   ( LPCTSTR lpszCfg );
	VOID   ShowFlash   ( LPCTSTR lpszCfg );
	VOID   ShowActiveFlash   ( LPCTSTR lpszCfg );

	VOID   CloseVideo         ( );
	VOID   CloseFlash         ( );
	VOID   CloseActiveFlash   ( );

}

#endif