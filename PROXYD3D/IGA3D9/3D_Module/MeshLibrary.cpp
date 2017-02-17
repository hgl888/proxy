/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : MeshLibrary.cpp
*	����������IGA3D, modify by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#include "stdafx.h"
#include "IGA3D9.h"
#include "IGA3DModule.h"
#include "MeshLibrary.h"

extern float g_fDistPick;  //�������

CMeshLibrary g_CMeshLibrary;

CMeshLibrary::CMeshLibrary()
 : m_pd3dDevice(NULL),
   m_pAnimInstance(NULL)
{
}

CMeshLibrary::~CMeshLibrary()
{
	Destory();
}


HRESULT CMeshLibrary::Init( LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;

	if ( ! m_mapMeshLibrary.empty() )
	{
		m_mapMeshLibrary.clear();
	}
	if (! m_listMeshIDLibrary.empty() )
	{
		m_listMeshIDLibrary.clear();
	}
	if ( !m_setErrorID.empty() )
	{
		m_setErrorID.clear();
	}
	m_lRefCount = 0;

	return hr;
}

void CMeshLibrary::Lost()
{
	for( m_mapIterator = m_mapMeshLibrary.begin(); m_mapIterator != m_mapMeshLibrary.end(); m_mapIterator++ )
	{
		DWORD	dwID = m_mapIterator->first;
		m_listMeshIDLibrary.push_back(dwID);
		try
		{
			if( m_mapIterator->second != NULL )
			{
				m_mapIterator->second->ReleaseResource();
				delete m_mapIterator->second;
				m_mapIterator->second = NULL;
			}
		}
		catch(...)
		{
			if( IS_3D_TOOL )
			{
				OutputDebugString("occur exception at resource release!");
			}
		}

	}

	m_mapMeshLibrary.clear();

}

void CMeshLibrary::Resume()
{
	for( m_listIterator = m_listMeshIDLibrary.begin(); m_listIterator != m_listMeshIDLibrary.end(); m_listIterator++ )
	{
		DWORD	dwID = *m_listIterator;
		AddResource( dwID );	
	}
	m_listMeshIDLibrary.clear();
}


void CMeshLibrary::Render( DWORD dwID, D3DXMATRIX* pmatWorld, LPDIRECT3DTEXTURE9 pAdvTex, char* pchURL )
{
	//char szOutputDebug[256];
	//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
	//OutputDebugString( szOutputDebug );

	//���ж��ǲ���û�е���Դ
	m_setIterator = m_setErrorID.find(dwID);
	if( m_setIterator != m_setErrorID.end() )
	{
		g_IGA3DModule.m_bIsLoad = false;
		return;
	}
	
	m_mapIterator = m_mapMeshLibrary.find( dwID );
	if( m_mapIterator == m_mapMeshLibrary.end() )
	{
		AddResource( dwID );
		return;	//�������������
	}

	m_pAnimInstance = m_mapIterator->second;

	m_pAnimInstance->Update(1.0f/60.0f, pmatWorld );

	m_pAnimInstance->Render( pAdvTex );

	//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
	//OutputDebugString( szOutputDebug );

	//-------------------------------
	//2008-7-22: ʰȡ����
		//static bool bIsPick = false;
		//short sStatus = 0;

		//SIGA3DMSG msg = g_IGA3DModule.m_Msg;
		//if( msg.key == IGA3D_MSG_RBUTTONDOWN )
		//{
		//	sStatus = -1;
		//}
	
		//if(sStatus < 0 ) 
		{
			//if( !bIsPick )
			{
				HWND hWnd = GetActiveWindow();
				if(hWnd != NULL )
				{
					//�õ���Ļ�Ĵ�С
					IDirect3DSurface9 *pkD3DSurf;
					HRESULT hRes = m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pkD3DSurf);
					D3DSURFACE_DESC kD3DSurfDesc;
					pkD3DSurf->GetDesc(&kD3DSurfDesc);
					pkD3DSurf->Release();

					//�������ߵķ�������
					D3DXVECTOR3 vPickRayDir;
					D3DXVECTOR3 vPickRayOrig;

					//�ֱ�Ϊ��ͼ����ģ�ͣ����磩����ͶӰ����
					D3DXMATRIX m_kView, m_kWorld, m_kProjection;
					m_kView = g_IGA3DModule.m_matView;
					m_kProjection = g_IGA3DModule.m_matProj;
					m_kWorld      = *pmatWorld;

					POINT kCursor;
					GetCursorPos(&kCursor);
					ScreenToClient(hWnd, &kCursor);

					D3DVECTOR v;
					v.x = (((2.0f * kCursor.x) / kD3DSurfDesc.Width) - 1) / m_kProjection._11;
					v.y = -(((2.0f * kCursor.y) / kD3DSurfDesc.Height) - 1) / m_kProjection._22;
					v.z = 1.0f;

					D3DXMATRIX mWorldView = m_kWorld * m_kView;
					D3DXMATRIX m;
					D3DXMatrixInverse(&m, NULL, &mWorldView);

					vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
					vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
					vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
					vPickRayOrig.x = m._41;
					vPickRayOrig.y = m._42;
					vPickRayOrig.z = m._43;

					//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
					//OutputDebugString( szOutputDebug );

					float _fDist = 10000.0f;
					bool bHit = m_pAnimInstance->Pick(&vPickRayOrig, &vPickRayDir, NULL, &_fDist );
					if(bHit)		//ʰȡ�ɹ�
					{
						//char text[256];
						//sprintf( text, "_fDist:%f, DistPick: %f", _fDist, fDistPick );
						//OutputDebugString(text);
						if( _fDist > g_fDistPick )	//�������
						{
							return;						
						}
					
						int _PickID = g_IGA3DModule.m_iPickID;
						if( _PickID == -1 )	//��û�ж���ʰȡ��
						{
							g_IGA3DModule.m_iPickID = dwID;	//����ID��3Dģ��
							g_IGA3DModule.m_fDistMin = _fDist;
							g_IGA3DModule.m_iUpdataObjID = g_IGA3DModule.m_iCurObjID;
						}
						else	//������һ֡���Ѿ��ж���ѡ��
						{
							//�жϾ��룬ѡ�������Ӱ�������һ��
							if( g_IGA3DModule.m_fDistMin > _fDist )
							{
								g_IGA3DModule.m_iPickID = dwID;	//����ID��3Dģ��
								g_IGA3DModule.m_fDistMin = _fDist;
								g_IGA3DModule.m_iUpdataObjID = g_IGA3DModule.m_iCurObjID;
							}
						}

						//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
						//OutputDebugString( szOutputDebug );

						//char text[MAX_PATH];
						//sprintf( text,"ʰȡ��ģ��;%d............",id );
						//OutputDebugString(text);

						//if( url[0] != '\0' )
						//{
						//	ShellExecute( hWnd, "open", url, NULL, NULL, SW_SHOW );
						//	//OutputDebugString("�� url.............");
						//}
						//else
						//{
						//	//OutputDebugString("NULL �� url.............");
						//}

						//if( g_IGA3DModule.m_p3DFocus != NULL )
						//{
						//	//OutputDebugString("m_pFun != NULL ");
						//	g_IGA3DModule.m_p3DFocus(id, 0);
						//}

						//....
						//g_IGA3DModule.m_PickID = -1;
					}
				}
			}
		}
		//else
		//{
		//	bIsPick = false; 
		//}

		//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
		//OutputDebugString( szOutputDebug );
	//2008-7-22 ��
	//-------------------------------
}	

void CMeshLibrary::AddResource( DWORD id )
{
	//long  lRefCount = m_pd3dDevice->AddRef(); 

	//���������Դ
	FxAnimInstance* pSkinMesh = new FxAnimInstance(m_pd3dDevice);

	char	szFileName[MAX_PATH];
	sprintf( szFileName, "m_%d.X", id );

	char acXFileName[MAX_PATH], acXFilePath[MAX_PATH], acXFileFullName[MAX_PATH];
	strcpy(acXFileName, szFileName);
	strcpy(acXFilePath, "InterAdtive\\EMMOA\\3D\\");
	strcpy(acXFileFullName, acXFilePath);
	strcat(acXFileFullName, acXFileName);
	GetModulePath( acXFileFullName );
	GetModulePath( acXFileName );

	HRESULT hr = pSkinMesh->InitialiseResource(acXFileFullName, acXFilePath);
	if( FAILED(hr) )
	{
		if( IS_3D_TOOL )
		{
			OutputDebugString("CSkinMeshLibrary:�����ļ�ʧ�ܣ�");
		}
		delete pSkinMesh;
		m_setErrorID.insert(id);

		//m_pd3dDevice->Release();

		return;
	}
	g_IGA3DModule.m_bIsLoad = true;

	//�Ѽ��ص�����ŵ�MESH����
	m_mapMeshLibrary.insert( MAP_MESH_NODE(id, pSkinMesh) );

	//--------------------------------------------
	//m_pd3dDevice->AddRef();
 //   long  lRefCount1 = m_pd3dDevice->Release();
	//long  lRefCount2 = lRefCount1 - lRefCount ;
	//m_pd3dDevice->Release();

	//m_lRefCount += lRefCount2;

	//if( g_IGA3DModule.m_pFunDeviceRefCount != NULL && lRefCount2 != 0 )
	//{
	//	g_IGA3DModule.m_pFunDeviceRefCount( m_pd3dDevice, lRefCount2, 0 );
	//}

}

void CMeshLibrary::ReMove( DWORD id )
{

}

void CMeshLibrary::Destory()
{
	Lost();
	m_listMeshIDLibrary.clear();
	m_setErrorID.clear();

	//if( g_IGA3DModule.m_pFunDeviceRefCount != NULL && m_lRefCount != 0 )
	//{
	//	g_IGA3DModule.m_pFunDeviceRefCount( m_pd3dDevice, -m_lRefCount, 0 );
	//}
	//m_lRefCount = 0;

	/*if ( m_pd3dDevice != NULL )
	{
		m_pd3dDevice.Release();
	}*/
	m_pd3dDevice = NULL;
}
