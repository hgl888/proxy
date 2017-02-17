
#pragma  once

#include <map>
#include <set>
#include <list>
using namespace std;

#include "d3d8.h"
#include "d3dx8.h"

//------------------------------------------------------------------------
#include "FxSkeletalMesh.h"
#include "FxSkeletalAnim.h"

//---------------------------------------
//�����
class CMeshLibrary
{
public:
	CMeshLibrary();
	~CMeshLibrary();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Lost();	//�豸��ʧ
	void	Resume();	//�豸�ָ�
	void	Render( DWORD id, D3DXMATRIX* matWorld, LPDIRECT3DTEXTURE8 pAdvTex, char* url = NULL );	//�����������ָ����ͼ��
	void	Destory();	//�ͷ���Դ
	void    ClearError(){ m_setErrorID.clear();};

private:
	void	AddResource( DWORD id );	//����ID�����Դ
	void	ReMove( DWORD id );			//����ID�Ƴ���Դ


private:
	typedef pair<DWORD, CFxSkeletalMesh *>		MESH_NODE;
	map<DWORD, CFxSkeletalMesh *>			m_mapMeshLibrary;	//�������������
	map<DWORD, CFxSkeletalMesh *>::iterator	m_mapIterator;

	CFxSkeletalMesh *	m_pFxSkeletalMesh;
	LPDIRECT3DDEVICE8	m_pd3dDevice;

//�豸��ʧ���ָ�ʱʹ��
private:

	list<DWORD>				m_listMeshIDLibrary;	//���豸��ʧʱ���浱ǰʹ�õ�������Ϣ��ID,���豸�ָ���ʱ�������¼�������
	list<DWORD>::iterator	m_listIterator;

	set<DWORD>				m_setErrorID;
	set<DWORD>::iterator	m_setIterator;
	long					m_longRefCount;

};

extern CMeshLibrary g_CMeshLibrary;

//CMeshLibrary* GetMeshLibrary();


