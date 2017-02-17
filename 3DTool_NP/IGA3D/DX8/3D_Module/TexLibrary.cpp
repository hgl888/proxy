#include "stdafx.h"

CTexLibrary* GetTexLibrary()
{
	static CTexLibrary TexLib;
	return &TexLib;
}

//---------------------------------------------
//�������ʵ��
CTexture::CTexture()
{
	m_pTexture = NULL;
	m_pd3dDevice = NULL;
}

CTexture::~CTexture()
{
	if( m_pTexture != NULL )
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

HRESULT CTexture::Init( IDirect3DDevice8 *pd3dDevice, char* fileName, CPack* pPack  )
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;

	//�������ļ���������ģ��
	char	*tmp_data = NULL;
	DWORD	size     = 0x0;
	pPack->GetDataFromPack( fileName, tmp_data, &size );
	if( tmp_data == NULL || size == 0x0 )
	{
		if( tmp_data != NULL )
		{
			delete tmp_data;
			tmp_data = NULL;
		}
		OutputDebugString("�����ݰ��л����������ʧ�ܣ�");
		return E_FAIL;
	}

	if( FAILED( D3DXCreateTextureFromFileInMemory( m_pd3dDevice, tmp_data, size,
													&m_pTexture ) ) )
	{
		if( tmp_data != NULL )
		{
			delete tmp_data;
		}

		m_pTexture = NULL;
		OutputDebugString("���ڴ��м�����������ʧ�ܣ�");
		return E_FAIL;
	}
	delete tmp_data;
	tmp_data = NULL;


	return hr;
}

LPDIRECT3DTEXTURE8 CTexture::GetTexture()
{
	return m_pTexture;
}

//------------------------------------------------------------------------------------

CTexLibrary::CTexLibrary()
{
	m_pd3dDevice = NULL;
}

CTexLibrary::~CTexLibrary()
{
	Destory();
}


HRESULT CTexLibrary::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;

	m_TexLibrary.clear();
	m_pNode		= NULL;
	m_pTex		= NULL;

	m_TexIDLibrary.clear();
	m_pNodeID	= NULL;

	m_ErrorID.clear();
	m_pErrorNode = NULL;

	//OutputDebugString("������������Ŀ¼......");
	char path[256];
	sprintf( path, "%s%s",PATH_WORK,"adv_res\\" );

	GetAbsPath( path );

	m_Pack.Init("texture", path);
	m_Pack.LoadDataRootFromFile();	//�������ݰ�Ŀ¼

#ifdef USE_IGA3D_TOOL
	int i = 0;
	while(1)
	{
		AddResource( i );

		m_pErrorNode = m_ErrorID.find(i);
		if( m_pErrorNode != m_ErrorID.end() )
		{
			GetAdTool()->SetMaxTexID(i);
			break;
		}
		i++;	
	}
#endif

	return hr;
}

void CTexLibrary::Lost()
{
	for( m_pNode = m_TexLibrary.begin(); m_pNode != m_TexLibrary.end(); m_pNode++ )
	{
		DWORD	id = m_pNode->first;
		m_TexIDLibrary.push_back(id);

		if( m_pNode->second != NULL )
		{
			delete m_pNode->second;
			m_pNode->second = NULL;
		}
	}

	m_TexLibrary.clear();
}

void CTexLibrary::Resume()
{
	for( m_pNodeID = m_TexIDLibrary.begin(); m_pNodeID != m_TexIDLibrary.end(); m_pNodeID++ )
	{
		DWORD	id = *m_pNodeID;
		AddResource( id );	
	}
	m_TexIDLibrary.clear();
}

LPDIRECT3DTEXTURE8 CTexLibrary::GetTexture( DWORD id )
{
	//���ж��ǲ���û�е���Դ
	m_pErrorNode = m_ErrorID.find(id);
	if( m_pErrorNode != m_ErrorID.end() )
	{
		return NULL;
	}

	//�ڿ����Ƿ��ڵ�ǰ����
	m_pNode = m_TexLibrary.find( id );
	if( m_pNode == m_TexLibrary.end() )
	{
		//��Ҫ������Դ
		AddResource( id );
		return NULL;	//�������������
	}

	//�����������������
	m_pTex = m_pNode->second->GetTexture();
	
	return m_pTex;

}

void CTexLibrary::AddResource( DWORD id )
{
	//����ID����Ҫ���ص������ļ�����
	char	fileName[256];
	
	//���������Դ
	CTexture*	pTex = new CTexture();
	if( pTex == NULL )
	{
		OutputDebugString("����CMesh�Ŀռ����");
		return;
	}

	sprintf( fileName, "tex_%d.jpg", id );
	
	//��������
	HRESULT hr = pTex->Init( m_pd3dDevice, fileName, &m_Pack );
	if( FAILED(hr) )
	{
		delete pTex;
		m_ErrorID.insert(id);
		return;
	}

	//�Ѽ��ص�����ŵ�MESH����
	m_TexLibrary.insert( TEX_NODE(id, pTex) );
}

void CTexLibrary::ReMove( DWORD id )
{

}

void CTexLibrary::Destory()
{
	Lost();
	m_TexIDLibrary.clear();
	m_ErrorID.clear();
}
