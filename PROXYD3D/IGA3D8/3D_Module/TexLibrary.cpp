#include "stdafx.h"
#include "define.h"

#include "3D_Core.h"
#include "tchar.h"
#include "z_FileIni.h"
#include "TexLibrary.h"

//CTexLibrary* GetTexLibrary()
//{
//	static CTexLibrary TexLib;
//	return &TexLib;
//}

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

	m_mapTexLibrary.clear();
	//m_pNode		= NULL;
	m_pDirect3DTexture		= NULL;

	m_listTexIDLibrary.clear();
	//m_pNodeID	= NULL;

	m_setErrorID.clear();
	//m_pErrorNode = NULL;

	//OutputDebugString("������������Ŀ¼......");
	char path[256];
	sprintf( path, "%s%s",PATH_WORK,"adv_res\\" );

	GetModulePath( path );

	m_Pack.Init("texture", path);
	m_Pack.LoadDataRootFromFile();	//�������ݰ�Ŀ¼

#ifdef USE_IGA3D_TOOL
	int i = 0;
	while(1)
	{
		AddResource( i );

		m_setIterator = m_setErrorID.find(i);
		if( m_setIterator != m_setErrorID.end() )
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
	for( m_mapIterator = m_mapTexLibrary.begin(); m_mapIterator != m_mapTexLibrary.end(); m_mapIterator++ )
	{
		DWORD	id = m_mapIterator->first;
		m_listTexIDLibrary.push_back(id);

		if( m_mapIterator->second != NULL )
		{
			delete m_mapIterator->second;
			m_mapIterator->second = NULL;
		}
	}

	m_mapTexLibrary.clear();
}

void CTexLibrary::Resume()
{
	for( m_listIterator = m_listTexIDLibrary.begin(); m_listIterator != m_listTexIDLibrary.end(); m_listIterator++ )
	{
		DWORD	id = *m_listIterator;
		AddResource( id );	
	}
	m_listTexIDLibrary.clear();
}

LPDIRECT3DTEXTURE8 CTexLibrary::GetTexture( DWORD id )
{
	//���ж��ǲ���û�е���Դ
	m_setIterator = m_setErrorID.find(id);
	if( m_setIterator != m_setErrorID.end() )
	{
		return NULL;
	}

	//�ڿ����Ƿ��ڵ�ǰ����
	m_mapIterator = m_mapTexLibrary.find( id );
	if( m_mapIterator == m_mapTexLibrary.end() )
	{
		//��Ҫ������Դ
		AddResource( id );
		return NULL;	//�������������
	}

	//�����������������
	m_pDirect3DTexture = m_mapIterator->second->GetTexture();
	
	return m_pDirect3DTexture;

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
		m_setErrorID.insert(id);
		return;
	}

	//�Ѽ��ص�����ŵ�MESH����
	m_mapTexLibrary.insert( MAP_TEXTURE_NODE(id, pTex) );
}

void CTexLibrary::ReMove( DWORD id )
{

}

void CTexLibrary::Destory()
{
	Lost();
	m_listTexIDLibrary.clear();
	m_setErrorID.clear();
}
