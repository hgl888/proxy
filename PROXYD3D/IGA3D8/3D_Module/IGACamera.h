#include <d3d8.h>
#include <d3dx8.h>

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
	D3DXVECTOR3     m_vCameraPoint;    //��Ӱ����λ��

	//��׵��
	D3DXVECTOR3		m_vtx[8];
	D3DXVECTOR3		m_vViewCentrum[8];

	//�ü���Ч���뼰�ü��Ƕ�
	double			m_dbClipDistance;
	float			m_fHorizontalClipAngle;	//ˮƽ�ü���
	float			m_fVerticalClipAngle;	//��ֱ�ü���
};

extern CIGACamera g_CIGACamera;

