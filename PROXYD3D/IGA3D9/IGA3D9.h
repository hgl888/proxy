/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : IGA3D9.h
*	IGA3D9.dll�����ļ�����
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�
/************************************************************************/

#pragma  once

#include "d3d9.h"
#include "d3dx9.h"


extern int g_bDubug;		//	ȫ�ֱ����������Ƿ���ʾ������Ϣ����emmoa.ini�ļ��ж�ȡ

//typedef void(*LPIGAFUN)(int id);
typedef VOID(WINAPI *LPCallBack3DFocus)(DWORD,LPVOID);
typedef VOID(WINAPI *LPCallBackDeviceRefCount)(VOID*,long,LPVOID);


DWORD IGA3DInit(HWND hGameWnd, LPDIRECT3DDEVICE9 pd3dDeviceOrig, LPDIRECT3DDEVICE9 pd3dDeviceProxy, LPCallBack3DFocus lpfocus, LPCallBackDeviceRefCount lpRefCount );
void IGA3DDestory();
void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix );
void IGA3DRender();
void SetFristDraw();
void SetUpdate();


void GetCameraPos();

