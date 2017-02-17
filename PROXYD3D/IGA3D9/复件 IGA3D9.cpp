/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : IGA3D9.cpp
*	IGA3D9.dll�����ļ�ʵ��
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�
/************************************************************************/

#include "stdafx.h"
#include "IGA3DModule.h"
#include "IGA3D9.h"


int g_bDubug = 0;		//	ȫ�ֱ����������Ƿ���ʾ������Ϣ����emmoa.ini�ļ��ж�ȡ


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	//OutputDebugString("IGA3D9  DllMain");
    return TRUE;
}
//��ʼ��3Dģ��
DWORD IGA3DInit(HWND hGameWnd, LPDIRECT3DDEVICE9 pd3dDeviceOrig, LPDIRECT3DDEVICE9 pd3dDeviceProxy, LPCallBack3DFocus lpfocus, LPCallBackDeviceRefCount lpRefCount )
{
	//
	//��ֹ�ظ���ʼ��
	//
	static bool s_bLoad = false;
	if ( ! s_bLoad )
	{
		s_bLoad = true;
	}
	else
	{
		IGA3DDestory();
	}
	setlocale(LC_ALL,"Chinese-simplified");
	char szNameINI[MAX_PATH];
	strcpy( szNameINI, "emmoa.ini" );
	GetModulePath(szNameINI);
	g_bDubug = GetPrivateProfileInt("Common", "debug", 0, (LPCTSTR)szNameINI);

	DWORD draw_pos = 0;
	__try
	{
		g_IGA3DModule.SetGameWnd( hGameWnd );
		g_IGA3DModule.m_spD3DDevice = pd3dDeviceOrig;
		g_IGA3DModule.m_spD3D9DeviceProxy = pd3dDeviceProxy;
		g_IGA3DModule.m_pFun3DFocus = lpfocus;
		g_IGA3DModule.m_pFunDeviceRefCount = lpRefCount;
		draw_pos = g_IGA3DModule.Init();
		g_IGA3DModule.m_iDrawPos = draw_pos;
	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
	}

	return draw_pos;
}

//�ͷ�3Dģ��
void IGA3DDestory()
{
	__try
	{
		g_IGA3DModule.Destory();
	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
	}
}

//��ȡ��������
void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix )
{
	__try
	{
		if( pMatrix == NULL )
		{
			return;
		}

		if( D3DTS_WORLD == State )
		{
			/*char szOutputString[256];
			sprintf(szOutputString, "_11= %f, _12= %f, _13= %f, _14=%f, _21=%f, _22=%f, _23=%f, _24=%f, \
			_31=%f, _32=%f, _33=%f, _34=%f, _41= %f, _42=%f, _43= %f, _44=%f", \
			pMatrix->_11, pMatrix->_12, pMatrix->_13, pMatrix->_14, pMatrix->_21, pMatrix->_22, pMatrix->_23, pMatrix->_24,\
			pMatrix->_31, pMatrix->_32, pMatrix->_33, pMatrix->_34, pMatrix->_41, pMatrix->_42, pMatrix->_43, pMatrix->_44);
			OutputDebugString(szOutputString);*/
			memcpy( &g_IGA3DModule.m_matWorld, pMatrix, sizeof(D3DXMATRIX) );
		}
		else if( (D3DTS_VIEW == State)  &&(!g_IGA3DModule.m_bInitView) )
		{
			if( g_IGA3DModule.m_dwGameID == IGA_WLOL )	 	//������Ⱥ������
			{	
				static int istaticWLCount = 0;
				if( fabsf(pMatrix->_41) > 1.0f )
				{
					if( istaticWLCount == 1 )
					{
						memcpy( &g_IGA3DModule.m_matView, pMatrix, sizeof(D3DXMATRIX) );
						g_IGA3DModule.m_bInitView = true;
						istaticWLCount = 0;
					}
					else
					{
						istaticWLCount++;
					}
				}
			}
			else  //������Ϸ
			{
				if( fabsf(pMatrix->_41) > 0.0f )
				{
					memcpy( &g_IGA3DModule.m_matView, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitView = true;
				}
			}


		}
		else if( ( D3DTS_PROJECTION == State) &&( !g_IGA3DModule.m_bInitProj ) )
		{

			switch (g_IGA3DModule.m_dwGameID)
			{
			case IGA_FENGYICHUANSHUO:  //��ӡ��˵
				/*if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
				memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				g_IGA3DModule.m_bInitProj = true;
				SHOWOUTPUTDEBUG(szOutputString);
				}*/
				//sprintf(szOutputString, "_11= %f, _12= %f, _13= %f, _14=%f, _21=%f, _22=%f, _23=%f, _24=%f, \
				_31=%f, _32=%f, _33=%f, _34=%f, _41= %f, _42=%f, _43= %f, _44=%f", \
				pMatrix->_11, pMatrix->_12, pMatrix->_13, pMatrix->_14, pMatrix->_21, pMatrix->_22, pMatrix->_23, pMatrix->_24,\
				pMatrix->_31, pMatrix->_32, pMatrix->_33, pMatrix->_34, pMatrix->_41, pMatrix->_42, pMatrix->_43, pMatrix->_44);
				//OutputDebugString(szOutputString);
				memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				g_IGA3DModule.m_bInitProj = true;
				break;
			case IGA_SHUSHAN:   //��ɽ
				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;
			case IGA_YINGTAOCHENG:   //Ӥ�Գ�
				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;
				//���������Ĵ��룬�����Ϸ�ܹ���ʾ3Dģ�͡�֧��ģ�͵����֧�ֲü�������ܹ�֧������ͳ�ƣ������¼��غ��˳�ʱ���Ὣģ�͵���ʾ��Ϣ���浽�ļ���
				//ģ�ͻ��ڵ�ס��ϷUI���档
			case IGA_SHENQI:
				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && ( pMatrix->_22 - pMatrix->_11 > EPSINON ) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;
				//���󺽺�ʱ�����Ĵ��룬֧��3Dģ�͵���ʾ�������֧�ֲü�������ܹ�֧������ͳ�ơ������¼��غ��˳�ʱ���Ὣģ�͵���ʾ��Ϣ���浽�ļ���
				//ģ�ͻ��ڵ�ס��ϷUI���档
			case IGA_GV:
				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;

				//������Ⱥ�������Ĵ��룬֧��3Dģ����ʾ�������֧�ֲü�������ܹ�֧������ͳ�ơ��ڻ�ȡ�ӿھ���ʱ��������Ҫ��
				//ע�⣺�����Ϸ��3Dģ����ʾ���Ǻ��ȶ������ض�����»ᵼ��ģ����ʾ�����������磬���������Ϸ�еĹ����ƾͿ��ܵ���ģ����ʾ�����������뿪һ�ξ����ֿ�����ʾ��
				//ģ�ͻ��ڵ�ס��ϷUI���档
			case IGA_WLOL:

				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON ) && pMatrix->_33 >= 1.0f)
				{
					memcpy(&g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DMATRIX));
					g_IGA3DModule.m_bInitProj = true;
				}
				break;

				//���漣���硷�Ĵ��룬֧��3Dģ����ʾ�������֧�ֲü�������ܹ�֧������ͳ�ơ������Ϸ����ʾʱ���е����⣨�о��ϲ��Ǹ���Ϸ������ͳһ����ϵ�£���������Ӱ����Ϸ��
				//ģ�ͻ��ڵ�ס��ϷUI���档
			case IGA_SUN:	
				if( FLOAT_EQAUL_ZERO( pMatrix->_44)  && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}	
				break;

				//���������򡷵Ĵ��룬֧��3Dģ����ʾ�������֧�ֲü�������ܹ�֧������ͳ�ơ������Ϸ����ʾʱ����Ϸ�ģգɱ߽���ڵ�ס��ӵģ���ģ�ͣ�Ӱ�첻��
				//�����Ϸ�Ƿֳ����ģ����ڲ�ͬ�ĵĳ����ܿ��ܻ�ʹ����һ���������꣬��ˣ���һ����������ӵ�ģ�ͣ���������һ��������Ҳ�ܿ�����
				//����Ҫ��Ч�ĳ���ʶ���ܡ�
			case IGA_RELIPAIQIU:
				if( FLOAT_EQAUL_ZERO( pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}

				break;

				//�����ܿ��������Ĵ��룬֧��3Dģ����ʾ�������֧�ֲü�������ܹ�֧������ͳ�ơ�
				//ģ�ͻ��ڵ�ס��Ϸ������˵������ʲô�ӽǶ�����ʾ����ǰ�棬�����Ǹ���Ϸ�ĳ�����һ���ռ��С�
				//�����Ϸ�Ƿֳ����ģ����ڲ�ͬ�ĵĳ����ܿ��ܻ�ʹ����һ���������꣬��ˣ���һ����������ӵ�ģ�ͣ���������һ��������Ҳ�ܿ���������Ҫ��Ч�ĳ���ʶ���ܡ�
			case IGA_PAOPAOKADINGCHE:
				if( FLOAT_EQAUL_ZERO( pMatrix->_44) && (pMatrix->_22 - pMatrix->_11 > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}

				break;

				//�ڡ�Ψ�����......
			case IGA_WEIWUDUZUN:
				if( FLOAT_EQAUL_ZERO( pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;
				//δ֪����Ϸ,����Ĭ�ϵ������Ϊ����Щ�����Ե���Ϸ��������Ҫ����Դ�����Ϸ������Ĵ����Ѿ����˾�����͡�
				//��Ȼ���Ժ󾭲��Ժ����Ϸ��Ҳ��Ҫ������ID��ӵ�����Ĵ��������֧�����˵����

			case IGA_TEST:
				if( FLOAT_EQAUL_ZERO( pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;

			default:
				memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				g_IGA3DModule.m_bInitProj = true;
				break;
			}	
		}

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
	}
}

//����
void IGA3DRender()
{
	__try
	{
		/*
		* �ӳټ���3Dģ�飬�ӳ�30����,����״̬�²��ӳټ��أ�
		*/
#ifndef _DEBUG
		static long s_time = GetTickCount();
		static bool s_bStartDraw = false;
		if( !s_bStartDraw )
		{
			if( GetTickCount() - s_time > 1*1000 )
			{
				s_bStartDraw = true;
			}
			else
			{
				return;
			}
		}
#endif

		if( ! g_IGA3DModule.m_bFrist  )
		{
			return;
		}

		g_IGA3DModule.m_bFrist = false;

		//��ȡ����״̬
		g_IGA3DModule.IGA3DInputState();

		//����
		g_IGA3DModule.PerIGA3DRender();

		//����IGA3D�ű�
		//g_IGA3DModule.RunScript();

		g_IGA3DModule.OnIGA3DRender();

		g_IGA3DModule.PostIGA3DRender();

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
	}


}

void SetFristDraw()
{
	g_IGA3DModule.m_bFrist = true;
	g_IGA3DModule.m_bInitView	= false;
	g_IGA3DModule.m_bInitProj	= false;
	g_IGA3DModule.m_bInitWprld	= false;
}

void SetUpdate()
{
	g_IGA3DModule.m_bUpdate = true;
}


