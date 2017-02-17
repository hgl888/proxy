#include <d3d8.h>
#include <d3dx8.h>
#include <string>
#include "list"
using namespace std;

//UI��Ƭ
class CIGA3DUIElement
{
public:
	CIGA3DUIElement();
	~CIGA3DUIElement();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Render();
	void	Destory();

//��������
public:
	//���ô�С
	HRESULT SetSize( int width, int height );

	//������������
	void SetTexCoord( int w, int h );	//�����������Ĵ�С

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
	int						m_iWidth,m_iHeight;

	struct _VUI
	{
		float x,y,z,rhw;
		DWORD color;
		float u,v;
	};
#define _D3DFVF_VUI ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )

};

//��������
class CTexture8Play
{
public:
	CTexture8Play();
	~CTexture8Play();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Destroy();
	void PlayAVIFile( char* fileName );
	void PlayFlashFile( char* fileName );
	void Stop();
	void Pause();
	void PlayNext();

	void AddToPlayList( string name );
	
	int ShowPlayer( UINT type );	//����Ļ����ʾ�����ͣ���ʾͼ�񲢲�������������ʾһ����ť������������

	void SetPos( int x, int y );

//---------------------------------------------------------------------
protected:
	bool		m_bIsMin;	//�Ƿ���С������
	bool		m_bPause;   //�Ƿ�����ͣ״̬
	string		m_PlayFileName;	//��ǰ���ŵ��ļ�����


//-------------------------------------------------------------------
//��������
protected:
	CIGA3DUIElement		m_MediaFrame; //ý���ܣ��������
	float				m_FrameX, m_FrameY;		//��ܵ����Ͻǵ�
	float				m_FrameWidth, m_FrameHeight; //�������Ĵ�С

protected:
	CIGA3DUIElement		m_MediaScreen; //ý����Ļ
	float				m_ScreenX, m_ScreenY;		//ý����Ļ�����Ͻǵ�
	float				m_ScreenWidth, m_ScreenHeight; //ý����Ļ�Ĵ�С

protected:
	CIGA3DUIElement		m_MinScreen;  //��С�����ڿ��
	float				m_MinX, m_MinY;		
	float				m_MinWidth, m_MinHeight; 

protected:
	CIGA3DUIElement		m_MinButton;  //��С����ť
	float				m_MinButtonX, m_MinButtonY;		
	float				m_MinButtonWidth, m_MinButtonHeight; 

protected:
	CIGA3DUIElement		m_PlayButton;  //���š���ͣ��ť
	float				m_PlayButtonX, m_PlayButtonY;		
	float				m_PlayButtonWidth, m_PlayButtonHeight; 

protected:
	CIGA3DUIElement		m_StopButton;  //ֹͣ��ť
	float				m_StopButtonX, m_StopButtonY;		
	float				m_StopButtonWidth, m_StopButtonHeight; 



protected:
	LPDIRECT3DTEXTURE8	m_pTexScreen;

	LPDIRECT3DTEXTURE8  m_pTexBackGround;

	LPDIRECT3DTEXTURE8  m_pFrameTex;	//�����������
	LPDIRECT3DTEXTURE8  m_pMinTex;		//�����С������
	LPDIRECT3DTEXTURE8  m_pMinTex1;		//���ѡ��״̬������

	LPDIRECT3DTEXTURE8  m_pMinButtonTex;		//�����С������
	LPDIRECT3DTEXTURE8  m_pMinButtonTex1;		//���ѡ��״̬������

	LPDIRECT3DTEXTURE8  m_pPlayButtonTex;		//���Ű�ť����
	LPDIRECT3DTEXTURE8  m_pPlayButtonTex1;		//

	LPDIRECT3DTEXTURE8  m_pPauseButtonTex;		//��ͣ��ť����
	LPDIRECT3DTEXTURE8  m_pPauseButtonTex1;		//

	LPDIRECT3DTEXTURE8  m_pStopButtonTex;		//ֹͣ��ť����
	LPDIRECT3DTEXTURE8  m_pStopButtonTex1;		//

//-------------------------------------------------------------------
//�ײ����
protected:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	list<string>			m_PlayList;
	list<string>::iterator	m_pNode;
	bool					m_bIsPlay;		//�Ƿ��ڲ���

	CDirectXVideoTexture	m_VideoTexture;
	FlashSprite				m_FlashTexture;

	int						m_nFlag;	//�������ͱ�־

};

extern CTexture8Play g_CTexture8Play;
