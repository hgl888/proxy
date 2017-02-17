#pragma  once 

#include "d3d8.h"
#include "d3dx8.h"
#include <map>
#include <list>
#include <set>
#include "pack.h"
using namespace std;
//ע�⣺��ԴҪ�������ڴ���
//-----------------------------------------------
class CFace
{
public:
	CFace();
	~CFace();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );

	HRESULT SetPosition( int x = 0, int y = 0, int size_x = 100, int size_y = 100 );
	void	SetTexture( LPDIRECT3DTEXTURE8 pTexture );

	void Render();

	void Destory();

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	struct _vf
	{
		float x,y,z,w;
		float u,v;
	};
	#define D3D_FVF ( D3DFVF_XYZRHW|D3DFVF_TEX1 ) 

	LPDIRECT3DVERTEXBUFFER8		m_pVB;
	LPDIRECT3DTEXTURE8			m_pTex;
	D3DMATERIAL8				m_Material;



};

CFace* GetCFace();

//--------------------------------------------------------
//UI�������
class CUITexture
{
public:
	CUITexture();
	~CUITexture();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );

	LPDIRECT3DTEXTURE8 GetTexture( DWORD id );

	

	void	Lost();	//�豸��ʧ
	void	Resume();	//�豸�ָ�

//---------------------------------------------------
//��ӻ��Ƴ�
private:
	void AddResource( DWORD id );
	void ReMove( DWORD id );	//����ID�Ƴ���Դ
	void Destory();

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;
	LPDIRECT3DTEXTURE8	m_pTex;

//--------------------------------------------------
//����� 
private:
	typedef map<DWORD, LPDIRECT3DTEXTURE8>	UI_TEXTURE;
	typedef pair<DWORD, LPDIRECT3DTEXTURE8> UI_TEX_NODE;

	UI_TEXTURE		m_UITexLibrary;	//�����
	map<DWORD, LPDIRECT3DTEXTURE8>::iterator	m_pNode;

//--------------------------------------------------
//�豸��ʧ���ָ�ʱʹ��
private:
	typedef list<DWORD>		TEX_ID;
	TEX_ID					m_TexIDLibrary;

	list<DWORD>::iterator	m_pTexID;

//--------------------------------------------------
//����Ķ���������¼����ʱʧ�ܵ���ԴID
//������ʧ�ܵ�IDʱ�򣬾�������ִ�м��ز���
private:
	typedef set<DWORD>		TEX_ERROR_ID;
	TEX_ERROR_ID			m_ErrorID;

	set<DWORD>::iterator	m_pErrorNode;

private:
	CPack					m_Pack;
};

CUITexture* GetUITexture();

//-------------------------------------------------------------------
//UI
class CUI
{
public:
	CUI();
	~CUI();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );

	void Render();
	
	void	Lost();	//�豸��ʧ
	void	Resume();	//�豸�ָ�

private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

};

CUI* GetUI();

//--------------------------------------------------------------------
//UI�Ի���
class CUIDialog
{
public:
	CUIDialog();
	~CUIDialog();

public:
	DWORD Render( POINT* pPoint, DWORD id );	//���ƶԻ���

private:
	POINT	m_Point;	//��ǰ����λ�ã������ж϶Ի����¼�

};

DWORD ShowDlg( POINT* pPoint, DWORD id );