/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : defineST.h
*	����ʹ�õĽṹ�嶨��, add by hgl
*	ɾ������Ľṹ����
*	��ǰ�汾��2, 0, 9, 0304
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/
#pragma  once

#include "IGATexture9.h"


enum IGA3D_MSG_ENUM
{
	IGA3D_MSG_UNKNOW = -1,
	IGA3D_MSG_LBUTTONDOWN,
	IGA3D_MSG_LBUTTONUP,
	IGA3D_MSG_RBUTTONDOWN,
	IGA3D_MSG_RBUTTONUP,

};


typedef struct tagD3DSearchParam
{
	typedef VOID(WINAPI *LPCallBack3DFocus)(DWORD,LPVOID);
	union
	{
		DWORD	dwAdID;
		DWORD	dwEventID;
	};

	DWORD	dwGameID;
	int		nSearchType;

	LPCallBack3DFocus pFun3DFocus;	//	ͳ�Ƶ�������Ļص�����
	HWND	hWndFocus;
	HWND	hWndDeviceFullscreen;
	HWND	hWndDeviceWindowed;

	WPARAM	wParam;
	LPARAM	lParam;

}D3DSEARCH_PARAM, *LPD3DSEARCH_PARAM;



struct STIGAStateData
{
	float fx;
	float fy;
	float fz;
	bool bIsReload;
	UINT nPickObjID;
	BOOL bIsShowPickMesh;

	STIGAStateData()
	{
		fx = 0;
		fy = 0;
		fz = 0;
		bIsReload = false;
		nPickObjID = 0;
		bIsShowPickMesh = false;
	}

};



struct _S3DXML
{
	int		id;
	int		Close;
};

struct STDrawElement
{
	int		iObjID;				//����id
	int		iMeshID;			//ģ��ID
	int		iSceneID;			//����ID
	int		iTextureNum;		//��������
	int		iTextureIndex;		//��������
	int		iArrTexture[100];	//�����б�
	int		iTextureID;
	//2008-10-13
	//�����飬���ڱȽϸ���ģ�ͣ�ʹ�����������ʽ��������
	int     iMeshSubNum;
	int     iArrMesh[100];

	//��ʾʱ��
	long	lStart;
	bool	bIsShow;
	float	x;
	float   y;
	float	z;
	float	s_xyz;
	float	r_x;
	float	r_y;
	float	r_z;

	char	szUrl[MAX_PATH];

	UINT	nTotalShowTime;	//�ܹ���ʾ��ʱ��
	UINT	nStartShowTime;	//��ʼ��ʾ��ʱ��
	bool    bStartShow;		//�Ƿ�ʼ��ʾ
	UINT	nPickCount;		//����Ĵ���

	//һ��ģ�Ͷ�Ӧһ���������add by hgl
	bool bIsClip;							// �Ƿ�ü� , false ����û�б��ü��� ture�Ǳ��ü�
	bool bIsPlay;							//�Ƿ����ڲ��ţ�false û�в��Ż���ͣ��ture�����ڲ���
	char szTextureFilePath[MAX_PATH];		//�����ļ���ȫ·��
	CD3D9ManageTexture  st_D3D9ManageTexture; //�����������࣬��IGATexture9.dll����

	STDrawElement()
	{
		memset(szUrl, 0, MAX_PATH );
		memset(szTextureFilePath, 0, MAX_PATH );
		lStart  = 0L;
		bIsShow = false;
		bIsPlay = false;
		bIsClip = false;

		iObjID  = -1;
		iMeshID = -1;
		iSceneID = -1;
		iTextureID   = -1;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		iTextureNum		= 0;
		iTextureIndex	= 0;
		iArrTexture[0]		= -1;

		iMeshSubNum = 0;
		iArrMesh[0] = -1;

		nTotalShowTime		= 0;		
		nStartShowTime	= 0;	
		bStartShow		= false;
		nPickCount		= 0;

	}

	STDrawElement( const STDrawElement& obj )
	{
		strcpy( szUrl, obj.szUrl );
		strcpy( szTextureFilePath, obj.szTextureFilePath );

		lStart  = obj.lStart;
		bIsShow = obj.bIsShow;
		iObjID  = obj.iObjID;
		iMeshID = obj.iMeshID;
		iSceneID = obj.iSceneID;
		iTextureID   = obj.iTextureID;
		x = obj.x;
		y = obj.y;
		z = obj.z;
		s_xyz = obj.s_xyz;
		r_x  = obj.r_x;
		r_y  = obj.r_y;
		r_z  = obj.r_z;
		//------------------------------------
		iTextureNum		= obj.iTextureNum;
		iTextureIndex	= obj.iTextureIndex;
		for( int i = 0; i < iTextureNum; i++ )
		{
			iArrTexture[i]		= obj.iArrTexture[i];
		}
		//--------------------------------------
		iMeshSubNum = obj.iMeshSubNum;
		for( int i = 0; i < iMeshSubNum; i++ )
		{
			iArrMesh[i] = obj.iArrMesh[i];
		}
		nTotalShowTime		= obj.nTotalShowTime;		
		nStartShowTime	= obj.nStartShowTime;	
		bStartShow		= obj.bStartShow;
		nPickCount		= obj.nPickCount;

	}

	void Clear()
	{
		iMeshID = -1;
		iSceneID = -1;
		iTextureID   = -1;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		szUrl[0] = '\0';
		szTextureFilePath[0] = '\0';

		iTextureNum		= 0;
		iTextureIndex	= 0;
		iArrTexture[0]		= -1;

		iMeshSubNum = 0;
		iArrMesh[0] = -1;

		nTotalShowTime		= 0;		
		nStartShowTime	= 0;	
		bStartShow		= false;
		nPickCount		= 0;
	}
};


struct STIGA3DMSG
{
	bool	bIsProc;	//�Ƿ񱻴����
	DWORD	dwType;		//��Ϣ������
	int     iKey;	    //��ֵ
	int		ixPosition;		//����λ��
	int		iyPosition;

	STIGA3DMSG()
	{
		bIsProc = false;
		dwType	= IGA3D_MSG_UNKNOW;
		iKey		= IGA3D_MSG_UNKNOW;
		ixPosition		= 0;
		iyPosition      = 0;
	}

	STIGA3DMSG( const STIGA3DMSG& obj )
	{
		bIsProc		= obj.bIsProc;
		dwType		= obj.dwType;
		iKey		= obj.iKey;
		ixPosition	= obj.ixPosition;
		iyPosition  = obj.iyPosition;
	}

	~STIGA3DMSG()
	{}
};

