/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : 3D_Core.h
*	����������IGA3D�ӹ�ʵ����, modify by hgl
*	�޸�˵�������������ķ���Ȩ�ޣ�ɾ��û���õ��ı����Ķ���
*	D3D�豸����ʹ������ָ�룬�Զ������豸�ͷ�
*	��ǰ�汾��2, 0, 9, 0304
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ� 
*************************************
*	�޸�˵����D3D�豸����ʹ������ָ�룬�Զ������豸�ͷ�
*	��ǰ�汾��2, 0, 9, 0317
*	����/�޸��ߣ�hgl
*	������ڣ�2009-3-17
*	ȡ���汾: 2, 0, 9, 0304
*	�޸��ˣ�
*	������ڣ�
/************************************************************************/

#pragma  once 

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <string>
#include <list>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "DefineConst.h"
#include "DefineStruct.h"
#include "GlobalFunction.h"

#include "MeshLibrary.h"
#include "IGA3D_UI.h"
#include "IGACamera.h"
#include "shellapi.h"
#include "z_Memory.h"
#include "tinyxml.h"
#include "IGA3D9.h"
#include "ShowPic.h"
#include "IGATexture9.h"

#include "atlbase.h"


typedef CShareMemory<STIGAStateData,1> IGA_SHAREMEMORY_DATA;


typedef HRESULT(WINAPI *pFunD3D9SearchInit)(LPDIRECT3DDEVICE9, LPARAM);
typedef void(WINAPI *pFunD3D9SearchRender)();
typedef void(WINAPI *pFunD3D9SearchDestory)();


//////////////////////////////////////////////////////////////////////////

class CIGA3DModule
{
public:
	CIGA3DModule();
	~CIGA3DModule();

public:
	int Init(HWND hGameWnd, LPDIRECT3DDEVICE9 pd3dDeviceOrig, LPDIRECT3DDEVICE9 pd3dDeviceProxy, LPCallBack3DFocus lpfocus, LPCallBackDeviceRefCount lpRefCount);
	void Destory();

	void PerIGA3DRender();
	void OnIGA3DRender();
	void PostIGA3DRender();
	void IGA3DInputState();
	void RunScript();
	void ComputeSceneID();
	int ReLoad();

public:
	void	SetGameWnd(HWND hWnd)	{ m_hGameWnd = hWnd; }
	HWND	GetGameWnd()			{ return m_hGameWnd; }

private:
	void CheckAdInfo();
	void SaveShowInfo();

//--------------------------------------
//2008-7-24��ӣ�
//�߼���Դ: �������汻ʰȡ�ϵĶ���ID��
public:
	int		m_iPickID;	//��ʰȡ��ģ�͵�ID,���С��0��ʾû��ʰȡ���κ�3D����
	float   m_fDistMin; //��¼���ߵ���Сֵ
	int		m_iCurObjID;	//��ǰ�Ķ���
	int		m_iUpdataObjID;	//��ǰ�Ķ���

	STIGA3DMSG	m_IGA3DMsg;	//��ǰ��Ϣ
	bool		m_bGetMsg;	//
	bool		m_bUpdate;

public:
	//ʹ�õ�D3D��Դ���Ǵ���Ϸ��ȡ�� 
	IDirect3DDevice9*	m_spD3DDevice;
	//IDirect3DDevice9 * m_pD3D9Device;
	IDirect3DDevice9*	m_spD3D9DeviceProxy; //�ӹ�d3d�豸����ָ��
	int m_iDeviceRefCount;

	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProj; 
	D3DXMATRIX			m_matWorld;
	volatile bool		m_bInitView;
	volatile bool		m_bInitProj;
	volatile bool		m_bInitWprld;

private:	//������Ϸ��Ⱦ״̬
	DWORD	m_dwFVF;
	DWORD	m_dwLighting;
	DWORD	m_dwCullMode;
	DWORD	m_dwZEnable;
	DWORD	m_dwVertexBlend;
	DWORD	m_dwTexStage0ColorOp;
	DWORD	m_dwTexStage0ColorArg1;
	DWORD	m_dwTexStage0ColorArg2;

	//CComPtr<IDirect3DBaseTexture9>	m_D3DBaseTexture;

	//ϵͳ��Դ
	CComPtr<IDirect3DStateBlock9>	m_pkOriginalStateBlock;
	CComPtr<IDirect3DStateBlock9>	m_pkProxyStateBlock;
	HWND	m_hGameWnd;   //��Ϸ���ھ��

public:
	bool	m_bFrist;
	int		m_iDrawPos;	//��Core����Ƶ�λ��
	DWORD	m_dwGameID;	//��ϷID
	bool	m_bIsLoad;	//ģ���Ƿ���سɹ�

private:
	list<STDrawElement>	m_listDrawLibrary;    //ģ���б�
	bool				m_bUseClip;
	bool				m_bCheckXML;

public:
	LPCallBack3DFocus			m_pFun3DFocus;		//�����Ļص�����
	LPCallBackDeviceRefCount	m_pFunDeviceRefCount; //�豸���ü���

private:
	//D3D9EditSearch��Ϸ������ʾ������
	BOOL					InitD3D9Search();
	tagD3DSearchParam		m_SearchInitParam;
	bool					m_bSearchLoad;
	HMODULE					m_hD3D9Search;
	pFunD3D9SearchInit		m_pFunD3D9SearchInit;
	pFunD3D9SearchRender	m_pFunD3D9SearchRender;
	pFunD3D9SearchDestory	m_pFunD3D9SearchDestory;

private:
	//��ʾͼƬ
	CShowPic			*m_pShowPic;
	CD3D9ManageTexture	 m_D3D9ManageTexture;

};

extern CIGA3DModule g_IGA3DModule;


