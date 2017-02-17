#include "stdafx.h"

CMeshLibrary* GetMeshLibrary()
{
	static CMeshLibrary meshLib;
	return &meshLib;
}


//--------------------------------------------------------
//
CMeshLibrary::CMeshLibrary()
{
	m_pd3dDevice = NULL;
}

CMeshLibrary::~CMeshLibrary()
{
	Destory();
}


HRESULT CMeshLibrary::Init( LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;

	m_MeshLibrary.clear();
	m_pNode		= NULL;
	m_pMesh		= NULL;

	m_MeshIDLibrary.clear();
	m_pNodeID	= NULL;

	m_ErrorID.clear();
	m_pErrorNode = NULL;

	char path[256];
	sprintf( path, "%s%s",PATH_WORK,"adv_res\\" );

	GetAbsPath( path );



	return hr;
}

void CMeshLibrary::Lost()
{
	for( m_pNode = m_MeshLibrary.begin(); m_pNode != m_MeshLibrary.end(); m_pNode++ )
	{
		DWORD	id = m_pNode->first;
		m_MeshIDLibrary.push_back(id);

		if( m_pNode->second != NULL )
		{
			delete m_pNode->second;
			m_pNode->second = NULL;
		}
	}

	m_MeshLibrary.clear();
}

void CMeshLibrary::Resume()
{
	for( m_pNodeID = m_MeshIDLibrary.begin(); m_pNodeID != m_MeshIDLibrary.end(); m_pNodeID++ )
	{
		DWORD	id = *m_pNodeID;
		AddResource( id );	
	}
	m_MeshIDLibrary.clear();
}

void CMeshLibrary::Render( DWORD id, D3DXMATRIX* pmatWorld, LPDIRECT3DTEXTURE9 pAdvTex, char* url )
{
	//���ж��ǲ���û�е���Դ
	m_pErrorNode = m_ErrorID.find(id);
	if( m_pErrorNode != m_ErrorID.end() )
	{
		GetIGA3DModule()->m_bIsLoad = false;
		return;
	}

	//�ڿ����Ƿ��ڵ�ǰ����
	m_pNode = m_MeshLibrary.find( id );
	if( m_pNode == m_MeshLibrary.end() )
	{
		//��Ҫ������Դ
		AddResource( id );
		return;	//�������������
	}

	//�����������������
	m_pMesh = m_pNode->second;

	//m_pd3dDevice->SetTransform(D3DTS_WORLD, pmatWorld );
	m_pMesh->Update(1.0f/60.0f, pmatWorld );
	m_pMesh->Render( pAdvTex );

	//-------------------------------
	//2008-7-22: ʰȡ����
		static bool bIsPick = false;
		short sStatus = 0;

		SIGA3DMSG msg = GetIGA3DModule()->m_Msg;
		if( msg.key == IGA3D_MSG_RBUTTONDOWN )
		{
			sStatus = -1;
		}
	
		if(sStatus < 0 && GetIGA3DModule()->m_PickID == -1 ) //û�ж���ѡ�е�ʱ������ж�
		{
			if( !bIsPick )
			{
				HWND hWnd = GetActiveWindow();
				if(hWnd)
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
					m_kView = GetIGA3DModule()->m_matView;
					m_kProjection = GetIGA3DModule()->m_matProj;
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

					bool bHit = m_pMesh->Pick(&vPickRayOrig, &vPickRayDir);
					if(bHit)		//ʰȡ�ɹ�
					{
						bIsPick = true;
						GetIGA3DModule()->m_PickID = id;	//����ID��3Dģ��

						char text[256];
						sprintf( text,"ʰȡ��ģ��;%d............",id );
						OutputDebugString(text);

						if( url[0] != '\0' )
						{
							ShellExecute( hWnd, "open", url, NULL, NULL, SW_SHOW );
							//OutputDebugString("�� url.............");
						}
						else
						{
							//OutputDebugString("NULL �� url.............");
						}

						if( GetIGA3DModule()->m_pFun != NULL )
						{
							//OutputDebugString("m_pFun != NULL ");
							GetIGA3DModule()->m_pFun(id);
						}

						//....
						GetIGA3DModule()->m_PickID = -1;
					}
				}
			}
		}
		else
		{
			bIsPick = false; 
		}

	//2008-7-22 ��
	//-------------------------------
}	

void CMeshLibrary::AddResource( DWORD id )
{
	//���������Դ
	FxAnimInstance* pSkinMesh = new FxAnimInstance(m_pd3dDevice);



	
	//����ID����Ҫ���ص������ļ�����
	char	fileName[256];
	
	sprintf( fileName, "m_%d.X", id );

	char acXFileName[2048], acXFilePath[2048], acXFileFullName[2048];
	strcpy(acXFileName, fileName);
	strcpy(acXFilePath, "InterAdtive\\EMMOA\\3D\\");
	strcpy(acXFileFullName, acXFilePath);
	strcat(acXFileFullName, acXFileName);
	GetAbsPath( acXFileFullName );
	GetAbsPath( acXFileName );

	HRESULT hr = pSkinMesh->InitialiseResource(acXFileFullName, acXFilePath);
	if( FAILED(hr) )
	{
		OutputDebugString("CSkinMeshLibrary:�����ļ�ʧ�ܣ�");
		delete pSkinMesh;
		m_ErrorID.insert(id);
		return;
	}
	GetIGA3DModule()->m_bIsLoad = true;

	//�Ѽ��ص�����ŵ�MESH����
	m_MeshLibrary.insert( MESH_NODE(id, pSkinMesh) );

}

void CMeshLibrary::ReMove( DWORD id )
{

}

void CMeshLibrary::Destory()
{
	Lost();
	m_MeshIDLibrary.clear();
	m_ErrorID.clear();
}
