
#pragma once

#include "D3D.h"



struct D3DXFRAME_DERIVED : public D3DXFRAME               //��չ֡
{
    D3DXMATRIXA16        CombinedTransformationMatrix;//��ϱ任����
};


struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER            //��չMesh����
{
    LPDIRECT3DTEXTURE9*  ppTextures;       
                                
    // SkinMesh��Ϣ            
    LPD3DXMESH           pOrigMesh;
    LPD3DXATTRIBUTERANGE pAttributeTable;
    DWORD                NumAttributeGroups; 
    DWORD                NumInfl;
    LPD3DXBUFFER         pBoneCombinationBuf;
    D3DXMATRIX**         ppBoneMatrixPtrs;
    D3DXMATRIX*          pBoneOffsetMatrices;
    DWORD                dwNumPaletteEntries;
    bool                 UseSoftwareVP;
    DWORD                iAttributeSW;     //SW   HW
};

//skinģʽ
enum METHOD 
{
    D3DNONINDEXED,
    D3DINDEXED,
    SOFTWARE,
    NONE
};


class CSkinMesh;


class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	CAllocateHierarchy( CSkinMesh *pSkinMesh ) : m_pSkinMesh( pSkinMesh ) 
	{
	}



    STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame );

    STDMETHOD( CreateMeshContainer )( 
		THIS_ LPCSTR Name, 
        CONST D3DXMESHDATA *pMeshData,
        CONST D3DXMATERIAL *pMaterials, 
        CONST D3DXEFFECTINSTANCE *pEffectInstances, 
        DWORD NumMaterials, 
        CONST DWORD *pAdjacency, 
        LPD3DXSKININFO pSkinInfo, 
        LPD3DXMESHCONTAINER *ppNewMeshContainer );


    STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pFrameToFree );
    STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerBase );
    

public:
	CSkinMesh*		m_pSkinMesh;
};



//��Ƥ����
class CSkinMesh  
{
public:

	CSkinMesh();


	VOID SetAnim( BOOL bAnim );
	BOOL CSkinMesh::InterSect( D3DVECTOR * pRayOrig, D3DVECTOR * pRayDir,D3DVECTOR * pVRet );
	VOID UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );


	LPD3DXANIMATIONCONTROLLER GetAnimController()	//���Ʊ任�Ķ���
	{
		return m_pAnimController;
	}

	BOOL SetAnimationName( CHAR * strAnimName );		//���ö���������
//	void SetPosition(D3DXVECTOR3 &vPos)			{m_vPos=vPos; };
//	void SetPosition(float x,float y,float z)	{m_vPos.x=x;m_vPos.y=y;m_vPos.z=z;};

	VOID	Render( float fTimeFromLastFrame, D3DXVECTOR3 vPos, float angle, float scale = 1.0f );
	HRESULT LoadFromXFile( TCHAR * strFileName );
	HRESULT GenerateSkinnedMesh( D3DXMESHCONTAINER_DERIVED * pMeshContainer );


	void CalculateBondingBox( LPD3DXFRAME pFrameParent, D3DXVECTOR3 * pVmin, D3DXVECTOR3 * pVmax );

	virtual ~CSkinMesh();

protected:

//	FLOAT						m_fRotateAngle;
	BOOL						m_bMoving;
	LPDIRECT3DDEVICE9			m_pDevice;
	LPD3DXFRAME					m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
    FLOAT						m_fElapsedTime;      //������ʱ�䣬ֱ�����һ֡


	VOID DrawMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );
	VOID DrawFrame( LPD3DXFRAME pFrame );
	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );

public:

	LPDIRECT3DVERTEXSHADER9		m_pIndexedVertexShader[4];
	D3DXMATRIXA16 *				m_pBoneMatrices;

	UINT						m_NumBoneMatricesMax;
	METHOD						m_SkinningMethod;
	D3DCAPS9 					m_d3dCaps;
    D3DXVECTOR3                 m_vObjectCenter;    // ����ķ�Χ����
    FLOAT                       m_fObjectRadius;    // ����ķ�Χ�뾶
	D3DXVECTOR3                 m_vMin;				// ����ķ�Χ���ĵİ뾶����Сֵ
	D3DXVECTOR3                 m_vMax;				//����ķ�Χ���ĵİ뾶�����ֵ

};




class CSkinMeshObj : public CBase
{
public:
	CSkinMeshObj( CSkinMesh * pSkinMesh, D3DXVECTOR3 vPos, float fAngleY, float fScaling )
	{
		m_pSkinMesh = pSkinMesh;
		m_vPos = vPos;
		m_fAngle = fAngleY;
		m_fScaling = fScaling;

		m_dwBaseType = OBJECT_TYEP_SKIN_MESH;
		m_pDevice = g_pD3D->GetDevice();
	}
	~CSkinMeshObj()
	{
	}


	void Render();

	void SetPos( D3DXVECTOR3 vPos )
	{
		m_vPos = vPos;
	}
	void SetPos( float x, float y, float z )
	{
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	void GetPos( D3DXVECTOR3 & vPos )
	{
		vPos = m_vPos;
	}
	D3DXVECTOR3 * GetPos()
	{
		return &m_vPos;
	}

	void SetRotationY( float fAngleY )
	{
		m_fAngle = fAngleY;
	}
	void RotationY( float fAngleY )
	{
		m_fAngle += fAngleY;
	}
	
	void SetScaling( float fScaling )
	{
		m_fScaling = fScaling;
	}

	void SetAction( CHAR * szName )
	{
		m_pSkinMesh->SetAnimationName( szName );
	}

protected:

	CSkinMesh *				m_pSkinMesh;

	D3DXVECTOR3				m_vPos;
	float					m_fAngle;
	float					m_fScaling;

};