/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : IGACamera.h
*	����������IGA3D, modify by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#pragma  once 

#include <d3d9.h>
#include <d3dx9.h>

class CIGACamera
{
public:
	CIGACamera();
	~CIGACamera();

	void ComputeCamera( D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj );
	bool InViewCentrum( D3DXVECTOR3* pv );
	bool GetCameraPt( D3DXVECTOR3* pvCameraPt );

	void SetClipDistance( float fDist );

protected:
	void UpdataViewCentrum();

private:
	D3DXMATRIX		m_matView;	//��ͼ����
	D3DXMATRIX		m_matProj;	//ͶӰ����

	//�õ���ز���
	D3DXVECTOR3		m_vLookAtDir;	//��Ӱ���Ĺ۲췽��
	D3DXVECTOR3		m_vUpDir;		//��Ӱ�����Ϸ���
	D3DXVECTOR3		m_vRightDir;    //��Ӱ�����з���
	D3DXVECTOR3     m_vCameraPt;    //��Ӱ����λ��

	//��׵��
	D3DXVECTOR3		m_vtx[8];
	D3DXVECTOR3		m_vViewCentrum[8];

	//�ü���Ч���뼰�ü��Ƕ�
	double			m_fClipDistance;
	float			m_fHorizontalClipAngle;	//ˮƽ�ü���
	float			m_fVerticalClipAngle;	//��ֱ�ü���
};

extern CIGACamera g_CIGACamera;


//CIGACamera* GetIGACamera();