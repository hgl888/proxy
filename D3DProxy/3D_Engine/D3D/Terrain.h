

#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#include "D3D.h"
#include "StaticMesh.h"
#include "SkinMesh.h"
#include "Base/Texture.h"
#include "BillBoard.h"



#define D3DFVF_TERRAIN	( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )	//������Ϣ

class CTerrain : public CBase
{
protected:
	struct VERTEX_TERRAIN
	{
		float	x,y,z;			//������Ϣ
		float   nx,ny,nz;
		float	tu,tv;
	};
protected:
	
	CTexture *					m_pTexture;				//����
	LPDIRECT3DVERTEXBUFFER9		m_pVB;//���㻺��
	LPDIRECT3DINDEXBUFFER9		m_pIB;//�������㻺��
	long						m_nCol;
	long						m_nRow;//����
	float						m_maxHeight;//���߶�
	float						m_fCellWidth;//��Ԫ���
	BYTE *						m_pHeightData;//�߶�ָ��
	D3DMATERIAL9				m_matMaterial;
	VERTEX_TERRAIN *			m_pVerticesBackUp;



	Array<CStaticMeshObj *>		m_pStaticMeshObj;
	Array<CStaticMesh *>		m_pStaticMesh;

	Array<CSkinMeshObj *>		m_pSkinMeshObj;
	Array<CSkinMesh *>			m_pSkinMesh;

public:
	CTerrain( D3DVECTOR position );//���캯��
	virtual ~CTerrain();//��������
    bool InitialiseLight(int LightType);//��ʼ���ƹ�
	void CreateTerrain(TCHAR* strTerrain,TCHAR* strTexture, float cellWidth,float height=800);//��������
	float GetHeight(float x,float z);//�õ����γ���
	void Render();//��Ⱦ����
	void SetMatrix();//�任
	bool SetMaterial( D3DCOLORVALUE rgbaDiffuse, D3DCOLORVALUE rgbaAmbient, D3DCOLORVALUE rgbaSpecular, D3DCOLORVALUE rgbaEmissive, float rPower );
	bool LineCheck( D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, D3DXVECTOR3 & vOut );
	bool MoveCheck( RAY ray );
	bool RayCheck( RAY ray, float length, D3DXVECTOR3 & vOut );
	void GetFourVertices( D3DXVECTOR3 & v0, D3DXVECTOR3 & v1, D3DXVECTOR3 & v2, D3DXVECTOR3 & v3, int iCol, int iRow );

};
