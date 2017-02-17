

#include "stdafx.h"
#include <shellapi.h>
#include "tchar.h"

#include "define.h"
#include "z_FileIni.h"
#include "IGA3D_UI.h"
#include "IGACamera.h"
#include "3D_Core.h"



float g_floatPick = 10.0f;
bool  g_bShowPickMesh = false;
bool  g_bCheckXML = true;

IGA_SHAREMEM_DATA	g_IGAShareMemory(_IGA_DATA);
IGA3DModule			g_IGA3DModule;


IGA3DModule::IGA3DModule()
{ 
	m_pkFxSkelMesh = NULL; 
	m_enumGameID = IGA_NO_GAME;
	m_pFun = NULL;
	m_pFun3DFocus = NULL;
	m_pDeviceRefCount = NULL;
	m_iCurrentScene = -1;
	m_PickID = -1;
	m_fDistMin = -1.0f;

	m_hModule			= NULL;
	m_pFunInit			= NULL;
	m_pFunGetTexture	= NULL;
	m_pFunDestory		= NULL;
	
	m_bUseClip = false;
	m_bIsLoad = false;
	m_bUpdate = false;
	m_bIsLoad	= false;
	m_bGetMsg	= false;
	m_bIGATexture	= false;

	m_lStart = GetTickCount();
};

IGA3DModule::~IGA3DModule()
{
	if ( NULL != m_hModule )
	{
		FreeLibrary( m_hModule );
		m_hModule = NULL;
	}

}

//---------------------------------------------------
int IGA3DModule::Init()
{	
	g_CIGA3DMessageManager.Init();
	g_CMeshLibrary.Init(m_pd3dDevice);

	m_hModule = ::LoadLibrary(TEXT("IGATexture8.dll"));
	if( NULL != m_hModule )
	{
		m_pFunInit			= (pFunInit)::GetProcAddress( m_hModule, _T("IGATextureInit"));
		m_pFunGetTexture	= (pFunGetTexture)::GetProcAddress( m_hModule, _T("IGAGetTexture"));
		m_pFunDestory		= (pFunDestroy)::GetProcAddress( m_hModule, _T("IGATextureDestory"));
		m_pFunStopPlayer	= (pFunStopPlayer)GetProcAddress( m_hModule, TEXT("StopPlayer"));
		
		if( m_pFunInit != NULL && m_pFunGetTexture != NULL && m_pFunDestory != NULL )
		{
			if( SUCCEEDED( m_pFunInit( m_pd3dDevice ) ) )
			{
				m_bIGATexture = true;	
			}
		}
	}

	ReLoad();

	////����ԡ����������Ϊ�˲���ģ����ʾ��UI���档

	//if( m_enumGameID == IGA_WUJIEQU				//���������
	//|| m_enumGameID == IGA_WUJIEQU_MLXY			//�����(��������)
	//|| m_enumGameID == IGA_WUJIEQU_XJP			//�����(�¼���)
	//|| m_enumGameID == IGA_WUJIEQU_FLB)			//�����(���ɱ�)
	//{
	//	m_iDrawPos = Per_SetTransform;
	//}
	//else
	//{
	//	m_iDrawPos = Per_Present;
	//}
	m_iDrawPos = Per_Present;
	return m_iDrawPos;
}

void IGA3DModule::Destory()
{
	SaveShowInfo();

	if( m_bIGATexture )
	{
		m_pFunDestory();
		m_bIGATexture = false;
	}

	if ( NULL != m_hModule )
	{
		FreeLibrary( m_hModule );
		m_hModule = NULL;
	}

	if ( ! m_listDrawLibrary.empty())
	{
		m_listDrawLibrary.clear();
	}

	g_CMeshLibrary.Destory();
	g_CIGA3DMessageManager.Destory();

}


//����֮ǰ��3D����״̬�ռ�
void IGA3DModule::PerIGA3DRender()
{
	m_lElapseTime = GetTickCount() - m_lStart;
	
	if( g_CIGA3DMessageManager.GetIGA3DMessage( &m_Msg ) )
	{
		m_bGetMsg = true;
	}

	if( ! m_bIsLoad )
	{
		static long timeLoad = GetTickCount();
		if( GetTickCount() - timeLoad > 10000 )
		{
			g_CMeshLibrary.ClearError();	//��ղ����ڵ�����ID�����¼���
			timeLoad = GetTickCount();
			m_bIsLoad = true;
		}
		//return;
	}
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, false);
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false);

	if( m_iDrawPos == Per_Present )
	{
		m_pd3dDevice->BeginScene();
	}
}

//---------------------------------
//����3Dģ��
void IGA3DModule::OnIGA3DRender()
{
	if( m_bUseClip )
	{
		g_CIGACamera.ComputeCamera( &m_matView, &m_matProj );
	}

	//OutputDebugString("IGA3DModule::OnIGA3DRender");
	for( m_listIterator = m_listDrawLibrary.begin(); m_listIterator != m_listDrawLibrary.end(); m_listIterator++ )
	{
		//OutputDebugString("In m_listDrawLibrary");
		m_stDrawElement	= (*m_listIterator);
		m_CurObjID = m_stDrawElement.m_iObjID;
		int mesh_id		= m_stDrawElement.m_iMeshID;
		float x			= m_stDrawElement.x;
		float y			= m_stDrawElement.y;
		float z			= m_stDrawElement.z;

		int& mesh_sub_num = m_stDrawElement.mesh_sub_num;
		int* mesh_array   = m_stDrawElement.mesh_array;

		//�е���Ϸ������ʹ�òü�����ʾ������
		if(  m_bUseClip )
		{
			UINT& ShowTime		= m_listIterator->ShowTime;
			UINT& StartShowTime = m_listIterator->StartShowTime;
			bool& bStartShow	= m_listIterator->bStartShow;
			
			if( !(g_CIGACamera.InViewCentrum(&D3DXVECTOR3(x,y,z))) )
			{
				if( bStartShow )
				{
					OutputDebugString("m_pFunStopPlayer");
					/*for( int i = 0; i< mesh_sub_num; i++ )
					{	
						m_pFunStopPlayer( mesh_array[i]);
					}*/
					bStartShow = false;
					UINT e_time = GetTickCount() - StartShowTime;
					ShowTime += e_time;			
				}
				continue;
			}

			if(!bStartShow)
			{
				bStartShow = true;
				StartShowTime = GetTickCount();		
			}
		}

		float s_xyz     = m_stDrawElement.s_xyz;
		float r_x       = m_stDrawElement.r_x;
		float r_y       = m_stDrawElement.r_y;
		float r_z       = m_stDrawElement.r_z;

		int& tex_Num     = m_stDrawElement.tex_Num;
		int& tex_index   = m_listIterator->tex_Index;
		int* tex		 = m_stDrawElement.tex;

		char szOutputDebug[256];
		sprintf(szOutputDebug, "mesh_sub_num = %d", mesh_sub_num );
		OutputDebugString(szOutputDebug );

		D3DXMATRIX matWorld, matRot, matRotX, matRotY, matRotZ, matScal;
		D3DXMatrixTranslation( &matWorld, x,y,z );

		D3DXMatrixScaling( &matScal, s_xyz, s_xyz, s_xyz );

		D3DXMatrixRotationX( &matRotX, r_x );
		D3DXMatrixRotationY( &matRotY, r_y );
		D3DXMatrixRotationZ( &matRotZ, r_z );
		matRot = matRotX * matRotY * matRotZ;
		matWorld = matScal * matRot * matWorld;


		if(  m_bIGATexture )
		{
			//OutputDebugString("begin m_pFunGetTexture");
			for( int i = 0; i< mesh_sub_num; i++ )
			{	
				bool bIsPlayEnd = false;	

				int iWidth = 512;
				int iHeight = 256;
				LPDIRECT3DTEXTURE8 pTex =NULL;
				pTex = m_pFunGetTexture(mesh_array[i], &bIsPlayEnd, &iWidth, &iHeight );
				char szOutputDebugString[256];
				sprintf(szOutputDebugString, "mesh_array[i]  = %d", mesh_array[i] );
				OutputDebugString(szOutputDebugString);
				if( bIsPlayEnd )
				{
					tex_index < tex_Num-1 ? tex_index++: tex_index = 0;
				}
				g_CMeshLibrary.Render(  mesh_array[i],  &matWorld, pTex, NULL );
			}

			//OutputDebugString("end m_pFunGetTexture");
		}

		else if( mesh_sub_num > 0 )	//�����������Ļ���
		{
			for( int i = 0; i< mesh_sub_num; i++ )
			{
				char szOutput[256];
				sprintf(szOutput, "medsh_ID = %d",mesh_array[i] );
				OutputDebugString(szOutput);
				g_CMeshLibrary.Render( mesh_array[i],  &matWorld, NULL, NULL );
			}
		}
		else
		{
			g_CMeshLibrary.Render( mesh_id,  &matWorld, NULL, m_stDrawElement.szURL );
		}
	}

	/*
		����Ĵ�����Ϊ�˼�������ģ�ͣ���ÿһ֡ʱ�������ģ�͵ĵ�������ڲ����������������һ��ģ�ͣ�
		����Ĵ������ҳ��Ǹ���ѡ�еĶ��󣬲���ִ����Ӧ�Ĳ�����
		1������ģ�鷢�����ID��
		2�������ģ��ָ��������
		3��֧�ֲü��Ļ�����¼���������
	*/

	SIGA3DMSG msg = g_IGA3DModule.m_Msg;
	if( msg.iKey == IGA3D_MSG_RBUTTONDOWN )
	{
		if( m_PickID != -1 )
		{
			for( m_listIterator = m_listDrawLibrary.begin(); m_listIterator != m_listDrawLibrary.end(); m_listIterator++ )
			{
				m_stDrawElement = (*m_listIterator);
				
				//
				if( m_UpdataObjID == m_listIterator->m_iObjID )
				{
					for( int i = 0; i < m_stDrawElement.mesh_sub_num; i++ )
					{
						if( m_PickID == m_stDrawElement.mesh_array[i] )	//�ҵ�ʰȡ�ϵ�ID
						{
							if( m_pFun3DFocus != NULL ) //����ģ�鷢��ʰȡid
							{
								m_pFun3DFocus(m_PickID, 0);
							}
							g_IGAShareMemory[0].nPickObjID = m_listIterator->m_iObjID;

							//�����id��url
							if( m_stDrawElement.szURL[0] != '\0' )
							{
								if( IS_3D_TOOL )
								{
									OutputDebugString(m_stDrawElement.szURL);
								}
								ShellExecute( GetActiveWindow(), "open", m_stDrawElement.szURL, NULL, NULL, SW_SHOW );
							}

							if( m_bUseClip )
							{
								m_listIterator->PickCount++;
							}
								
							goto END;
						}
					}
				}
			}
			//char text[MAX_PATH];
			//sprintf( text, "m_PickObjID: %u", m_PickID );
			//OutputDebugString(text);
		}
	}
END:
	//ÿһ֡�����ѡ��
	m_PickID = -1;
	m_UpdataObjID = -1;
	m_CurObjID = -2;
}

//---------------------------------
//���ƺ��3D�����ָ�
void IGA3DModule::PostIGA3DRender()
{
	//��������
	if( m_iDrawPos == Per_Present )
	{
		m_pd3dDevice->EndScene();
	}
	//m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, true);

	//�Ƴ�������Ϣ
	if( m_bGetMsg )
	{
		g_CIGA3DMessageManager.RemoveMsg();
		m_Msg.iKey = IGA3D_MSG_UNKNOW;
		m_bGetMsg = false;
	}

	//--------------------------------------------------
	static bool _bLoad = true;
	if( IS_3D_TOOL && GetAsyncKeyState('N') || g_IGAShareMemory[0].bIsReload )
	{
		if( _bLoad )
		{
			m_bUpdate = true;
			_bLoad = false;

			if( g_IGAShareMemory[0].bIsReload )
			{
				g_bCheckXML = false;
			}

			g_IGAShareMemory[0].bIsReload = false;
			g_IGAShareMemory.Update();
		}
	}
	else
	{
		_bLoad = true;
	}

	//2008-8-12
	//Ϊ�����ʵʱ����
	if( m_bUpdate )
	{
		ReLoad();
		m_bUpdate = false;
	}

	if( IS_3D_TOOL && GetAsyncKeyState('Z') )
	{
		g_bShowPickMesh = true;
	}
	else
	{
		g_bShowPickMesh = false;
	}

}

void IGA3DModule::SaveShowInfo()
{
	if( true || !m_bUseClip )
	{
		return;
	}

	time_t timep;
	time(&timep);
	tm* pTime = localtime(&timep);

	STShowInfoHead	t;
	t.year		= pTime->tm_year + 1900;
	t.month		= pTime->tm_mon + 1;
	t.day		= pTime->tm_mday;

	t.hour		= pTime->tm_hour;
	t.minute	= pTime->tm_min;
	t.sec		= pTime->tm_sec;

	char fileName[256];
	sprintf( fileName, "%d_%d_%d.txt", t.year, t.month, t.day );

	//�Ƚ��������ռ�
	int num = (int)m_listDrawLibrary.size();
	if( num <= 0 )
	{
		return;	//û�����ݵĻ��ͷ���
	}
	
	t.number = num;
	STShowInfoHead::ShowInfo* pShowInfo = new STShowInfoHead::ShowInfo[num];
	if( pShowInfo == NULL )
	{
		return;
	}

	int i = 0;
	for( m_listIterator = m_listDrawLibrary.begin(); m_listIterator != m_listDrawLibrary.end(); m_listIterator++ )
	{
		if( m_listIterator->bStartShow && m_listIterator->StartShowTime > 0 )
		{
			UINT e_time = GetTickCount() - m_listIterator->StartShowTime;
			m_listIterator->ShowTime += e_time;
		}

		pShowInfo[i].id			= m_listIterator->m_iObjID;
		pShowInfo[i].PickCount	= m_listIterator->PickCount;
		pShowInfo[i].ShowTime	= m_listIterator->ShowTime/1000.0f;
		i++;

		//char text[256];
		//sprintf( text, "id:%d ShowTime:%f", m_pDrawNode->obj_id, m_pDrawNode->ShowTime/1000.0f );
		//OutputDebugString(text);
	}

	//�����ݱ�����ļ�
	ofstream	out;
	out.open( fileName, ios::app );
	
	out<<"----------------------------------------"<<endl;
	out<<"ͳ��ʱ��: "<<t.year<<"-"<<(int)t.month<<"-"<<(int)t.day<<" "<<(int)t.hour<<":"<<(int)t.minute<<":"<<(int)t.sec<<endl<<endl;
	for( i = 0; i < num; i++ )
	{
		out<<"����ID:   "<<(int)pShowInfo[i].id<<endl;
		out<<"�������: "<<(int)pShowInfo[i].PickCount<<endl;
		out<<"��ʾʱ��: "<<pShowInfo[i].ShowTime<<endl<<endl;	
	}
	out<<endl;
	out.close();

	if( pShowInfo != NULL )
	{
		delete pShowInfo;
		pShowInfo = NULL;
	}
}

int IGA3DModule::ReLoad()
{
	/*
		�����¼���ǰ��¼��ʾ��Ϣ,�����浽�ļ��У������֧�ֲü������ｫ��ִ���κβ���
	*/
	SaveShowInfo();
	m_stDrawElement.Clear();

	//��emmoa.ini�л��game_ID
	char NameINI[MAX_PATH];
	strcpy( NameINI, "emmoa.ini" );
	GetModulePath(NameINI);
	g_ZFileIni.Attach(NameINI);
	int  iGameID = 0;
	g_ZFileIni.GetInt("common","GameID",iGameID,0 );
	m_enumGameID = (ENUM_IGA_GAME_LIST)iGameID;
	g_CMeshLibrary.Destory();	//�ͷż��ص�ģ����Դ
	
	if ( ! m_listDrawLibrary.empty())
	{
		m_listDrawLibrary.clear();
	}

	char szXFilePath[MAX_PATH];
	char szIniFileName[MAX_PATH];
	strcpy( szXFilePath, "InterAdtive\\EMMOA\\3D\\" );
	sprintf( szIniFileName,"%s%s", szXFilePath, "data.ini" );
	GetModulePath( szIniFileName );
	g_ZFileIni.Attach(szIniFileName);
	//int  iGameId = 0;
	//g_ZFileIni.GetInt("comm","gameid",iGameId,0 );
	
	if( IS_3D_TOOL )
	{
		char gid[MAX_PATH];
		sprintf( gid, "game_id: %d", iGameID );
		OutputDebugString(gid);
	}

	char szTmpBufList[MAX_PATH];
	list<int> listGameList;
	g_ZFileIni.GetString("comm","list",szTmpBufList,MAX_PATH,"" );
	int iOrg = 0;
	int  num = 0;
	char szNum[MAX_PATH] = {'\0'};
	int j = 0;
	//OutputDebugString(szTmpBufList);
	while( szTmpBufList[iOrg] != '\0' )
	{
		while(1)
		{
			if( szTmpBufList[iOrg] == ',' || szTmpBufList[iOrg] == '\0'  )
			{
				if( szTmpBufList[iOrg] != '\0' )
				{
					iOrg++;	
					if( iOrg >= 512 )
					{
						if( IS_3D_TOOL )
						{
							OutputDebugString("OUT_MEMRARY: iOrg >= MAX_PATH......");
						}
						return 0;
					}
				}
				break;
			}
			szNum[j] = szTmpBufList[iOrg];
			iOrg++;
			j++;
		}
		szNum[j] = '\0';
		listGameList.push_back(atoi(szNum));
		j = 0;
		szNum[0] = '\0';
	}

	TCHAR  szTmpBuf[MAX_PATH];
	for( list<int>::iterator i = listGameList.begin(); i != listGameList.end(); i++ )
	{
		int index = (int)(*i);
		TCHAR  szTmpBuf1[MAX_PATH];
		wsprintf(szTmpBuf1,"%u",index );
		m_stDrawElement.m_iObjID  = index;
		if( IS_3D_TOOL )
		{
			OutputDebugString(szTmpBuf1);
		}

		g_ZFileIni.GetString(szTmpBuf1,"mesh_id",szTmpBuf,MAX_PATH,"-1" );	//m_DrawElement.mesh_id	= (int)atof(szTmpBuf);OutputDebugString(szTmpBuf);
		//�������
		int iOrg = 0;
		int num = 0;
		char chNum[MAX_PATH] = {'\0'};
		int j = 0;
		while( szTmpBuf[iOrg] != '\0' )
		{
			while(1)
			{
				if( szTmpBuf[iOrg] == ',' || szTmpBuf[iOrg] == '\0'  )
				{
					if( szTmpBuf[iOrg] != '\0' )
					{
						iOrg++;	
					}
					break;
				}
				chNum[j] = szTmpBuf[iOrg];
				iOrg++;
				j++;
			}
			chNum[j] = '\0';
			m_stDrawElement.mesh_array[num]	= atoi(chNum);
			j = 0;
			chNum[0] = '\0';
			
			sprintf( chNum, "num: %d  index: %d ", num, m_stDrawElement.mesh_array[num]);
			
			num++;
			if( num >= 100 )
			{
				break;
			}
		}
		m_stDrawElement.mesh_sub_num = num;
				

		g_ZFileIni.GetString(szTmpBuf1,"scene_id",szTmpBuf,MAX_PATH,"-1" );
		m_stDrawElement.scene_id	= (int)atof(szTmpBuf);
		g_ZFileIni.GetString(szTmpBuf1,"tex_id",szTmpBuf,MAX_PATH,"-1" );	    
		m_stDrawElement.tex_id	= (int)atof(szTmpBuf);

		g_ZFileIni.GetString(szTmpBuf1,"s_xyz",szTmpBuf,MAX_PATH,"1.0" );		
		m_stDrawElement.s_xyz		= (float)atof(szTmpBuf);
		
		g_ZFileIni.GetString(szTmpBuf1,"r_x",szTmpBuf,MAX_PATH,"0.0" );		
		m_stDrawElement.r_x		= (float)atof(szTmpBuf);
		g_ZFileIni.GetString(szTmpBuf1,"r_y",szTmpBuf,MAX_PATH,"0.0" );		
		m_stDrawElement.r_y		= (float)atof(szTmpBuf);
		g_ZFileIni.GetString(szTmpBuf1,"r_z",szTmpBuf,MAX_PATH,"0.0" );		
		m_stDrawElement.r_z		= (float)atof(szTmpBuf);

		g_ZFileIni.GetString(szTmpBuf1,"x",szTmpBuf,MAX_PATH,"0.0" );			
		m_stDrawElement.x			= (float)atof(szTmpBuf);
		g_ZFileIni.GetString(szTmpBuf1,"y",szTmpBuf,MAX_PATH,"0.0" );			
		m_stDrawElement.y			= (float)atof(szTmpBuf);
		g_ZFileIni.GetString(szTmpBuf1,"z",szTmpBuf,MAX_PATH,"0.0" );			
		m_stDrawElement.z			= (float)atof(szTmpBuf);

		g_ZFileIni.GetString(szTmpBuf1,"url",m_stDrawElement.szURL,MAX_PATH,"" );

		if( IS_3D_TOOL )
		{
			//char text[MAX_PATH];
			//sprintf( text, "mesh_id: %d, x: %f, y: %f, z:%f, s_xyz:%f, r_x:%f, r_y:%f, r_z:%f, url:%s", m_DrawElement.mesh_id, m_DrawElement.x,m_DrawElement.y,m_DrawElement.z, m_DrawElement.s_xyz, m_DrawElement.r_x, m_DrawElement.r_y, m_DrawElement.r_z, m_DrawElement.url );
			//OutputDebugString(text);
		}
		m_listDrawLibrary.push_back(m_stDrawElement);
		//OutputDebugString("m_listDrawLibrary.push_back");
	}

	if( g_bCheckXML )
	{
		CheckAdInfo();
	}

	if( iGameID == IGA_WUJIEQU				//���������
	 || iGameID == IGA_WUJIEQU_MLXY			//�����(��������)
	 || iGameID == IGA_WUJIEQU_XJP			//�����(�¼���)
	 || iGameID == IGA_WUJIEQU_FLB)			//�����(���ɱ�)
	{
		g_floatPick = 10.0f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(50.0f);
	}
	else if( iGameID == IGA_TIANZUN ) //������
	{
		//g_fPick = 1000.0f;
		//m_bUseClip = true;
		//GetIGACamera()->SetClipDistance(8000.0f);
	}

	g_IGAShareMemory[0].nPickObjID = -1;

	return iGameID;
}


void IGA3DModule::CheckAdInfo()
{
	if( !m_listDrawLibrary.empty() )	
	{
		//�򿪲�����XML�ļ�
		char fullPath[MAX_PATH];
		strcpy(fullPath,"InterAdtive\\AdInfo.xml");
		GetModulePath(fullPath);
		TiXmlDocument doc(fullPath);
		if (doc.LoadFile()) 
		{
			TiXmlHandle hDoc(&doc);	//��ô򿪵�XML�ļ��ľ��
			TiXmlElement* pElem;	//����XML�ļ���Ԫ��
			TiXmlHandle hRoot(0);	//����XML�ļ���Ŀ¼
			pElem = hDoc.FirstChildElement().Element();	//Ѱ��XML�ļ��ĵ�һ��Ԫ��
			if( pElem != NULL )
			{
				//��ȡ�������һ����Ԫ�ص�Ŀ¼
				hRoot = TiXmlHandle(pElem);
				
				//�õ����Ԫ���е���Ԫ��
				TiXmlElement* pSub = hRoot.FirstChild( "IGA_2D" ).FirstChild().Element();
				if( pSub != NULL )
				{
					typedef list<_S3DXML>	_3DXML_POOL;
					typedef list<_S3DXML>::iterator LP3DXML;
					_3DXML_POOL	xml_pool;

					//��ȡ����
					for( ; pSub != NULL; pSub = pSub->NextSiblingElement("AD"))
					{
						const char* sub_name = pSub->Attribute("AdType");
						if( sub_name != NULL )
						{
							if( strcmp(sub_name,"3D") == 0 )
							{
								const char* sub_id = pSub->Attribute("id");
								const char* sub_Closed = pSub->Attribute("Closed");
								int id = atoi(sub_id);
								int close = atoi(sub_Closed);
							
								_S3DXML obj;
								obj.id = id;
								obj.Close = close;
								xml_pool.push_back(obj);
							}
						}
					}
					
					//���бȽ�
					if(!xml_pool.empty())
					{
						//Ѱ��ƥ���id
						for( m_listIterator = m_listDrawLibrary.begin(); m_listIterator != m_listDrawLibrary.end(); m_listIterator++)
						{
							m_stDrawElement	= (*m_listIterator);
							int mesh_id		= m_stDrawElement.m_iObjID;
	
							for( LP3DXML p = xml_pool.begin(); p != xml_pool.end(); p++ )
							{
								int obj_id = p->id;
								if( mesh_id == obj_id )
								{
									m_listIterator->m_bIsShow = true;
									if( p->Close == 1 )
									{
										m_listIterator->m_bIsShow = false;
										break;
									}
								}
							}
						}

						//ɾ������ʾ��Ԫ��
						for( m_listIterator = m_listDrawLibrary.begin(); m_listIterator != m_listDrawLibrary.end(); )
						{
							m_stDrawElement	= (*m_listIterator);
							list<STDrawElement>::iterator p = m_listIterator;
							m_listIterator++;

							if(!p->m_bIsShow)
							{
								//char text[MAX_PATH];
								//sprintf(text, "delete id: %d", p->obj_id);
								//OutputDebugString(text);
								m_listDrawLibrary.erase(p);
							}
						}

					}
					//else
					//{
					//	//AdInfo.xmlû��3Dģ��Ҫ��ʾ
					//	
					//	if ( ! m_listDrawLibrary.empty())
					//	{
					//		m_listDrawLibrary.clear();
					//	}
					//}
				}
			}
		}
		else
		{
			if( IS_3D_TOOL )
			{
				OutputDebugString("AdInfo.xml ��ʧ�ܣ�");
				OutputDebugString(fullPath);
			}
   //         //AdInfo.xml����ʧ��Ҳ����ʾ
			//if ( ! m_listDrawLibrary.empty())
			//{
			//	m_listDrawLibrary.clear();
			//}
			
		}
	}
}

//--------------------------------------------------
//����״̬
void IGA3DModule::IGA3DInputState()
{
	g_CIGA3DMessageManager.UpdataMessage();
}

//IGA3D�ű�����
void IGA3DModule::RunScript()
{
	//------------------------------------------------------
	//���нű�
	//GetScript()->RunScript();

	if( IS_3D_TOOL && GetAsyncKeyState('C') )
	{
		GetCameraPos();
	}
}

void IGA3DModule::GetCameraPos()
{
	D3DXMATRIX matView,matProj, matCombViewProj, matInv;
	D3DXVECTOR3 vCamera(0.0f,0.0f,0.0f);
	//D3DXMatrixIdentity(&matView);
	//D3DXMatrixIdentity(&matProj);

	matView = g_IGA3DModule.m_matView;
	matProj = g_IGA3DModule.m_matProj;

	matCombViewProj = matView * matProj;

	D3DXMatrixInverse(&matInv, NULL, &matCombViewProj );

	D3DXVec3TransformCoord( &vCamera, &vCamera, &matInv );

	char text[MAX_PATH];
	sprintf( text, "Camera x:%3.2f, y:%3.2f, z:%3.2f", vCamera.x, vCamera.y, vCamera.z );
	OutputDebugString(text);
}
