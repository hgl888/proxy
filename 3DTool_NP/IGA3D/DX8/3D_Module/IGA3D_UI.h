#include "d3d8.h"
#include "d3dx8.h"

//UI��Ƭ
class CIGA3D_UI_Element
{
public:
	CIGA3D_UI_Element();
	~CIGA3D_UI_Element();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Render();
	void	Destory();

//��������
public:
	//���ô�С
	HRESULT SetSize( int width, int height );

	//����λ��
	HRESULT SetPosition( int x, int y );

	//����ʹ�õ�����
	void SetTexture( LPDIRECT3DTEXTURE8 pTex );

	bool IsInSide( int x, int y );


public:
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER8	m_pVB;			//ʹ�õ��ı���
	LPDIRECT3DTEXTURE8		m_pTex;			//ʹ�õ�����

	int						m_x,m_y;
	int						m_width,m_height;

	struct _VUI
	{
		float x,y,z,rhw;
		DWORD color;
		float u,v;
	};
#define _D3DFVF_VUI ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )

};

CIGA3D_UI_Element* GetUI();

//------------------------------------------------------
//UI����
class CUI_Tex
{
public:
	CUI_Tex();
	~CUI_Tex();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );

	LPDIRECT3DTEXTURE8 GetTexture( int id );

	void Destory();

private:
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	LPDIRECT3DTEXTURE8      m_pTex;
	LPDIRECT3DTEXTURE8      m_pTex1;
    LPDIRECT3DTEXTURE8      m_pTex2;

	LPDIRECT3DTEXTURE8      m_pTex3;
    LPDIRECT3DTEXTURE8      m_pTex4;
};

CUI_Tex* GetUITexture();

//---------------------------------------------
//��ť����
typedef void(*LPFUN)();
class CButtom
{
public:
	CButtom();
	~CButtom();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice, LPFUN pFun = 0 );
	
	void	Draw( int x, int y );	//���λ��

	void	Destory();

public:
	void	SetPosition( int x, int y );
	void	SetTextureID( int tex_id = 1 );
	void	SetProcFun( LPFUN pFun );	//���ð�ť���¼�������

private:
	LPDIRECT3DDEVICE8		m_pd3dDevice;

	CIGA3D_UI_Element		m_Buttom;		//��ťͼ�ζ���

	LPFUN					m_pProcFun;		//��ť�������Ĵ�����

private:
	int						m_TexID;	
};

void CallBackProcFun();
void CallBackProcFun1();


//-----------------------------------------------------
//
struct _UIElement
{
	DWORD	id;
	int		tex_id;	//ʹ�õ�����ID
	RECT	rect;
	
	_UIElement*  pFirstChildNode; //�ӽڵ�
	_UIElement*  pSiblingNode;    //�ֵܽڵ�

	_UIElement()
	{
		id		= 0;
		tex_id	= 0;
		rect.top		= 0;
		rect.left		= 0;
		rect.right		= 30;
		rect.bottom		= 50;

		pFirstChildNode = NULL;
		pSiblingNode	= NULL;
	}

};

class IGA3DDialog
{
public:
	IGA3DDialog();
	~IGA3DDialog();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void    Render();
	void	Destory();

private:
	LPDIRECT3DDEVICE8		m_pd3dDevice;

	_UIElement*				m_pFrame;	//�Ի���������

	CIGA3D_UI_Element		m_Frame;	//���
	
	CButtom					m_ButtomOK;	//OK��ť
	CButtom					m_ButtomCancel;	//OK��ť

};
IGA3DDialog* GetDialog();

//------------------------------------------------
//��Ϣ�¼�
#include <queue>
#include <functional>

enum IGA3D_MSG
{
	IGA3D_MSG_UNKNOW = -1,
	IGA3D_MSG_LBUTTONDOWN,
	IGA3D_MSG_LBUTTONUP,
	IGA3D_MSG_RBUTTONDOWN,
	IGA3D_MSG_RBUTTONUP,

};



struct SIGA3DMSG
{
	bool	bIsProc;	//�Ƿ񱻴����
	DWORD	type;		//��Ϣ������
	int     key;	    //��ֵ

	int		x;		//����λ��
	int		y;

	SIGA3DMSG()
	{
		bIsProc = false;
		type	= IGA3D_MSG_UNKNOW;
		key		= IGA3D_MSG_UNKNOW;
		x		= 0;
		y       = 0;
	}

	SIGA3DMSG( const SIGA3DMSG& obj )
	{
		bIsProc = obj.bIsProc;
		type	= obj.type;
		key		= obj.key;
		x		= obj.x;
		y       = obj.y;
	}

	~SIGA3DMSG()
	{
	
	}


};


class CIGA3DMessageManager
{
public:
	CIGA3DMessageManager();
	~CIGA3DMessageManager();

public:
	HRESULT Init();
	void    Destory();

	void	UpdataMessage();
	
	bool    GetIGA3DMessage( SIGA3DMSG* msg );	//����Ϣ������õ�һ����Ϣ
	void	SendIGA3DMessage( SIGA3DMSG msg );	//����Ϣ���͵���Ϣ������
	void	RemoveMsg();//�Ƴ�������Ϣ

	void	SetMsgManagerState( DWORD state );	//���ù�����״̬

private:
	typedef queue<SIGA3DMSG>	IGAMSG;
	IGAMSG		m_Msg;

public:
	int		m_xCursor;
	int		m_yCursor;

};
CIGA3DMessageManager* GetMessageManager();

