/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : MeshLibrary.h
*	����������IGA3D, modify by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#pragma  once 
#include "d3d9.h"
#include "d3dx9.h"

#include <map>
#include <set>
#include <list>
using namespace std;

#include "FxAnimationInstance.h"
//---------------------------------------
//�����
class CMeshLibrary
{
public:
	CMeshLibrary();
	~CMeshLibrary();

public:
	HRESULT Init( LPDIRECT3DDEVICE9 pd3dDevice );
	void	Lost();	//�豸��ʧ
	void	Resume();	//�豸�ָ�
	void	Render( DWORD dwID, D3DXMATRIX* pmatWorld, LPDIRECT3DTEXTURE9 pAdvTex, char* pchURL = NULL );	//�����������ָ����ͼ��
	void	Destory();	//�ͷ���Դ
	void    ClearError(){ m_setErrorID.clear();};

	void	AddResource( DWORD id );	//����ID�����Դ
	void	ReMove( DWORD id );			//����ID�Ƴ���Դ
	

private:
	IDirect3DDevice9*	m_pd3dDevice;
	FxAnimInstance *	m_pAnimInstance;

	typedef pair<DWORD, FxAnimInstance *>		MAP_MESH_NODE;
	map<DWORD, FxAnimInstance *>			m_mapMeshLibrary;	//�������������
	map<DWORD, FxAnimInstance*>::iterator	m_mapIterator;

//�豸��ʧ���ָ�ʱʹ��
	list<DWORD>				m_listMeshIDLibrary;	//���豸��ʧʱ���浱ǰʹ�õ�������Ϣ��ID,���豸�ָ���ʱ�������¼�������
	list<DWORD>::iterator	m_listIterator;

	set<DWORD>				m_setErrorID;      //����Ķ���������¼����ʱʧ�ܵ���ԴID,������ʧ�ܵ�IDʱ�򣬾�������ִ�м��ز���
	set<DWORD>::iterator	m_setIterator;	

public:
	long		m_lRefCount;

};

extern CMeshLibrary g_CMeshLibrary;

//CMeshLibrary* GetMeshLibrary();


