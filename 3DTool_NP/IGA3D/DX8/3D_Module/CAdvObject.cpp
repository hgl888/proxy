#include "stdafx.h"
CAdvObject* GetAdvObject()
{
	static CAdvObject advObject;
    return &advObject;
}

CAdvObjectTool* GetAdTool()
{
	static CAdvObjectTool adTool;
	return &adTool;
}

//-------------------------------------------------------------------------------------------------
//CAdvObject��ʵ��
CAdvObject::CAdvObject()
{
	m_SelectID   = 0xffffffff;

	ZeroMemory( m_vtx, sizeof( m_vtx[0] ) * 8 );
	ZeroMemory( m_plane, sizeof( m_plane[0] ) * 6 );

	m_MaxObjectID = 0;

	m_vPos = D3DXVECTOR3(0,0,0);

	D3DXMatrixIdentity(&m_MatrixTranslate);
	D3DXMatrixIdentity(&_m_MatrixTranslate);

}

CAdvObject::~CAdvObject()
{
	Release();
}

HRESULT CAdvObject::InitAdvObject( LPDIRECT3DDEVICE8 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	m_bIsDesignMode = false;
	m_nearLength    = 0.0f;

	m_Des.clear();

	//��ʼ������
	GetMeshLibrary()->Init( m_pd3dDevice );
	GetTexLibrary()->Init( m_pd3dDevice );

	GetSkinMesh()->Init( m_pd3dDevice );


	//�������ĳ�ʼ��
	InitSub();

	m_pPackObject	= NULL;

	return S_OK;
}

void CAdvObject::SetMatrixTranslation( D3DXMATRIX matTran )
{	
	D3DXMatrixMultiply(&m_MatrixTranslate, &matTran, &m_MatrixTranslate );
}

void CAdvObject::SetOffsetParam( float tran, float rot, float scal )
{
	m_OffsetTran	= tran;
	m_OffsetRot		= rot;
	m_OffsetScal	= scal;
}

void CAdvObject::AddDes( int mesh_id, int tex_id )
{
	m_Des.insert( NODE(mesh_id, tex_id) );
}

void CAdvObject::Render( D3DXMATRIX matView, D3DXMATRIX matProj )
{
	static bool b = true;
	if( GetIGA3DModule()->InputState[b_F5] )
	{ 
		if( b )
		{
			b = false;

			static int id = 0;
			
			id > 9 ? id = 0:id++;

			AddDes( 4, id );
		}
	}
	else
	{
		b = true;
	}


	m_matView = matView;
	m_matProj = matProj;
	
	D3DXMatrixRotationY( &_m_MatrixTranslate, GetIGA3DModule()->m_ElapseTime/1000.0f);
	
	UpdataViewCentrum( &m_matView, &m_matProj );
	KeyProcessor();
	for( list<AdvObjectDescription*>::iterator ii = m_AdvObjectContainer.begin(); ii != m_AdvObjectContainer.end(); ii++ )
	{
		//��ȡ��Ϣ
		AdvObjectDescription *tmp_pAdvObjectDescription = ((AdvObjectDescription*)(*ii));
		D3DXVECTOR3 tmp_CenterPoint = tmp_pAdvObjectDescription->CenterPoint;

		////----------------------------------------------------------------
		//��׵�ü�
		if( ViewCentrumClip( &tmp_CenterPoint ) == false )
		{
			if( m_bIsDesignMode == false && tmp_pAdvObjectDescription->is_Show )	//�����ʱͳ��
			{
				tmp_pAdvObjectDescription->is_Show = false;
				long tmp_time = GetTickCount() - tmp_pAdvObjectDescription->time;
				tmp_pAdvObjectDescription->e_time += tmp_time;
			}
			continue;	//���������׵����ж���һ������
		}
		
		//-----------------------------------------------------------------
	    //ͨ����׵�ü�
		//��ʾ��ʱͳ��
		if( m_bIsDesignMode == false )//�������ģʽ�½��м�ʱͳ��
		{
			if( tmp_pAdvObjectDescription->is_Show == false )
			{
				tmp_pAdvObjectDescription->is_Show = true;
				tmp_pAdvObjectDescription->time = GetTickCount();
			}
		}

		//ʰȡ�ж�
		if( GetIGA3DModule()->InputState[b_LBUTTON] )	//������������£��ͽ���һ��ʰȡ�ж�
		{
			if(tmp_pAdvObjectDescription->b_isPack == false ) //�����ǰ����û�б�ʰȡ��
			{
				//��ȡ�����Ϣ
				D3DXVECTOR3 tmp_Position, tmp_MinPosition, tmp_MaxPosition;
				tmp_Position	= tmp_pAdvObjectDescription->CenterPoint;
				tmp_MinPosition = tmp_pAdvObjectDescription->MinPoint;
				tmp_MaxPosition = tmp_pAdvObjectDescription->MaxPoint;
				BOOL tmp_isPack = Pick( &tmp_Position, &tmp_MinPosition, &tmp_MaxPosition );
				if( tmp_isPack )
				{
					//Ϊ�˷�ֹ��ͬһ��ֱ���ϵ���ײ�ж���ѡ�У���Ҫ���о����жϣ�
					//�ҳ������һ��ѡ�ж�����Ϊ��ʰȡ�ϵ�Ŀ��
					D3DXVECTOR3 tmp_Vec = tmp_Position - m_vPos;
					float length = D3DXVec3Length( &tmp_Vec );
					if( m_nearLength - length > 0.000001f)
					{
						m_nearLength = length;

						tmp_pAdvObjectDescription->b_isPack = true;
						if( m_bIsDesignMode == false )//�����ģʽ�²����е��ͳ��
						{
							tmp_pAdvObjectDescription->num_Pack++;
						}
						m_SelectID = tmp_pAdvObjectDescription->AdvObjectDescriptionID;
						if( m_pPackObject != NULL )
						{
							m_pPackObject->b_isPack = false;
						}
						m_pPackObject = tmp_pAdvObjectDescription;
						m_Iterator    = ii;
					}
				}
			}
		}
		else
		{
			if( m_bIsDesignMode == false && m_pPackObject != NULL )	//�൱��ȡ��ESC,���Ҳ������ģʽ��
			{
				m_pPackObject->b_isPack = false;
				m_pPackObject = NULL;
				m_Iterator = NULL;
			}
			m_nearLength = 65530.0f;	//���̧���ʱ��������0��
		}
			 
		//��������Ļ���
		RenderSub( tmp_pAdvObjectDescription );

		//���ƹ��ģ��
		int mesh_id = tmp_pAdvObjectDescription->mesh_id;
		int tex_id  = tmp_pAdvObjectDescription->texture_id;
		
		if(  mesh_id >= 0 )
		{
			LPDIRECT3DTEXTURE8 tmp_pTex = NULL;
			if( 1 )
			{
				//����λ��
				D3DXVECTOR3 cp = tmp_pAdvObjectDescription->CenterPoint;
				cp.y -= ( tmp_pAdvObjectDescription->MaxPoint.y - tmp_pAdvObjectDescription->MinPoint.y ) / 2;
				D3DXMATRIX  matWorld, matR;
				D3DXMatrixIdentity( &matWorld );
				matWorld._41 = cp.x;
				matWorld._42 = cp.y;
				matWorld._43 = cp.z;
				
				D3DXMatrixRotationY( &matR, tmp_pAdvObjectDescription->RotationY );
				
				if( tmp_pAdvObjectDescription->is_Active )
				{
					matWorld = matR * _m_MatrixTranslate * m_MatrixTranslate * matWorld;
				}
				else
				{
					matWorld = matR * m_MatrixTranslate * matWorld;
				}
				
				//ģ�ͽű�
				//RunScript( mesh_id, &tex_id );
	
				//����ģ��
				if( tex_id != -1 )
				{
					//��ȡ����
					LPDIRECT3DTEXTURE8 pAdvTex = GetTexLibrary()->GetTexture( tex_id );
					
					//....
					GetMeshLibrary()->Render( mesh_id,  &matWorld, pAdvTex );
				}
				else
				{
					GetMeshLibrary()->Render( mesh_id,  &matWorld, NULL );
				}
			}
		}
		else
		//int skinID = tmp_pAdvObjectDescription->skin_mesh_id;
		//if( skinID >= 0 )
		{
			//����λ��
			D3DXVECTOR3 cp = tmp_pAdvObjectDescription->CenterPoint;
			cp.y -= ( tmp_pAdvObjectDescription->MaxPoint.y - tmp_pAdvObjectDescription->MinPoint.y ) / 2;
			D3DXMATRIX  matWorld, matR;
			D3DXMatrixIdentity( &matWorld );
			matWorld._41 = cp.x;
			matWorld._42 = cp.y;
			matWorld._43 = cp.z;
			
			D3DXMatrixRotationY( &matR, tmp_pAdvObjectDescription->RotationY );
			
			if( tmp_pAdvObjectDescription->is_Active )
			{
				matWorld = matR * _m_MatrixTranslate * m_MatrixTranslate * matWorld;
			}
			else
			{
				matWorld = matR * m_MatrixTranslate * matWorld;
			}
			GetSkinMesh()->Render( 0, matWorld );
		}
	}
}

void CAdvObject::RunScript( int meshID, int* texID )
{
	m_pNode = m_Des.find(meshID);
	if( m_pNode == m_Des.end() )
	{
		return;
	}

	*texID = m_pNode->second;

	m_Des.erase(meshID);
}

void CAdvObject::Release()
{
	//���ͷ��������
	ReleaseSub();

	//�ͷŹ���������
	for( list<AdvObjectDescription*>::iterator ii = m_AdvObjectContainer.begin(); ii != m_AdvObjectContainer.end(); ii++ )
	{
		delete ((AdvObjectDescription*)(*ii));
	}
	m_AdvObjectContainer.clear();
}

bool CAdvObject::ViewCentrumClip( D3DXVECTOR3* pv )
{
	float		fDist;

	fDist = D3DXPlaneDotCoord( &m_plane[3], pv );
	if( fDist > PLANE_EPSILON ) return FALSE;	// plane�� normal����ָ�� far�����Ϊ��������ʾ��ƽ��ͷ���ⲿ.
	fDist = D3DXPlaneDotCoord( &m_plane[4], pv );
	if( fDist > PLANE_EPSILON ) return FALSE;	// plane�� normal����ָ�� left�����Ϊ��������ʾ��ƽ��ͷ������.
	fDist = D3DXPlaneDotCoord( &m_plane[5], pv );
	if( fDist > PLANE_EPSILON ) return FALSE;	// plane�� normal����ָ�� right�����Ϊ��������ʾ��ƽ��ͷ����ұ�.
	
	return TRUE;
}

void CAdvObject::UpdataViewCentrum( D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj )
{
	if( pmatView == NULL || pmatProj == NULL )
	{
		return;	
	}

	int				i;
	D3DXMATRIXA16	matInv;

	m_vtx[0].x = -1.0f;	m_vtx[0].y = -1.0f;	m_vtx[0].z = 0.0f;
	m_vtx[1].x =  1.0f;	m_vtx[1].y = -1.0f;	m_vtx[1].z = 0.0f;
	m_vtx[2].x =  1.0f;	m_vtx[2].y = -1.0f;	m_vtx[2].z = 1.0f;
	m_vtx[3].x = -1.0f;	m_vtx[3].y = -1.0f;	m_vtx[3].z = 1.0f;
	m_vtx[4].x = -1.0f;	m_vtx[4].y =  1.0f;	m_vtx[4].z = 0.0f;
	m_vtx[5].x =  1.0f;	m_vtx[5].y =  1.0f;	m_vtx[5].z = 0.0f;
	m_vtx[6].x =  1.0f;	m_vtx[6].y =  1.0f;	m_vtx[6].z = 1.0f;
	m_vtx[7].x = -1.0f;	m_vtx[7].y =  1.0f;	m_vtx[7].z = 1.0f;

	m_vtx[8].x = 0.0f;	m_vtx[8].y =  0.0f;	m_vtx[8].z = 0.8f;

	m_matView = *pmatView;
	m_matProj = *pmatProj;
	D3DXMATRIX matViewProj = m_matView * m_matProj;

	D3DXMatrixInverse(&matInv, NULL, &matViewProj );

	for( i = 0; i < 9; i++ )
		D3DXVec3TransformCoord( &m_vtx[i], &m_vtx[i], &matInv );
	
	m_vShowPos = m_vtx[8];
	m_vPos = ( m_vtx[0] + m_vtx[5] ) / 2.0f;

    D3DXPlaneFromPoints(&m_plane[3], m_vtx+2, m_vtx+6, m_vtx+7);
	D3DXPlaneFromPoints(&m_plane[4], m_vtx  , m_vtx+3, m_vtx+7);
	D3DXPlaneFromPoints(&m_plane[5], m_vtx+1, m_vtx+5, m_vtx+6);
}

BOOL CAdvObject::Pick( D3DXVECTOR3 *pPosition, CONST D3DXVECTOR3 * pMin, CONST D3DXVECTOR3 * pMax )
{
	//�������ߵ�ԭ��ͷ���
	//����ʰȡ������ر�������
	POINT        ptCursor;  //���λ��
	D3DXMATRIX   matWorld, m;
	D3DXVECTOR3  vPickRayOrig, vPickRayDir;

	//��ȡ��ǰ����ڴ��ڿͻ����е�λ��
	GetCursorPos( &ptCursor );
	ScreenToClient( GetActiveWindow(), &ptCursor );

	//��ô��ڴ�С
	D3DDEVICE_CREATION_PARAMETERS dcp;
	m_pd3dDevice->GetCreationParameters( &dcp );
	HWND hWnd = dcp.hFocusWindow;
	RECT rect;
	GetWindowRect(hWnd,&rect); 
	iWidth = rect.right;
	iHeight = rect.bottom;

	//��������۲����������
	D3DXMatrixIdentity( &matWorld );	//һ��Ҫ�ȵ�λ�����󣬷ǳ���Ҫ�����򽫻��д���
	matWorld._11 = 1.0f;
	matWorld._12 = 0.0f;
	matWorld._13 = 0.0f;
	matWorld._21 = 0.0f;
	matWorld._22 = 1.0f;
	matWorld._23 = 0.0f;
	matWorld._31 = 0.0f;
	matWorld._32 = 0.0f;
	matWorld._33 = 1.0f;
	matWorld._41 = pPosition->x;
	matWorld._42 = pPosition->y;
	matWorld._43 = pPosition->z;

	//��������۲����������
	D3DXMATRIX mWorldView = matWorld * m_matView;
	D3DXMatrixInverse( &m, NULL, &mWorldView );
	
	//����ʰȡ���ߵķ�����ԭ��
	D3DXVECTOR3 vTemp;
	vTemp.x =  ((( 2.0f * ptCursor.x ) / iWidth  ) - 1 ) / m_matProj._11;
	vTemp.y = -((( 2.0f * ptCursor.y ) / iHeight ) - 1 ) / m_matProj._22;
	vTemp.z =  1.0f;

	vPickRayDir.x  = vTemp.x*m._11 + vTemp.y*m._21 + vTemp.z*m._31;
	vPickRayDir.y  = vTemp.x*m._12 + vTemp.y*m._22 + vTemp.z*m._32;
	vPickRayDir.z  = vTemp.x*m._13 + vTemp.y*m._23 + vTemp.z*m._33;

	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;
	
	BOOL b_hit = D3DXBoxBoundProbe( pMin, pMax, &vPickRayOrig, &vPickRayDir );

	return b_hit;
}

void CAdvObject::OutputToFile( char* pathName )
{
	ofstream out;
	out.open( "log.ddf",ios::binary,0 );
	//����������ʾ��Ϣ
	for( list<AdvObjectDescription*>::iterator ii = m_AdvObjectContainer.begin(); ii != m_AdvObjectContainer.end(); ii++ )
	{
		BaseDescription *tmp_pAdvObjectDescription = ((BaseDescription*)(*ii));
		out.write( (char*)tmp_pAdvObjectDescription, sizeof( BaseDescription ));
	}
	out.close();

	//��������
	OutputToFileSub( pathName );
	return;
}


HRESULT CAdvObject::LoadDDF( char* FileName )
{
	HRESULT hr = S_OK;


	//���������ļ�
	ifstream in;
	in.open( FileName,ios::binary, 1 );
	int num = 0;
	in.read( (char*)&num, 4);
	
	if( num == 0 )
	{
		return E_FAIL;
	}

	int size = sizeof(BaseAdvObjectDescription);
	for( int i = 0; i < num; i++ )
	{
		AdvObjectDescription* tmp = new AdvObjectDescription();
		in.read( (char*)tmp, size );
		tmp->b_isPack = false;
		tmp->e_time = 0;
		DWORD id = tmp->AdvObjectDescriptionID;
		if( id > m_MaxObjectID )
		{
			m_MaxObjectID = id;
		}
		m_AdvObjectContainer.push_front(tmp);
	}
	in.close();

	return hr;
}

HRESULT CAdvObject::UnloadDDF()
{
	HRESULT hr = S_OK;

	//�ͷŹ���������
	for( list<AdvObjectDescription*>::iterator ii = m_AdvObjectContainer.begin(); ii != m_AdvObjectContainer.end(); ii++ )
	{
		delete ((AdvObjectDescription*)(*ii));
	}
	m_AdvObjectContainer.clear();

	m_MaxObjectID = 0;


	return hr;
}

D3DXVECTOR3 CAdvObject::GetViewPos()
{
	return m_vShowPos;
}

//----------------------------------------------------------------------------------------------------
//���ߵ�ʵ��
CAdvObjectTool::CAdvObjectTool():CAdvObject()
{
	//g_pErrorLog->Log("������CAdvObjectTool����");
	m_pCurrentMesh = NULL;
	
	//��ʼ�����ID�����⣩
	m_MaxTexID		= 0;
	m_MaxMeshID		= 0;
}

CAdvObjectTool::~CAdvObjectTool()
{
	//g_pErrorLog->Log("������CAdvObjectTool����");
}

HRESULT CAdvObjectTool::InitSub()
{
	//��ʼ������
	ZeroMemory( &m_WhiteMaterial, sizeof(m_WhiteMaterial) );
	m_WhiteMaterial.Ambient.a = m_WhiteMaterial.Diffuse.a = 1.0f;
	m_WhiteMaterial.Ambient.r = m_WhiteMaterial.Diffuse.r = 1.0f;
	m_WhiteMaterial.Ambient.g = m_WhiteMaterial.Diffuse.g = 1.0f;
	m_WhiteMaterial.Ambient.b = m_WhiteMaterial.Diffuse.b = 1.0f;

	ZeroMemory( &m_RedMaterial, sizeof(m_RedMaterial) );
	m_RedMaterial.Ambient.a = m_RedMaterial.Diffuse.a = 1.0f;
	m_RedMaterial.Ambient.r = m_RedMaterial.Diffuse.r = 1.0f;
	m_RedMaterial.Ambient.g = m_RedMaterial.Diffuse.g = 0.0f;
	m_RedMaterial.Ambient.b = m_RedMaterial.Diffuse.b = 0.0f;

	//��ʼ����ײ��
	if( FAILED( D3DXCreateBox( m_pd3dDevice, 1.0f, 1.0f, 1.0f, &m_pCurrentMesh, NULL ) ) )
	{
		m_pCurrentMesh = NULL;
	}
	x = 0, y = 0, z = 0, s_x = 0.2f, s_y = 0.2f, s_z = 0.2f;

	//ģʽ�趨
	m_bIsDesignMode = true;

	//	��ǰ����ΪNULL
	m_pAdvObjectDescription = NULL;

	return S_OK;
}

HRESULT CAdvObjectTool::CreateBoundBox()
{
	//�����û�ж���ѡ��
	if( m_pPackObject != NULL )
	{
		m_pPackObject->b_isPack = false;
	}

	//���´���һ�����󣬲�����Ϊѡ��״̬
	AdvObjectDescription *tmp_pAdvObjectDescription = new AdvObjectDescription();
	if( tmp_pAdvObjectDescription == NULL )
	{
		return E_FAIL;
	}

	D3DXVECTOR3 min, max;	//��ײ�еĶԽǵ㼰��������
	min.x = -0.1f;
	min.y = -0.1f;
	min.z = -0.1f;
	max.x = 0.1f;
	max.y = 0.1f;
	max.z = 0.1f;

	tmp_pAdvObjectDescription->CenterPoint  = m_vShowPos;
	tmp_pAdvObjectDescription->MaxPoint		= max;
	tmp_pAdvObjectDescription->MinPoint	    = min;
	tmp_pAdvObjectDescription->AdvObjectDescriptionID = m_MaxObjectID;

	m_MaxObjectID++;

	m_AdvObjectContainer.push_front( tmp_pAdvObjectDescription );
	m_Iterator		= m_AdvObjectContainer.begin();
	m_pPackObject	= tmp_pAdvObjectDescription;  //��ʰȡ�ϵĶ���

	return S_OK;
}

void CAdvObjectTool::ModifyBoundBox()
{
	if( m_pAdvObjectDescription == NULL )
	{
		return;	
	}

	//���㰴��״̬
	//--------------------------------------------------------------	
	//�ƶ�����
	if( GetIGA3DModule()->InputState[b_UP] )
	{
		
		if( GetIGA3DModule()->InputState[b_SHIFT] )
		{
			y += m_OffsetTran;
		}
		else
		{
			
			z += m_OffsetTran;
		}
	}
	if( GetIGA3DModule()->InputState[b_DOWN] )
	{
		if( GetIGA3DModule()->InputState[b_SHIFT] )
		{
			y -= m_OffsetTran;
		}
		else
		{
			z -= m_OffsetTran;
		}
	}
	if( GetIGA3DModule()->InputState[b_LEFT] )
	{
		x -= m_OffsetTran;
	}
	if( GetIGA3DModule()->InputState[b_RIGHT] )
	{
		x += m_OffsetTran;
	}

	//�޸ĳߴ�
	if( GetTickCount()%50 == 0 )
	{
		//���ȵ��޸�
		if( GetIGA3DModule()->InputState[b_L] )		
		{
			if( GetIGA3DModule()->InputState[b_ADD] )//"+"
			{
				s_x += m_OffsetScal;
			}
			else if( GetIGA3DModule()->InputState[b_SUB] )//"-"
			{
				s_x -= m_OffsetScal;
			}
		}

		//��ȵ��޸�
		if( GetIGA3DModule()->InputState[b_H] )		
		{
			if( GetIGA3DModule()->InputState[b_ADD] )//"+"
			{
				s_y += m_OffsetScal;
			}
			else if( GetIGA3DModule()->InputState[b_SUB] )//"-"
			{
				s_y -= m_OffsetScal;
			}
		}
		//�߶ȵ��޸�
		if( GetIGA3DModule()->InputState[b_W] )	
		{
			if( GetIGA3DModule()->InputState[b_ADD] )//"+"
			{
				s_z += m_OffsetScal;
			}
			else if( GetIGA3DModule()->InputState[b_SUB] )//"-"
			{
				s_z -= m_OffsetScal;
			}
		}

		//��ת
		if( GetIGA3DModule()->InputState[b_R] )
		{
			if( GetIGA3DModule()->InputState[b_ADD] )//"+"
			{
				r_y += m_OffsetRot;
			}
			else if( GetIGA3DModule()->InputState[b_SUB] )//"-"
			{
				r_y -= m_OffsetRot;
			}
		}
	}

	//�Ƿ�
	static bool tmp_isActive = true;
	if( GetIGA3DModule()->InputState[b_A] )
	{
		if( tmp_isActive )
		{
			tmp_isActive = false;
			m_pAdvObjectDescription->is_Active = !(m_pAdvObjectDescription->is_Active);
		}
	}
	else
	{
		tmp_isActive = true;
	}

	//�޸�ģ����Ϣ
	static bool tmp_change = true;
	if( GetIGA3DModule()->InputState[b_MAX] )
	{
		if( tmp_change )
		{
			tmp_change = false;
			if( GetIGA3DModule()->InputState[b_M] )
			{
				m_CurrentMeshID++;

				if( m_CurrentMeshID >= m_MaxMeshID )
				{
					m_CurrentMeshID = m_MaxMeshID - 1;
				}
			}
			else if( GetIGA3DModule()->InputState[b_T] )
			{
				m_CurrentTexID++;
				if( m_CurrentTexID >= m_MaxTexID )
				{
					m_CurrentTexID = m_MaxTexID - 1;
				}
			}
		}
	}
	else if( GetIGA3DModule()->InputState[b_MIN] )
	{
		if( tmp_change )
		{
			tmp_change = false;
			if( GetIGA3DModule()->InputState[b_M] )
			{
				m_CurrentMeshID--;
				if( m_CurrentMeshID < -1 )
				{
					m_CurrentMeshID = -1;
				}
			}
			else if( GetIGA3DModule()->InputState[b_T] )
			{
				m_CurrentTexID--;
				if( m_CurrentTexID < -1 )
				{
					m_CurrentTexID = -1;
				}
			}
		}
	}
	else
	{
		tmp_change = true;
	}
	//-----------------------------------------------------------
	//�޸���ײ������
	D3DXVECTOR3 min, max, center;	//��ײ�еĶԽǵ㼰��������

	min.x = -1 * s_x/2;
	min.y = -1 * s_y/2;
	min.z = -1 * s_z/2;

	max.x = 1 * s_x/2;
	max.y = 1 * s_y/2;
	max.z = 1 * s_z/2;

	center.x = x;
	center.y = y;
	center.z = z;

	m_pAdvObjectDescription->CenterPoint = center;
	m_pAdvObjectDescription->MinPoint	 = min;
	m_pAdvObjectDescription->MaxPoint	 = max;
	m_pAdvObjectDescription->RotationY   = r_y;

	m_pAdvObjectDescription->mesh_id	= m_CurrentMeshID;
	m_pAdvObjectDescription->texture_id = m_CurrentTexID;
}

void CAdvObjectTool::RenderSub( AdvObjectDescription *pAdvObjectDescription )
{
	//---------------------------------------------------------
	//�ж��Ƿ���ʾ
	if( m_bIsDesignMode == false )//�������ʾ���Ƴ����ģʽ
	{
		return;
	}
	
	//----------------------------------------------------------
	//��ʾ�Ļ��ͽ������ģʽ
	//����һ����������Ϊδѡ��״̬
	m_pAdvObjectDescription = pAdvObjectDescription;
	
	//����������Ϣ
	x = m_pAdvObjectDescription->CenterPoint.x;
	y = m_pAdvObjectDescription->CenterPoint.y;
	z = m_pAdvObjectDescription->CenterPoint.z;

	D3DXVECTOR3 tmp = ( m_pAdvObjectDescription->MaxPoint ) - ( m_pAdvObjectDescription->MinPoint );
	s_x = tmp.x;
	s_y = tmp.y;
	s_z = tmp.z;
	r_y = m_pAdvObjectDescription->RotationY;

	m_CurrentMeshID = m_pAdvObjectDescription->mesh_id;
	m_CurrentTexID  = m_pAdvObjectDescription->texture_id;


	//�жϵ�ǰ����ײ���Ƿ���Բ���
	if( m_pAdvObjectDescription->b_isPack )	//����ѡ�е���ײ����ʾ��ɫ���ʣ����ҿ����޸�
	{
		//������ײ����Ϣ
		ModifyBoundBox();
		m_pd3dDevice->SetMaterial( &m_RedMaterial);
	}
	else	//������ʾ��ɫ����
	{
		m_pd3dDevice->SetMaterial( &m_WhiteMaterial);
	}

	////������װ��
	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	D3DXMATRIX tmp_matWorld;
	D3DXMatrixIdentity( &tmp_matWorld );
	tmp_matWorld._11 = s_x;
	tmp_matWorld._22 = s_y;
	tmp_matWorld._33 = s_z;
	tmp_matWorld._41 = x;
	tmp_matWorld._42 = y;
	tmp_matWorld._43 = z;

	D3DXMatrixMultiply( &tmp_matWorld, &m_MatrixTranslate, &tmp_matWorld );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &tmp_matWorld);
	m_pd3dDevice->SetTexture( 0, NULL );
	m_pCurrentMesh->DrawSubset(0);
	if( m_pAdvObjectDescription->b_isPack )
	{
		D3DXMATRIX tmp_mat;
		D3DXMatrixIdentity( &tmp_mat );
		tmp_mat._41 = x;
		tmp_mat._42 = y + s_y*0.8f;
		tmp_mat._43 = z;
		D3DXMatrixMultiply( &tmp_mat, &_m_MatrixTranslate, &tmp_mat );
		D3DXMatrixMultiply( &tmp_mat, &m_MatrixTranslate, &tmp_mat );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &tmp_mat);
		m_pCurrentMesh->DrawSubset(0);
	}
	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
}

void CAdvObjectTool::ReleaseSub()
{
	if( m_pCurrentMesh != NULL )
	{
		m_pCurrentMesh->Release();
	}
}

void CAdvObjectTool::KeyProcessor()
{
	if( GetIGA3DModule()->InputState[b_F8] ) m_bIsDesignMode = true;
	if( GetIGA3DModule()->InputState[b_F9] ) m_bIsDesignMode = false;
	if( m_bIsDesignMode == false )	//�������ģʽ�£�����������Ч
	{
		//if( m_pPackObject != NULL )	//�൱��ȡ��ESC
		//{
		//	m_pPackObject->b_isPack = false;
		//	m_pPackObject = NULL;
		//	m_Iterator = NULL;
		//}
		return;
	}

	//---------------------------------------------------------------
	//ȡ����ѡ�е���װ��
	if( GetIGA3DModule()->InputState[b_ESC] )
	{
		if( m_pPackObject != NULL )
		{
			m_pPackObject->b_isPack = false;
			m_pPackObject = NULL;
			m_Iterator = NULL;
			OutputDebugString("ȡ����һ�����������ѡ��");
			m_SelectID = 0xffffffff; //��ʾû�ж���ʰȡ��
		}
	}

	//ɾ��ѡ�еĽڵ�
	if( GetIGA3DModule()->InputState[b_DELETE] )
	{
		if( m_pPackObject != NULL )
		{
			delete m_pPackObject;
			m_pPackObject = NULL;
			if( m_Iterator != NULL )
			{
				m_AdvObjectContainer.erase( m_Iterator );
				m_Iterator = NULL;
			}
			OutputDebugString("ɾ����һ����������");
		}
	}

	//�����������ݵ������ļ�
	static bool tmp_b_output = true;
	if( GetIGA3DModule()->InputState[b_CTRL] && GetIGA3DModule()->InputState[b_S] )
	{
		if( tmp_b_output )
		{
			OutputToFileSub( "DescriptionMessage.txt" );
			tmp_b_output = false;
		}
	}
	else
	{
		tmp_b_output = true;
	}

	//Ѱ�ҽڵ�
	static bool tmp_b_find = true;
	if( GetIGA3DModule()->InputState[b_CTRL] && GetIGA3DModule()->InputState[b_N] )
	{
		static list<AdvObjectDescription*>::iterator ii = m_AdvObjectContainer.begin();
		if( tmp_b_find )
		{
			tmp_b_find = false;

			if( ii == m_AdvObjectContainer.end() )
			{
				ii = m_AdvObjectContainer.begin();
			}

			AdvObjectDescription *tmp_pAdvObjectDescription = ((AdvObjectDescription*)(*ii));
			tmp_pAdvObjectDescription->b_isPack = true;

			m_SelectID = tmp_pAdvObjectDescription->AdvObjectDescriptionID;
			if( m_pPackObject != NULL )
			{
				m_pPackObject->b_isPack = false;
			}
			m_pPackObject = tmp_pAdvObjectDescription;
			m_Iterator    = ii;

			ii++;
		}
	}
	else
	{
		tmp_b_find = true;
	}

	//--------------------------------------------------------------
	//����һ����ײ�ж�������
	static tmp_b_create = true;
	if( GetIGA3DModule()->InputState[b_F2] )
	{
		if( tmp_b_create )
		{
			if( SUCCEEDED( CreateBoundBox() ) )
			{
				OutputDebugString("������һ����������");
			}
			tmp_b_create = false;
		}
	}
	else
	{
		tmp_b_create = true;
	}
	
}

void CAdvObjectTool::OutputToFileSub( char* pathName )
{
	ofstream out, out1;
	out.open( pathName );
	out1.open("data.ddf",ios::binary,0);
	int num = (int)m_AdvObjectContainer.size();
	out1.write( (char*)(&num), 4 );
	out<<"�ܹ��У�"<<num<<"����ײ��"<<endl;
	for( list<AdvObjectDescription*>::iterator ii = m_AdvObjectContainer.begin(); ii != m_AdvObjectContainer.end(); ii++ )
	{
		AdvObjectDescription *tmp_pAdvObjectDescription = ((AdvObjectDescription*)(*ii));
		if( tmp_pAdvObjectDescription->is_Show )
		{
			tmp_pAdvObjectDescription->is_Show = false;
			long tmp_time = GetTickCount() - tmp_pAdvObjectDescription->time;
			tmp_pAdvObjectDescription->e_time += tmp_time;
		}

		DWORD		id		= tmp_pAdvObjectDescription->AdvObjectDescriptionID;
		int			num		= tmp_pAdvObjectDescription->num_Pack;
		D3DXVECTOR3 c_p		= tmp_pAdvObjectDescription->CenterPoint;
		D3DXVECTOR3 max_p	= tmp_pAdvObjectDescription->MaxPoint;
		D3DXVECTOR3 min_p	= tmp_pAdvObjectDescription->MinPoint;
		DWORD		time    = tmp_pAdvObjectDescription->e_time;

		out<<"id: "<<id<<endl;
		out<<"num_Pack: "<<num<<endl;
		out<<"time: "<<time/1000<<"s  "<<endl;
		out<<"CenterPoint: "<<c_p.x<<" "<<c_p.y<<" "<<c_p.z<<endl;
		out<<"MaxPoint: "<<max_p.x<<" "<<max_p.y<<" "<<max_p.z<<endl;
		out<<"MinPoint: "<<min_p.x<<" "<<min_p.y<<" "<<min_p.z<<endl<<endl;
		
		out1.write( (char*)(BaseAdvObjectDescription*)tmp_pAdvObjectDescription, sizeof(BaseAdvObjectDescription) );

	}
	out1.close();
	out.close();
}






