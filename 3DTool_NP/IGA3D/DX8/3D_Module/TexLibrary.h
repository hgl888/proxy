
#include "d3d8.h"
#include "d3dx8.h"
#include "pack.h"
//----------------------------------------------------------------------------------------
//������
class CTexture
{
public:
	CTexture();
	~CTexture();

public:
	HRESULT Init( IDirect3DDevice8 *pd3dDevice, char* fileName, CPack* pPack  );

	LPDIRECT3DTEXTURE8 GetTexture();

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	LPDIRECT3DTEXTURE8	m_pTexture;

};

//--------------------------------------------------------------------
//�����
class CTexLibrary
{
public:
	CTexLibrary();
	~CTexLibrary();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Lost();	//�豸��ʧ
	void	Resume();	//�豸�ָ�

	LPDIRECT3DTEXTURE8 GetTexture( DWORD id );

	void	Destory();	//�ͷ���Դ

private:
	void	AddResource( DWORD id );	//����ID�����Դ
	void	ReMove( DWORD id );			//����ID�Ƴ���Դ

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	typedef map<DWORD, CTexture*>		TEX_LIBRARY;
	typedef pair<DWORD, CTexture*>		TEX_NODE;

	TEX_LIBRARY						m_TexLibrary;	//�������������
	map<DWORD, CTexture*>::iterator	m_pNode;

	LPDIRECT3DTEXTURE8				m_pTex;

//�豸��ʧ���ָ�ʱʹ��
private:
	typedef list<DWORD>		TEX_ID;
	TEX_ID					m_TexIDLibrary;	//���豸��ʧʱ���浱ǰʹ�õ�������Ϣ��ID
												//���豸�ָ���ʱ�������¼�������

	list<DWORD>::iterator	m_pNodeID;

//����Ķ���������¼����ʱʧ�ܵ���ԴID
//������ʧ�ܵ�IDʱ�򣬾�������ִ�м��ز���
private:
	typedef set<DWORD>			TEX_ERROR_ID;
	TEX_ERROR_ID				m_ErrorID;

	set<DWORD>::iterator		m_pErrorNode;	

private:
	CPack						m_Pack;

};

CTexLibrary* GetTexLibrary();