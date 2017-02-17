
#pragma  once


#include <d3d8.h>
#include <d3dx8.h>
#include <list>
#include "vector"
#include <time.h>
#include "tinyxml.h""
#include "define.h"
#include "z_Memory.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "IGA3D8.h"
#include "MeshLibrary.h"

class IGA3DModule
{
public:
	IGA3DModule();
	~IGA3DModule();

public:
	int Init();
	void Destory();
	void PerIGA3DRender();
	void OnIGA3DRender();
	void PostIGA3DRender();
	void IGA3DInputState();
	void RunScript();
	int ReLoad();
private:
	void CheckAdInfo();
	void SaveShowInfo();
//--------------------------------------
//2008-7-24��ӣ�
//�߼���Դ: �������汻ʰȡ�ϵĶ���ID��
public:

	int		m_PickID;			//��ʰȡ��ģ�͵�ID,���С��0��ʾû��ʰȡ���κ�3D����
	float   m_fDistMin;			//��¼���ߵ���Сֵ
	int		m_CurObjID;			//��ǰ�Ķ���
	int		m_UpdataObjID;		//��ǰ�Ķ���	

	bool		m_bGetMsg;	
	bool		m_bUpdate;
	SIGA3DMSG	m_Msg;			//��ǰ��Ϣ
//-------------------------------------
public:

	void GetCameraPos();
//ϵͳ��Դ
public:
	LPDIRECT3DDEVICE8	m_pd3dDevice;
	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProj;
	D3DXMATRIX			m_matWorld;
	bool				m_bInitView;
	bool				m_bInitProj;
	bool				m_bInitWorld;

public:
	BOOL		m_bFrist;
	long		m_lElapseTime;	//��ǰ��ͬ����ʱ��
	long		m_lStart;	
	bool		m_bIsLoad;		//ģ���Ƿ���سɹ�
	DWORD		m_dwState;
	int			m_iDrawPos;		//��Core����Ƶ�λ��

	ENUM_IGA_GAME_LIST			m_enumGameID; 

private:
	CFxSkeletalMesh *m_pkFxSkelMesh;
	STDrawElement	m_stDrawElement;

private:

	list<STDrawElement>				m_listDrawLibrary;
	list<STDrawElement>::iterator	m_listIterator;
	int								m_iCurrentScene;

public:
	LPIGAFUN					m_pFun;
	LPCallBack3DFocus			m_pFun3DFocus;		//�����Ļص�����
	LPCallBackDeviceRefCount	m_pDeviceRefCount; //�豸���ü���

//2008-8-14:IGA����
public:

	
	bool		m_bUseClip;

private:

	typedef HRESULT(*pFunInit)(LPDIRECT3DDEVICE8);
	typedef LPDIRECT3DTEXTURE8(*pFunGetTexture)(int,bool*, int *, int *);
	typedef void(*pFunDestroy)();
	typedef void(*pFunStopPlayer)(DWORD);

	HMODULE			m_hModule;
	pFunInit		m_pFunInit;
	pFunGetTexture	m_pFunGetTexture;
	pFunDestroy		m_pFunDestory;
	pFunStopPlayer  m_pFunStopPlayer;

	bool			m_bIGATexture;


	
};

extern IGA3DModule g_IGA3DModule;




