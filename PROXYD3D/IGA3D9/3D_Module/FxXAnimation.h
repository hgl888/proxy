/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : FxXAnimation.h
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

struct D3DXFRAME_DERIVED: public D3DXFRAME
{
	D3DXMATRIXA16        CombinedTransformationMatrix;
};


struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;       // array of textures, entries are NULL if no texture specified    

	// SkinMesh info             
	LPD3DXMESH           pOrigMesh;
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD                NumAttributeGroups; 
	DWORD                NumInfl;
	LPD3DXBUFFER         pBoneCombinationBuf;
	D3DXMATRIX**         ppBoneMatrixPtrs;
	D3DXMATRIX*          pBoneOffsetMatrices;
	DWORD                NumPaletteEntries;
	bool                 UseSoftwareVP;
	DWORD                iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning
	D3DXMATRIXA16*       pBoneMatrices;
};

HRESULT GenerateSkinnedMesh( IDirect3DDevice9 *pd3dDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer );

