#pragma  once 

#include <d3d8.h>
#include <d3dx8.h>

class CBasePlay
{
public:
	virtual void PlayAVIFile( LPDIRECT3DDEVICE8 pd3dDevice, char* AVIFileName ){}
	virtual void PlayFlashFile( LPDIRECT3DDEVICE8 pd3dDevice, char* SWFFileName ){}
	virtual void FrameMove(){}
	virtual void Destory(){}

public:
	static CBasePlay* GetPlayerInstance();
protected:
	static CBasePlay*	m_Instance;
};

class CBackGroundPlayer : public CBasePlay
{
public:
	CBackGroundPlayer();
	~CBackGroundPlayer();

public:
	void	PlayAVIFile( LPDIRECT3DDEVICE8 pd3dDevice, char* AVIFileName );
	void    PlayFlashFile( LPDIRECT3DDEVICE8 pd3dDevice, char* SWFFileName );
	void	FrameMove();
	void	Destory();
	void	Stop();

protected:
	CDirectXVideoTexture	m_VideoPlayer;	//��ȡ����������
	FlashSprite				m_FlashTexture; //��ȡflashͼ��

	bool CheckDevice();
	bool m_bDrawBuf;

	LPDIRECT3DTEXTURE8 GetMediaInfor();	//����Ŀǰ���ŵ����ͣ������Ӧ��ý����Ϣ
	void DrawToBkBuf( LPDIRECT3DTEXTURE8 pTexture );
	void DrawToMesh( LPDIRECT3DTEXTURE8 pTexture );

	int		m_nPlayType;

	bool	m_bPlayIsEnd;	//��ǰý���Ƿ񲥷����

	int		m_X;
	int		m_Y;

	int		m_nMediaWidth;
	int		m_nMediaHeight;


	LPDIRECT3DDEVICE8	m_pd3dDevice;
	LPDIRECT3DSURFACE8	m_pBackGroundBuff;
	D3DSURFACE_DESC		m_BackGroundBufDesc;

//------------------------------------------
//ʹ��������ƣ���Щ��Ϸ���������󱸻�����
protected:
	LPDIRECT3DVERTEXBUFFER8		m_pVB;
	struct _SVB
	{
		float x, y, z, rhw;
		float u, v;
		const static DWORD FVF = D3DFVF_XYZRHW|D3DFVF_TEX1;
	};

private:
	RECT	m_LockRect;
	POINT	m_kCursor;

	int __x;
	int __y;
	bool _bLB;

	void	ComputeCursor();
};

