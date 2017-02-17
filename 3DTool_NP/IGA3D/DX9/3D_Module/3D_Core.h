#include "d3d9.h"
#include "d3dx9.h"
#include "stdio.h"
#include <list>
using namespace std;

#include "luaScript.h"
#include "MeshLibrary.h"
#include "TexLibrary.h"
#include "Font.h"
#include "IGA3D_UI.h"

#include "shellapi.h"

//�ص�����
typedef void(*LPIGAFUN)(int id);

class IGA3DModule;
//��ʼ��3Dģ��
DWORD IGA3DInit( LPDIRECT3DDEVICE9 pd3dDeviceOrig/*, LPIGAFUN pFun = NULL*/ );

//�ͷ�3Dģ��
void IGA3DDestory();

//��ȡ��������
void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix );

//����
void IGA3DRender();

//���ø���
void SetFristDraw();
void SetUpdate();	//���������������֪ͨϵͳ���¼�����Դ

//-------------------------------------------------------------
struct SDrawElement
{
	int		mesh_id;  //ģ��ID
	int		scene_id; //����ID
	
	float	x, y, z;
	float	s_xyz;
	float	r_x, r_y, r_z;

	char	url[1024];

	SDrawElement()
	{
		mesh_id = -1;
		scene_id = -1;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		url[0] = '\0';
		
	}

	SDrawElement( const SDrawElement& obj )
	{
		mesh_id = obj.mesh_id;
		scene_id = obj.scene_id;

		x = obj.x;
		y = obj.y;
		z = obj.z;
		s_xyz = obj.s_xyz;
		r_x  = obj.r_x;
		r_y  = obj.r_y;
		r_z  = obj.r_z;

		strcpy( url, obj.url );
		
	}
};

//class ZLock
//{
//public:
//	 ZLock()  { ::InitializeCriticalSection(&m_cr); };
//	 ~ZLock() { ::DeleteCriticalSection(&m_cr);};
//	 Lock()   { ::EnterCriticalSection(&m_cr);};
//	 UnLock() { ::LeaveCriticalSection(&m_cr);};
//private:
//	 CRITICAL_SECTION    m_cr;
//};

class IGA3DModule
{
public:
	IGA3DModule(){};
	~IGA3DModule(){};

public:
	int Init();
	void Destory();

	void PerIGA3DRender();
	void OnIGA3DRender();
	void PostIGA3DRender();

	void IGA3DInputState();
	void RunScript();

	void ComputeSceneID();

	void ReLoad();

//--------------------------------------
//2008-7-24��ӣ�
//�߼���Դ: �������汻ʰȡ�ϵĶ���ID��
public:
	int		m_PickID;	//��ʰȡ��ģ�͵�ID,���С��0��ʾû��ʰȡ���κ�3D����

	SIGA3DMSG	m_Msg;	//��ǰ��Ϣ
	bool		m_bGetMsg;	//

	//�Ƿ���ʾģ��
	bool		m_bShow;

	//����ϵͳ: 2008-8-12
	bool		m_bUpdate;


//-------------------------------------
//ϵͳ��Դ
public:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	D3DXMATRIX			m_matView, m_matProj, m_matWorld;
	bool				m_bInitView, m_bInitProj, m_bInitWprld;

	IDirect3DStateBlock9 *m_pkOriginalStateBlock;
	IDirect3DStateBlock9 *m_pkProxyStateBlock;

	IDirect3DBaseTexture9 *pkTexStage0;
	DWORD ulFVF, ulLighting, ulCullMode, ulZEnable, ulVertexBlend, ulTexStage0ColorOp, ulTexStage0ColorArg1, ulTexStage0ColorArg2;


	SDrawElement		m_DrawElement;
	bool				m_bIsLoad;		//ģ���Ƿ���سɹ�

//3D����״̬
public:
	DWORD					m_Lighting, 
							m_CullMode, 
							m_ZEnable, 
							m_Alpha,
							m_TexStage0ColorOp,		//*
							m_TexStage0ColorArg1,	//*
							m_TexStage0ColorArg2;	//*

	IDirect3DBaseTexture9*	m_pTexStage0;	//*

	bool InputState[128];	//����״̬

	D3DXMATRIX				m_mat[100];
	
public:
	BOOL		m_bFrist;
	long		m_ElapseTime;	//��ǰ��ͬ����ʱ��
	long		m_Start;	


	DWORD		m_State;

	int			m_DrawPos;	//��Core����Ƶ�λ��

//private:	
//	ZLock       m_lock;

//2008-8-5���
private:
	typedef  list<SDrawElement>		_DE;
	typedef  list<SDrawElement>::iterator _DE_NODE;

	_DE			m_DrawLibrary;
	_DE_NODE	m_pDrawNode;

	int			m_CurrentScene;

public:
	LPIGAFUN			m_pFun;

};

IGA3DModule* GetIGA3DModule();

enum 
{
	b_ESC, b_F1, b_F2, b_F3, b_F4, b_F5, b_F6, b_F7, b_F8, b_F9, b_F10, b_F11, b_F12, //���ܼ�
	b_L, b_W, b_H, b_S, b_R, b_T, b_M, b_A, b_N,//��ĸ��
	b_UP, b_DOWN, b_LEFT, b_RIGHT, //�����
	b_LBUTTON, b_RBUTTON, //������
	b_CTRL, b_SHIFT, //CTRL��SHIFT��
	b_ADD, b_SUB, //��+������-����
	b_DELETE, //DELETE��
	b_MAX, b_MIN, //��,������.����<������>����
};


enum RenderPos
{
	Per_SetTransform,	//���þ���ǰ
	Post_SetTransform,	//���þ����
	Per_Present,		//��תǰ
	Post_Present		//��ת��
};

#define PATH_WORK "InterAdtive\\EMMOA\\3D\\"

void GetAbsPath( char fullPathName[256] );

//----------------------------------
//�ű�����
int CGetState( lua_State *L );
int CSetState( lua_State *L );

void DrawMesh( lua_State *L );
void DrawText( lua_State *L );

void DrawDlg( lua_State *L );

void OpenUrl( lua_State *L );

//��ȡ��Ӱ��λ��
void GetCameraPos();


//--------------------------------------------
//��Ϸ�б�
