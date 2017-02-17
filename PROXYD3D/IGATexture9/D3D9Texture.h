/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : IGATexture.h
*	����������IGATextur9, modify by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#pragma  once

#include "d3d9.h"
#include "IGATexture9.h"


//flash���� 
class CD3D9FlashTexture : public CD3D9TextureBase
{
public:
	CD3D9FlashTexture();
	virtual ~CD3D9FlashTexture();

public:
	HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice, char* szFileName, int iWidth, int iHeight );
	HRESULT D3D9ResetDevice( LPDIRECT3DDEVICE9 pd3dDevice );
	void D3D9StartPlay();
	void D3D9StopPlay();
	LPDIRECT3DTEXTURE9 D3D9GetTexture();
	void D3D9PausePlay();
	void D3D9UnPausePlay();
	void D3D9GotoFrame(int iFrame);
	void D3D9GetCurrentFrame();
	void D3D9TextureDestory();

private:
	FlashSprite		m_FlashSprite;
};


//��Ƶ����
class CD3D9VideoTexture : public CD3D9TextureBase
{
public:
	CD3D9VideoTexture();
	~CD3D9VideoTexture();

public:
	HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice, char* szFileName, int iWidth, int iHeight );
	HRESULT D3D9ResetDevice( LPDIRECT3DDEVICE9 pd3dDevice );
	void D3D9StartPlay();
	void D3D9StopPlay();
	LPDIRECT3DTEXTURE9 D3D9GetTexture();
	void D3D9PausePlay();
	void D3D9UnPausePlay();
	void D3D9GotoFrame(int iFrame);
	void D3D9GetCurrentFrame();
	void D3D9TextureDestory();

private:
	CDirectXVideoTexture	m_VideoTexture;
};

//--------------------------------------------------------------
//��̬����
class CIGAStaticTexture : public CD3D9TextureBase
{
public:
	CIGAStaticTexture();
	~CIGAStaticTexture();

private:
	HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice, char* name );
	LPDIRECT3DTEXTURE9 D3D9GetTexture();
	void D3D9TextureDestory();
	virtual void D3D9StartPlay(){}
	virtual void D3D9StopPlay(){}
	virtual void D3D9PausePlay(){}
	virtual	void GetSize( float* w = NULL, float* h = NULL ){}

private:
	//�������,ֱ��ʹ�û���Ķ���
};