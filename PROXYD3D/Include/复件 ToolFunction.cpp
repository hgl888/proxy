/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : ToolFunction.cpp
*	����ʹ�õĽṹ�嶨��, add by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/
#include "dxstdafx.h"
#include "ToolFunction.h"
#include "DXUT.h"
#include "DefineConst.h"


//����ת�ַ���,Ϊʲô���� itoa()
void APP_Tools::AppIntToChar(int num,char * text)
{
	int a = 0;
	if(num < 0)
	{
		text[0] = '-';
		a = 1;
		num = abs(num);
	}
	char tmp[10];
	for(int i=0;i<10;i++)
	{
		tmp[i] = num % 10 + 48;
		num = num / 10;
	}
	for(int i=9;i>=0;i--)
	{
		if(tmp[i] != '0')
		{
			text[a] = tmp[i];
			a ++;
		}
		else
		{
			if(a != 0 && text[0] !='-')
			{
				text[a] = tmp[i];
				a ++;
			}
		}
	}
	if(a == 0)
	{
		text[a] = '0';
		a ++;
	}
	text[a] = 0;
}

void APP_Tools::AppIntToChar(int num,TCHAR * text)
{
	int a = 0;
	if(num < 0)
	{
		text[0] = '-';
		a = 1;
		num = abs(num);
	}
	TCHAR tmp[10];
	for(int i=0;i<10;i++)
	{
		tmp[i] = num % 10 + 48;
		num = num / 10;
	}
	for(int i=9;i>=0;i--)
	{
		if(tmp[i] != '0')
		{
			text[a] = tmp[i];
			a ++;
		}
		else
		{
			if(a != 0 && text[0] !='-')
			{
				text[a] = tmp[i];
				a ++;
			}
		}
	}
	if(a == 0)
	{
		text[a] = '0';
		a ++;
	}
	text[a] = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
//�ж�ĳ���Ƿ��ھ���������
bool APP_Tools::AppInRect(int x,int y,RECT rect)
{
	if(x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom)
		return true;
	else
		return false;
}

bool APP_Tools::AppInRect(int x,int y,int rectX,int rectY,int rectWidth,int rectHeight)
{
	if(x >= rectX && x <= rectX + rectWidth && y >= rectY && y <= rectY + rectHeight)
		return true;
	else
		return false;
}


////////////////////////////////////////////////////////////////////////////////////////////
//2D������ײ
bool APP_Tools::AppRectangleBumpCheck( RECT rect1, RECT rect2 )
{
	int width2 = rect2.right - rect2.left;
	int height2 = rect2.bottom - rect2.top;

	if( ( rect1.left - rect2.left ) < width2 &&
		( rect2.right  - rect1.right ) < width2 &&
		( rect1.top - rect2.top ) < height2 &&
		( rect2.bottom - rect1.bottom ) < height2 )
		return true;
	else
		return false;
}


bool APP_Tools::AppRectangleBumpCheck( int x1, int y1, int width1, int height1,
						    int x2, int y2, int width2, int height2 )
{
	if( ( x1 - x2 ) < width2 &&
		( x2 + width2 - x1 - width1 ) < width2 &&
		( y1 - y2 ) < height2 &&
		( y2 + height2 - y1 - height1 ) < height2 )
		return true;
	else
		return false;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////
//����*�任����
void APP_Tools::AppVec3Transform( D3DXVECTOR3 & vPoint, D3DXMATRIX matTrans )
{
	D3DXVECTOR3 vTmp = vPoint;
	vPoint.x = vTmp.x * matTrans.m[0][0] + vTmp.y * matTrans.m[1][0] + vTmp.z * matTrans.m[2][0] + matTrans.m[3][0];
	vPoint.y = vTmp.x * matTrans.m[0][1] + vTmp.y * matTrans.m[1][1] + vTmp.z * matTrans.m[2][1] + matTrans.m[3][1];
	vPoint.z = vTmp.x * matTrans.m[0][2] + vTmp.y * matTrans.m[1][2] + vTmp.z * matTrans.m[2][2] + matTrans.m[3][2];
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////

double APP_Tools::AppGetTime()
{
	return GetTickCount() / 1000.f;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//3D���߼��
bool APP_Tools::AppRayPlaneCheck( D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 & vOut )
{
	
	/*D3DXPLANE plane;
	D3DXPlaneFromPoints( &plane, &v0, &v1, &v2 );

	D3DXVECTOR3 vTmp;
	if( !D3DXPlaneIntersectLine( &vTmp, &plane, &vPos1, &vPos2 ) )
		return false;

	float a = D3DXVec3Length( &( v0 - v1 ) );
	float b = D3DXVec3Length( &( v0 - v2 ) );
	float c = D3DXVec3Length( &( v1 - v2 ) );

	float p = ( a + b + c ) / 2;
	float s = p * ( p - a ) * ( p - b ) * ( p - c );

	a = D3DXVec3Length( &( v0 - vTmp ) );
	b = D3DXVec3Length( &( v1 - vTmp ) );
	c = D3DXVec3Length( &( v0 - v1 ) );
	p = ( a + b + c ) / 2;
	float s1 = p * ( p - a ) * ( p - b ) * ( p - c );

	a = D3DXVec3Length( &( v0 - vTmp ) );
	b = D3DXVec3Length( &( v2 - vTmp ) );
	c = D3DXVec3Length( &( v0 - v2 ) );
	p = ( a + b + c ) / 2;
	float s2 = p * ( p - a ) * ( p - b ) * ( p - c );

	a = D3DXVec3Length( &( v1 - vTmp ) );
	b = D3DXVec3Length( &( v2 - vTmp ) );
	c = D3DXVec3Length( &( v1 - v2 ) );
	p = ( a + b + c ) / 2;
	float s3 = p * ( p - a ) * ( p - b ) * ( p - c );

	s = sqrt( s ) - sqrt( s1 ) - sqrt( s2 ) - sqrt( s3 );
	if( s > -0.1f )
	{
		vOut = vTmp;
		return true;
	}
	else
	{
		return false;
	}*/
	
	D3DXVECTOR3 vDir = vPos2 - vPos1;

    // ��������ߵ�����
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &vDir, &edge2 );

    // ���detΪ0����ӽ������������������湲���ƽ�У����ཻ
//�˴�det���൱������ģ�
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );
    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = vPos1 - v0;
    }
    else
    {
        tvec = v0 - vPos1;
        det = -det;
    }
    if( det < 0.0001f )
        return FALSE;
    // ����uv�������Ƿ�Ϸ������������ڣ�
	float u,v,t;
    u = D3DXVec3Dot( &tvec, &pvec );
    if( u < 0.0f || u > det )
        return FALSE;
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );
    v = D3DXVec3Dot( &vDir, &qvec );
    if( v < 0.0f || v + u > det )
        return FALSE;

    t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    t *= fInvDet;
    u *= fInvDet;
    v *= fInvDet;
	D3DXPLANE plane;
	D3DXPlaneFromPoints( &plane, &v0, &v1, &v2 );
	D3DXVec3Normalize( &vDir, &vDir );
	D3DXVECTOR3 vTmp;
	D3DXPlaneIntersectLine( &vTmp, &plane, &vPos1, &vPos2 );
	if( ( vTmp.y >= vPos1.y && vTmp.y <= vPos2.y ) || ( vTmp.y <= vPos1.y && vTmp.y >= vPos2.y ) )
		vOut = vTmp;
	else
		return false;
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//����
void APP_Tools::ComputeTriNormal( D3DXVECTOR3 * v1, D3DXVECTOR3 * v2, D3DXVECTOR3 * v3,D3DVECTOR & normalVec )
{
	D3DXVECTOR3 tmpV1 = *v1 - *v2;
	D3DXVECTOR3 tmpV2 = *v1 - *v3;
	D3DXVECTOR3 tmpNormalVec;
	D3DXVec3Cross( &tmpNormalVec, &tmpV1, &tmpV2 );
	D3DXVec3Normalize( &tmpNormalVec, &tmpNormalVec );
	normalVec = tmpNormalVec;
}


void APP_Tools::GetRay(HWND hWnd, D3DXMATRIXA16 &matProj, D3DXMATRIXA16& matView, D3DXVECTOR3 vPos, int x, int y, LPRAY ray )
{
	ray->vOrig = vPos;
	RECT rect;
	GetClientRect( hWnd, &rect );

	//������Ļ�ռ������(-1,-1)��(1,1)
	D3DXVECTOR3 v;
	v.x =  ( 2 * x  / (float)rect.right - 1 ) / matProj._11;
	v.y =  -( 2 * y  / (float)rect.bottom - 1 ) / matProj._22;
	v.z =  1.0f;
	//�����ͼ�任����������
	D3DXMATRIXA16 m;
	D3DXMatrixInverse( &m, NULL, &matView );

	//����������ת������������
	D3DXVECTOR3 vTmp = v;
	v.x = vTmp.x * m.m[0][0] + vTmp.y * m.m[1][0] + vTmp.z * m.m[2][0] ;
	v.y = vTmp.x * m.m[0][1] + vTmp.y * m.m[1][1] + vTmp.z * m.m[2][1] ;
	v.z = vTmp.x * m.m[0][2] + vTmp.y * m.m[1][2] + vTmp.z * m.m[2][2] ;

	ray->vVector = v;

}
//��Ļ���굽��������ת��
void APP_Tools::ScreenPosToWorldPos(const HWND hWnd,const D3DXMATRIXA16 &matProj,const D3DXMATRIXA16& matView,int x, int y, D3DXVECTOR3 &vWorldPos)
{
	RECT rect;
	GetClientRect( hWnd, &rect );

	//������Ļ�ռ������(-1,-1)��(1,1)
	D3DXVECTOR3 v;
	v.x =  ( 2 * x  / (float)rect.right - 1 ) / matProj._11;
	v.y =  -( 2 * y  / (float)rect.bottom - 1 ) / matProj._22;
	v.z =  1.0f;
	//�����ͼ�任����������
	D3DXMATRIXA16 m;
	D3DXMatrixInverse( &m, NULL, &matView );

	//����������ת������������
	D3DXVECTOR3 vTmp = v;
	v.x = vTmp.x * m.m[0][0] + vTmp.y * m.m[1][0] + vTmp.z * m.m[2][0] ;
	v.y = vTmp.x * m.m[0][1] + vTmp.y * m.m[1][1] + vTmp.z * m.m[2][1] ;
	v.z = vTmp.x * m.m[0][2] + vTmp.y * m.m[1][2] + vTmp.z * m.m[2][2] ;
	vWorldPos = v;

}

bool  APP_Tools::RayCheck(D3DXVECTOR3 vMin,D3DXVECTOR3 vMax,float fScaling, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir)
{
	D3DXMATRIX  mMat;

	D3DXMatrixScaling(&mMat,fScaling,fScaling,fScaling);
	APP_Tools::AppVec3Transform(vMin,mMat);
	APP_Tools::AppVec3Transform(vMax,mMat);
	vMin = vMin;
	vMax = vMax;
	if (D3DXBoxBoundProbe(&vMin,&vMax,&vPos,&vDir))
		return true;
	else 
		return false;
}


//-----------------------------------------------------------------------------
// Desc: ʰȡ������
//-----------------------------------------------------------------------------
HRESULT Pick_Triangle(IDirect3DDevice9* pd3dDevice, LPD3DXMESH pMesh)
{

	HRESULT hr = S_OK;
	DWORD dwNumIntersections = 0L;

	if( !GetCapture() )
		return hr;

	//����ʰȡ������ر�������
	POINT        ptCursor;  //���λ��
	D3DXMATRIX   matWorld, matView, pmatProj, m;
	D3DXVECTOR3  vPickRayOrig, vPickRayDir;
	int          iWidth, iHeight;

	//��ȡ��̨�������Ŀ�Ⱥ͸߶�
	iWidth = DXUTGetBackBufferSurfaceDesc()->Width;
	iHeight = DXUTGetBackBufferSurfaceDesc()->Height;

	//��ȡ��ǰ����ڴ��ڿͻ����е�λ��
	GetCursorPos( &ptCursor );
	ScreenToClient( DXUTGetHWND(), &ptCursor );

	//��ȡ��ǰ�豸�ı任����
	pd3dDevice->GetTransform( D3DTS_WORLD, &matWorld );
	pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
	pd3dDevice->GetTransform( D3DTS_PROJECTION, &pmatProj );

	//��������۲����������
	D3DXMATRIX mWorldView = matWorld * matView;
	D3DXMatrixInverse( &m, NULL, &mWorldView );

	//����ʰȡ���ߵķ�����ԭ��
	D3DXVECTOR3 vTemp;
	vTemp.x =  ((( 2.0f * ptCursor.x ) / iWidth  ) - 1 ) / pmatProj._11;
	vTemp.y = -((( 2.0f * ptCursor.y ) / iHeight ) - 1 ) / pmatProj._22;
	vTemp.z =  1.0f;

	vPickRayDir.x  = vTemp.x*m._11 + vTemp.y*m._21 + vTemp.z*m._31;
	vPickRayDir.y  = vTemp.x*m._12 + vTemp.y*m._22 + vTemp.z*m._32;
	vPickRayDir.z  = vTemp.x*m._13 + vTemp.y*m._23 + vTemp.z*m._33;

	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;

	//���㱻ʰȡ����������, �õ�ʰȡ�������ε�����
	BOOL               bHit;
	LPD3DXBUFFER       pBuffer = NULL;
	D3DXINTERSECTINFO* pIntersectInfoArray;

	V_RETURN( D3DXIntersect( pMesh, &vPickRayOrig, &vPickRayDir, &bHit, 
		NULL, NULL, NULL, NULL, 
		&pBuffer, &dwNumIntersections ));

	//if( dwNumIntersections > 0 )
	//{
	//	pIntersectInfoArray = (D3DXINTERSECTINFO*)pBuffer->GetBufferPointer();
	//	if( dwNumIntersections > MAX_INTERSECTIONS )
	//		dwNumIntersections = MAX_INTERSECTIONS;
	//	for( DWORD iIntersection = 0; iIntersection < dwNumIntersections; iIntersection++ )
	//	{
	//		g_IntersectionArray[iIntersection] = pIntersectInfoArray[iIntersection].FaceIndex;
	//	}
	//}
	//SAFE_RELEASE( pBuffer );

	////����ʰȡ�������ε�����, �������ζ���������ӵ�g_pVB��
	//LPDIRECT3DVERTEXBUFFER9  pVB;
	//LPDIRECT3DINDEXBUFFER9   pIB;
	//WORD*                    pIndices;
	//D3DVERTEX*               pVertices;

	//pMesh->GetVertexBuffer( &pVB );
	//pMesh->GetIndexBuffer( &pIB );
	//pIB->Lock( 0, 0, (void**)&pIndices, 0 );
	//pVB->Lock( 0, 0, (void**)&pVertices, 0 );

	//if( dwNumIntersections > 0 )
	//{
	//	D3DVERTEX* v;
	//	D3DVERTEX* vThisTri;
	//	WORD*      iThisTri;
	//	DWORD*     pIntersection;

	//	g_pVB->Lock( 0, 0, (void**)&v, 0 );

	//	for( DWORD iIntersection = 0; iIntersection < dwNumIntersections; iIntersection++ )
	//	{
	//		pIntersection = &g_IntersectionArray[iIntersection];
	//		vThisTri      = &v[iIntersection * 3];
	//		iThisTri      = &pIndices[3*(*pIntersection)];  

	//		vThisTri[0] = pVertices[iThisTri[0]];
	//		vThisTri[1] = pVertices[iThisTri[1]];
	//		vThisTri[2] = pVertices[iThisTri[2]];
	//	}

	//	g_pVB->Unlock();
	//}

	//pVB->Unlock();
	//pIB->Unlock();
	//SAFE_RELEASE(pVB);
	//SAFE_RELEASE(pIB);

	return S_OK;
}

