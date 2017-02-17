
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
	void	Destory();	//�ͷ���Դ

	LPDIRECT3DTEXTURE8 GetTexture( DWORD id );


private:
	void	AddResource( DWORD id );	//����ID�����Դ
	void	ReMove( DWORD id );			//����ID�Ƴ���Դ

private:

	LPDIRECT3DDEVICE8	m_pd3dDevice;
	LPDIRECT3DTEXTURE8				m_pDirect3DTexture;
	
	
	

//�豸��ʧ���ָ�ʱʹ��
private:

	typedef pair<DWORD, CTexture*>		MAP_TEXTURE_NODE;
	map<DWORD, CTexture*>			m_mapTexLibrary;	//�������������
	map<DWORD, CTexture*>::iterator	m_mapIterator;

	list<DWORD>				m_listTexIDLibrary;	//���豸��ʧʱ���浱ǰʹ�õ�������Ϣ��ID,���豸�ָ���ʱ�������¼�������
	list<DWORD>::iterator	m_listIterator;
	set<DWORD>				m_setErrorID;		//����Ķ���������¼����ʱʧ�ܵ���ԴID,������ʧ�ܵ�IDʱ�򣬾�������ִ�м��ز���
	set<DWORD>::iterator	m_setIterator;	
	CPack					m_Pack;

};

//CTexLibrary* GetTexLibrary();