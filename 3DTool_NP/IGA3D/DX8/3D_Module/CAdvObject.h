#pragma once
#include "d3d8.h"
#include "d3dx8.h"
#include <list>
#include <iostream>
#include <fstream>

#include <queue>
#include <algorithm>

using namespace std;
#define PLANE_EPSILON	0.5f

#include "MeshLibrary.h"
#include "TexLibrary.h"

//------------------------------------------------------------
//
struct BaseDescription
{	
	DWORD			AdvObjectDescriptionID;	//����ID
	int				num_Pack;				//������Ĵ���
	DWORD			e_time;					//��ʾʱ��
	int				mesh_id;
	int				texture_id;
	//int				skin_mesh_id;
	BaseDescription(): AdvObjectDescriptionID(0),e_time(0),num_Pack(0)
	{
		mesh_id		= -1;
		texture_id	= -1;
		//skin_mesh_id = -1;
	}
};

struct BaseAdvObjectDescription : public BaseDescription
{
	D3DXVECTOR3		MinPoint, MaxPoint, CenterPoint;//�������ײ�еĶԽǵ�
	float           RotationY;
	bool			is_Active;				//�Ƿ��ǻ��
	BaseAdvObjectDescription()
	{
		MinPoint.x = -1;
		MinPoint.y = -1;
		MinPoint.z = -1;
		MaxPoint.x = 1;
		MaxPoint.y = 1;
		MaxPoint.z = 1;
		CenterPoint.x = 0;
		CenterPoint.y = 0;
		CenterPoint.z = 0;
		RotationY = 0;
		is_Active = false;
	}
};

struct AdvObjectDescription : public BaseAdvObjectDescription
{
	BOOL			b_isPack;				//�Ƿ�ʰȡ��
	DWORD			time;					//��ʼ��ʾʱ��
	bool			is_Show;				//��ʾ��ʱ�Ŀ�ʼ�������־
	
	AdvObjectDescription():b_isPack(true),time(0)
	{
		is_Show  = false;
	}
};

class CAdvObject
{
public:
	CAdvObject();
	~CAdvObject();

public:
	HRESULT InitAdvObject( LPDIRECT3DDEVICE8 pd3dDevice );	//��ʼ������������
	void Release();				//�ͷŹ���������

	//Ϊ�л������ṩ��
	HRESULT LoadDDF( char* FileName );	//����DDF�ļ�
	HRESULT UnloadDDF();				//ж��DDF�ļ�
	D3DXVECTOR3 GetViewPos();			//��ȡ��Ӱ����λ��

	//������Ϸ�ĵ�λ����ת������
	void	SetMatrixTranslation( D3DXMATRIX matTran );	//

	//���ù��ߵĲο�����
	void SetOffsetParam( float tran, float rot, float scal );

	void Render( D3DXMATRIX matView, D3DXMATRIX matProj );				//"����"���й�����
	//ע�⣺����Ļ����Ǽ�����������׵��Χ�ڵĹ����󣬲��ж��Ƿ��ж���ѡ��
	
	void OutputToFile( char* pathName );

	DWORD GetSelectID(){ return m_SelectID; };
	void  ClearSelectID(){ m_SelectID = 0xffffffff; };

private:
	void RunScript( int meshID, int* ptexID );

protected:
	bool ViewCentrumClip( D3DXVECTOR3* pv );		//��׵�ü�
	void UpdataViewCentrum( D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj );	//������׵��
	BOOL Pick(	D3DXVECTOR3		  * pPosition,	//��ײ�е�λ��
				CONST D3DXVECTOR3 * pMin,		//��ײ�еĶԽǵ�
				CONST D3DXVECTOR3 * pMax );		//ʰȡ����
	virtual void KeyProcessor(){};

protected:
	virtual HRESULT InitSub(){ return S_OK; };
	virtual void RenderSub( AdvObjectDescription *pAdvObjectDescription ){}; //�����Ļ���������
	virtual void ReleaseSub(){};
	virtual void OutputToFileSub( char* pathName ){};

	//�޸ĺ����Ĳο�����
	float					m_OffsetTran, m_OffsetRot, m_OffsetScal;

	//�����滻��������
	typedef map<int,int>		DES_SCR;	//�����ű�����
	typedef pair<int,int>		NODE;
	map<int,int>::iterator		m_pNode;   

	DES_SCR	m_Des;

	//��ӽű�����
	void AddDes( int mesh_id, int tex_id );


protected:
	list<AdvObjectDescription*>				m_AdvObjectContainer; //����������
	list<AdvObjectDescription*>::iterator   m_Iterator;			//ָ��ѡ�еĽڵ�
	AdvObjectDescription					*m_pPackObject;		//ָ��ʰȡ����ײ��
	D3DXVECTOR3								m_vShowPos;
	bool									m_bIsDesignMode;			//�Ƿ������ģʽ

	LPDIRECT3DDEVICE8		m_pd3dDevice;	//��ͼ�豸
	DWORD					m_SelectID;		//��ѡ�ֶ����ID
	DWORD					m_MaxObjectID;	//��ǰ���������ID����

	int          iWidth, iHeight;

private:	//���ڽ�����׵�и�
    D3DXVECTOR3	m_vtx[9];	/// ����ƽ��ͷ���8������
	D3DXPLANE	m_plane[6];	/// ����ƽ��ͷ���6��ƽ��
	D3DXVECTOR3	m_vPos;		/// ��ǰ���������������
	float       m_nearLength; //��ǰ֡����Ӱ�������һ��ѡ�ж���ľ���
	D3DXMATRIX  m_matView, m_matProj;

protected:
	D3DXMATRIX  m_MatrixTranslate;	//ת������

	D3DXMATRIX  _m_MatrixTranslate; //�ڲ���ת��������ģ��������ռ������任
	
};

//------------------------------------------------------------
class CAdvObjectTool : public CAdvObject
{
public:
	CAdvObjectTool();
	~CAdvObjectTool();

public:
	HRESULT CreateBoundBox();	//����Ӱ����ƽ�洴��һ�����ӣ����Ŀռ����������ײ�͵ļ�������
	HRESULT DeleteBoundBox();	//ɾ����ǰ����ײ�У��ӽڵ��������Ƴ���ǰ����

public:
	void OutputToFileSub( char* pathName );

public:  //���صĳ�Ա����
	void RenderSub( AdvObjectDescription *pAdvObjectDescription );
	void ReleaseSub();
	HRESULT InitSub();
	void SetMaxMeshID( int id ){ m_MaxMeshID = id; };
	void SetMaxTexID( int id ){ m_MaxTexID = id; };

private:
	void	KeyProcessor();	  //��������
	void    ModifyBoundBox(); //�޸���ײ������

private:	//���ڴ������ƹ�����
	LPD3DXMESH				m_pCurrentMesh;
	D3DMATERIAL8			m_WhiteMaterial,m_RedMaterial;

	float					x, y, z, s_x, s_y, s_z, r_y;	//ѡ����ײ�е�����λ�úгߴ磬������Ϊһ����ʱ�洢��
		
	AdvObjectDescription    *m_pAdvObjectDescription;	//��ǰ��ײ�������������ڴ����µ���ײ��

private:	//������ֿ�
	int						m_CurrentMeshID, m_MaxMeshID;
	int						m_CurrentTexID, m_MaxTexID;
//----------------------------------------------
//����ӿ�
protected:
	virtual void ComputeBoundBox( LPD3DXMESH pMesh ){};			//������ײ�в�������ײ�жԽǵ�
	virtual void SetPosition( float x, float y, float z ){};	//������ײ��λ��
	virtual void AdjustBoundBox(){};	//ͨ�����̵�����ײ�е�����
};

CAdvObject* GetAdvObject();

CAdvObjectTool* GetAdTool();

