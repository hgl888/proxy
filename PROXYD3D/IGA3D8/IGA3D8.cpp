// IGA3D.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "tchar.h"
#include "z_FileIni.h"
#include "3D_Core.h"
#include "IGA3D8.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

LPDIRECT3DDEVICE8 g_pDirect3DDevice8 = NULL;

DWORD IGA3DInit( LPDIRECT3DDEVICE8 pd3dDeviceOrig, LPCallBack3DFocus lpfocus, LPCallBackDeviceRefCount lpRefCount )
{

	g_IGA3DModule.m_pd3dDevice = pd3dDeviceOrig;
	g_pDirect3DDevice8 = pd3dDeviceOrig;
	int draw_pos = g_IGA3DModule.Init();
	g_IGA3DModule.m_iDrawPos = draw_pos;
	g_IGA3DModule.m_pFun3DFocus = lpfocus;
	g_IGA3DModule.m_pDeviceRefCount = lpRefCount;

	return draw_pos;
}

void IGA3DDestory()
{
	g_IGA3DModule.Destory();
}

void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix )
{
	if( pMatrix == NULL )
	{
		return;
	}
	if( D3DTS_WORLD == State )
	{
		//OutputDebugString("D3DTS_WORLD == State");
		memcpy( &g_IGA3DModule.m_matWorld, pMatrix, sizeof(D3DXMATRIX) );
		g_IGA3DModule.m_bInitWorld = true;
	}

	if( (D3DTS_VIEW == State)  && (! g_IGA3DModule.m_bInitView ) )
	{
		memcpy( &g_IGA3DModule.m_matView, pMatrix, sizeof(D3DXMATRIX) );
		g_IGA3DModule.m_bInitView = true;
	}
	if( (D3DTS_PROJECTION == State) && ( !g_IGA3DModule.m_bInitProj ) )
	{
		/*
		����������Ĵ���,�����Ϸ֧��3Dģ�͵���ʾ�����������֧�ֲü�������ܹ���¼ģ�͵���ʾʱ�䣨��λ���룩���ڸ��º���Ϸ�˳���ʱ���ܹ�����ʾ���ݱ��浽�ļ��С�
		*/
		int game_id = g_IGA3DModule.m_enumGameID;

		if( game_id == IGA_WUJIEQU				//���������
			|| game_id == IGA_WUJIEQU_MLXY			//�����(��������)
			|| game_id == IGA_WUJIEQU_XJP			//�����(�¼���)
			|| game_id == IGA_WUJIEQU_FLB)			//�����(���ɱ�)
		{
			if((pMatrix->_22 / pMatrix->_11 - 1.0f) > 0.0f && pMatrix->_33 >= 1.0f)
			{
				memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				g_IGA3DModule.m_bInitProj = true;
			}
		}

		/*
		�����𡷵Ĵ���,������ʾģ�ͣ������ܵ��,��֧�ֲü���
		ע�⣺�����Ϸ��ʾ3Dģ�ͻ������⵲����Ϸ�����ϣ���������Ϸ�ڲ�ģ����ͬһ�ռ�
		��ʾ�ϻ���ƫ�
		*/
		else if( g_IGA3DModule.m_enumGameID == IGA_TIANZUN )
		{
			if( pMatrix->_44 == 0.0f )	
			{
				if((pMatrix->_22 / pMatrix->_11 - 1.0f) > 0.0f && fabs(pMatrix->_33) >= 1.0f)
				{
					memcpy(&g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DMATRIX));
					g_IGA3DModule.m_bInitProj = true;
				}
			}			
		}
		/*
		δ֪����Ϸ,����Ĭ�ϵ������Ϊ����Щ�����Ե���Ϸ��������Ҫ����Դ�����Ϸ������Ĵ����Ѿ����˾�����͡�
		��Ȼ���Ժ󾭲��Ժ����Ϸ��Ҳ��Ҫ������ID��ӵ�����Ĵ��������֧�����˵����
		*/
		else
		{
			memcpy(&g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DMATRIX));
			g_IGA3DModule.m_bInitProj = true;
		}
	}
}

void IGA3DRender()
{
	if( g_IGA3DModule.m_bFrist == FALSE )
	{
		return;
	}
	g_IGA3DModule.m_bFrist = FALSE;
	g_IGA3DModule.IGA3DInputState();
	g_IGA3DModule.PerIGA3DRender();
	g_IGA3DModule.RunScript();
	g_IGA3DModule.OnIGA3DRender();
	g_IGA3DModule.PostIGA3DRender();
}

void SetFristDraw()
{
	g_IGA3DModule.m_bFrist		= TRUE;
	g_IGA3DModule.m_bInitView	= false;
	g_IGA3DModule.m_bInitProj	= false;
	g_IGA3DModule.m_bInitWorld	= false;
}

void SetUpdate()
{
	g_IGA3DModule.m_bUpdate = true;
}

void GetModulePath( char fullPathName[MAX_PATH] )
{
	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *p;
	p = strrchr(TmpPath,'\\');
	if(!p)
		return ;
	*(p+1) = 0;
	char path[MAX_PATH];
	sprintf(path,"%s%s",TmpPath,fullPathName );
	strcpy(fullPathName, path);
}
