/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : IGATexture9.h
*	����������IGATextur9, modify by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�    
*	�޸ģ�������ƽӿڣ�Ŀ���ܹ�ʵ�������������ݣ�ͨ�������ļ����ķ�ʽ�޸�
*	�޸��ߣ�	hgl
*	�޸����ڣ� 2009-01-19
/************************************************************************/

#pragma  once 

//extern LPDIRECT3DDEVICE8	g_pd3dDevice;

HRESULT D3D9TextureInit( LPDIRECT3DDEVICE9 pd3dDevice );
HRESULT D3D9StartPlay(char * pszTextureFileName);
HRESULT D3D9StopPlay();
LPDIRECT3DTEXTURE9 D3D9GetTexture( int id, bool* bIsPlayEnd, float* w = NULL, float* h = NULL );
HRESULT D3D9PausePlay();
HRESULT D3D9UnPausePlay();
HRESULT D3D9GotoFrame(int iFrame);
HRESULT D3D9GetCurrentFrame();
void	D3D9TextureDestory();

//////////////////////////////////////////////////////////////////////////

int     DrawPlayer();
void	PlayAVI( LPDIRECT3DDEVICE9 pd3dDevice, char* name);
void	PlayFLASH(char* name);
void	PausePlayer();
void	StopPlayer( DWORD dwID);