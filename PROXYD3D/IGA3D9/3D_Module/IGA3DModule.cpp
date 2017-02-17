/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : 3D_Core.cpp
*	����������IGA3D�ӹ�ʵ����, modify by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#include "stdafx.h"
#include "IGA3D9.h"
#include "z_FileIni.h"
#include "IGA3DModule.h"

float		g_fDistPick		= 10.0f;  //�������
bool		g_bShowPickMesh = false;  // �Ƿ���ʾ����

IGA_SHAREMEMORY_DATA  g_IGAShareMem_Date(_IGA_DATA);	//�����ڴ�

CIGA3DModule g_IGA3DModule;

CIGA3DModule::CIGA3DModule()
:	m_pFun3DFocus(NULL),
	m_pFunDeviceRefCount(NULL),
	m_bUseClip(false),
	m_bCheckXML(true),
	m_dwGameID(0),
	m_hD3D9Search(NULL),
	m_pShowPic(NULL),
	m_hGameWnd(NULL),
	m_bSearchLoad( false )
{
	m_bIsLoad	= false;
	m_bGetMsg	= false;

	m_iPickID = -1;
	m_fDistMin = -1.0f;
	m_bUpdate = false;

	//�����ʼ����־
	m_bInitView		= false;
	m_bInitProj		= false;
	m_bInitWprld	= false;
};

CIGA3DModule::~CIGA3DModule(){}

void CIGA3DModule::ComputeSceneID(){}

//---------------------------------------------------
int CIGA3DModule::Init(HWND hGameWnd, LPDIRECT3DDEVICE9 pd3dDeviceOrig, LPDIRECT3DDEVICE9 pd3dDeviceProxy, LPCallBack3DFocus lpfocus, LPCallBackDeviceRefCount lpRefCount)
{
	m_iDeviceRefCount = 0;

	m_spD3DDevice		= pd3dDeviceOrig;
	m_hGameWnd			=  hGameWnd;
	m_spD3D9DeviceProxy = pd3dDeviceProxy;
	m_pFun3DFocus		= lpfocus;
	m_pFunDeviceRefCount= lpRefCount;
	//m_pD3D9Device = m_spD3DDevice;
	//long lref1 = m_pD3D9Device->AddRef() -1;

	if ( m_pkOriginalStateBlock != NULL )
	{
		m_pkOriginalStateBlock.Release();
	}
	m_spD3DDevice->CreateStateBlock(D3DSBT_ALL, &m_pkOriginalStateBlock);

	if ( m_pkProxyStateBlock != NULL )
	{
		m_pkProxyStateBlock.Release();
	}
	m_spD3DDevice->CreateStateBlock(D3DSBT_ALL, &m_pkProxyStateBlock);

	if(m_pkProxyStateBlock)
	{
		m_pkProxyStateBlock->Capture();
	}

	InitD3D9Search();

	g_CIGA3DMessageManager.Init();
	g_CMeshLibrary.Init( m_spD3DDevice );

	m_dwGameID = ReLoad();
	if (m_pShowPic == NULL )
	{
		m_pShowPic = new CShowPic();
		m_pShowPic->SetDevice9( m_spD3DDevice);
		m_pShowPic->SetGameWnd( m_hGameWnd );
		m_pShowPic->InitPic();
		m_D3D9ManageTexture.D3D9TextureInit( m_spD3DDevice,m_pShowPic->GetPicFileName(), 256, 256 );
		m_D3D9ManageTexture.D3D9StartPlay();

	}
	//long lref2 = m_pD3D9Device->Release();
	//long lrf = lref2 - lref1;

	return Per_Present;

}

BOOL CIGA3DModule::InitD3D9Search()
{
	//////////////////////////////////////////////////////////////////////////
	//����xml�ļ��ж��Ƿ����D3D9Searchģ��
	int iAdID = 0;
	bool bIsLoad = false;
	char szFullPath[MAX_PATH];
	char szModulePath[MAX_PATH];
	GetModuleFileName(NULL,szModulePath,MAX_PATH);
	char *p = NULL;
	p = strrchr(szModulePath,'\\');
	if(p == NULL )
		return false;
	*(p+1) = 0;
	sprintf( szFullPath, "%s%s", szModulePath, "InterAdtive\\AdInfo.xml");

	TiXmlDocument xmlDoc(szFullPath);
	if (xmlDoc.LoadFile()) 
	{
		TiXmlHandle hDoc(&xmlDoc);	//��ô򿪵�XML�ļ��ľ��
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
						if( strcmp(sub_name,"SEARCH") == 0 )
						{
							const char* sub_id = pSub->Attribute("id");
							const char* sub_Closed = pSub->Attribute("Closed");
							iAdID = atoi(sub_id);
							int close = atoi(sub_Closed);
							if ( close == 0 )
							{
								const char * pchAdFile = pSub->Attribute( "AdFile" );
								char szFileNames[MAX_PATH ];
								strcpy(szFileNames, pchAdFile );
								char szFlashFile[MAX_PATH];
								char *needle=";";
								const char* buf = strtok( szFileNames, needle);
								while ( buf != NULL )
								{
									//�ж���Դ�ļ��Ƿ����
									sprintf(szFlashFile, "%s%s%s",szModulePath, ADINFO_EMMOA_RES_FLASH, buf );
									if ( !FileExist(szFlashFile) )
									{
										return false;
									}
									buf = strtok(NULL, needle );
								}

								bIsLoad = true;
							}
						}
					}
				}
			}
		}
	}
	if ( !bIsLoad )
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//��Դ�ļ����ڣ���Ҫ��ʾ������������D3D9Search.dll

	if( m_hD3D9Search == NULL )
	{
		m_hD3D9Search = ::LoadLibraryW(L"D3D9Search.dll");
		m_pFunD3D9SearchInit	= (pFunD3D9SearchInit)::GetProcAddress(m_hD3D9Search, "D3D9SearchInit");
		m_pFunD3D9SearchRender		= (pFunD3D9SearchRender)::GetProcAddress( m_hD3D9Search, "D3D9SearchRender");
		m_pFunD3D9SearchDestory	= (pFunD3D9SearchDestory)::GetProcAddress( m_hD3D9Search, "D3D9SearchDestory");
	}
	if( m_hD3D9Search !=NULL && m_pFunD3D9SearchInit != NULL && m_pFunD3D9SearchRender != NULL && m_pFunD3D9SearchDestory != NULL )
	{
		memset( & m_SearchInitParam, 0, sizeof(tagD3DSearchParam));
		m_SearchInitParam.dwGameID		= m_dwGameID;
		m_SearchInitParam.dwAdID		= (DWORD)iAdID;
		m_SearchInitParam.pFun3DFocus		= m_pFun3DFocus;
		m_SearchInitParam.hWndFocus				=m_hGameWnd;
		m_SearchInitParam.hWndDeviceWindowed	=m_hGameWnd;
		m_SearchInitParam.hWndDeviceFullscreen	= m_hGameWnd;

		if(SUCCEEDED( m_pFunD3D9SearchInit(m_spD3DDevice, (LPARAM)&m_SearchInitParam) ))
		{
			m_bSearchLoad = true;
			return	TRUE;
		}
		else
		{
			m_bSearchLoad = false;
			return FALSE;
		}
	}
	return FALSE;
}


void CIGA3DModule::Destory()
{
	//long lref1 = m_pD3D9Device->AddRef() -1;

	//SaveShowInfo();
	for ( list<STDrawElement>::iterator listIterator = m_listDrawLibrary.begin(); 
		listIterator != m_listDrawLibrary.end(); listIterator ++ )
	{
		listIterator->st_D3D9ManageTexture.D3D9StopPlay();
		listIterator->st_D3D9ManageTexture.D3D9TextureDestory();
	}

	m_D3D9ManageTexture.D3D9StopPlay();
	m_D3D9ManageTexture.D3D9TextureDestory();

	if (m_pShowPic != NULL )
	{
		delete m_pShowPic;
		m_pShowPic = NULL;
	}

	if (m_hD3D9Search != NULL )
	{
		m_pFunD3D9SearchDestory();
		//FreeLibrary(m_hD3D9Search);
		//m_hD3D9Search = NULL;
		//SHOWOUTPUTDEBUG(CIGA3DModule::Destory_m_pFunD3D9SearchDestory);
	}

	m_listDrawLibrary.clear();

	g_CMeshLibrary.Destory();
	g_CIGA3DMessageManager.Destory();

	if ( m_pkOriginalStateBlock != NULL )
	{
		m_pkOriginalStateBlock.Release();
	}
	if ( m_pkProxyStateBlock != NULL )
	{
		m_pkProxyStateBlock.Release();
	}

	m_spD3D9DeviceProxy = NULL;
	m_spD3DDevice = NULL; 


	//long lref2 = m_pD3D9Device->Release();
	//long lrf = lref2 - lref1;
	return;
}


void CIGA3DModule::PerIGA3DRender()
{

	if( g_CIGA3DMessageManager.GetIGA3DMessage( &m_IGA3DMsg ) )
	{
		m_bGetMsg = true;
	}
	if( !m_bIsLoad )
	{
		static long timeLoad = GetTickCount();
		if( GetTickCount() - timeLoad > 10000 )
		{
			g_CMeshLibrary.ClearError();	//��ղ����ڵ�����ID�����¼���
			timeLoad = GetTickCount();
		}
	}
	if(m_pkOriginalStateBlock)
	{
		m_pkOriginalStateBlock->Capture();
	}

	if(m_pkProxyStateBlock)
	{
		m_pkProxyStateBlock->Apply();
	}

	if( m_iDrawPos == Per_Present )
	{
		if( SUCCEEDED( m_spD3DDevice->BeginScene() ) )
		{
			//OutputDebugString("IGA3DModule: beginScene.....");
		}
	}

	//------------------------------------------------------
	m_spD3DDevice->GetFVF(&m_dwFVF);
	m_spD3DDevice->GetRenderState(D3DRS_LIGHTING, &m_dwLighting);
	m_spD3DDevice->GetRenderState(D3DRS_CULLMODE, &m_dwCullMode);
	m_spD3DDevice->GetRenderState(D3DRS_ZENABLE, &m_dwZEnable);
	m_spD3DDevice->GetRenderState(D3DRS_VERTEXBLEND, &m_dwVertexBlend);
	//m_pD3DDevice->GetTexture(0, &m_D3DBaseTexture);
	m_spD3DDevice->GetTextureStageState(0, D3DTSS_COLOROP, &m_dwTexStage0ColorOp);
	m_spD3DDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &m_dwTexStage0ColorArg1);
	m_spD3DDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &m_dwTexStage0ColorArg2);

	m_spD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	m_spD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_spD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_spD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_spD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	m_spD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_spD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );

}

//---------------------------------
//����3Dģ��
void CIGA3DModule::OnIGA3DRender()
{
	//////////////////////////////////////////////////////////////////////////
	if( m_bUseClip )
	{
		g_CIGACamera.ComputeCamera( &m_matView, &m_matProj );
	}

	list<STDrawElement>::iterator listIterator;
	for( listIterator = m_listDrawLibrary.begin(); listIterator != m_listDrawLibrary.end(); listIterator++ )
	{
		long& lStart    = listIterator->lStart;
		bool& bIsShow   = listIterator->bIsShow;
		m_iCurObjID		= listIterator->iObjID;
		int	obj_id      = listIterator->iObjID;
		int mesh_id		= listIterator->iMeshID;
		float x			= listIterator->x;
		float y			= listIterator->y;
		float z			= listIterator->z;

		//�е���Ϸ������ʹ�òü�����ʾ������

		if( m_bUseClip )
		{
			UINT& nTatalShowTime	= listIterator->nTotalShowTime;
			UINT& nStartShowTime	= listIterator->nStartShowTime;
			bool& bStartShow		= listIterator->bStartShow;
			
			if( ! g_CIGACamera.InViewCentrum(&D3DXVECTOR3(x,y,z)) )
			{
				if( bStartShow )
				{
					bStartShow = false;	
					UINT nTime = GetTickCount() - nStartShowTime;
					nTatalShowTime += nTime;			
				}
				//���ü���ʱ����ͣ�����ţ���ʾʱ��������
				if ( listIterator->bIsPlay )
				{
					listIterator->st_D3D9ManageTexture.D3D9PausePlay();
					listIterator->bIsPlay = false;
				}
				continue;
			}
			else if ( ! bStartShow )
			{
				if ( ! listIterator->bIsPlay )
				{
					listIterator->st_D3D9ManageTexture.D3D9UnPausePlay();
					listIterator->bIsPlay = true;
				}
				bStartShow = true;
				nStartShowTime = GetTickCount();		
			}
		}

		float s_xyz     = listIterator->s_xyz;
		float r_x       = listIterator->r_x;
		float r_y       = listIterator->r_y;
		float r_z       = listIterator->r_z;

		int	tex_id		= listIterator->iTextureID;

		int& tex_Num     = listIterator->iTextureNum;
		int& tex_index   = listIterator->iTextureIndex;
		int* tex		 = listIterator->iArrTexture;
		int& iTextureID  = listIterator->iTextureID;

		int& mesh_sub_num = listIterator->iMeshSubNum;
		int* mesh_array   = listIterator->iArrMesh;

		D3DXMATRIX matWorld, matRot, matRotX, matRotY, matRotZ, matScal;
		D3DXMatrixTranslation( &matWorld, x,y,z );

		D3DXMatrixScaling( &matScal, s_xyz, s_xyz, s_xyz );

		D3DXMatrixRotationX( &matRotX, r_x );
		D3DXMatrixRotationY( &matRotY, r_y );
		D3DXMatrixRotationZ( &matRotZ, r_z );
		matRot = matRotX * matRotY * matRotZ;

		matWorld = matScal * matRot * matWorld;


		if(  iTextureID == 1  )
		{
			for( int i = 0; i< mesh_sub_num; i++ )
			{
				//bool	_bIsPlayEnd = false;	
				LPDIRECT3DTEXTURE9 pTex = NULL; 
				pTex = listIterator->st_D3D9ManageTexture.D3D9GetTexture();
				g_CMeshLibrary.Render( mesh_array[i],  &matWorld, pTex, listIterator->szUrl );
			}
		}

		else if( mesh_sub_num > 0 )
		{
			
			for( int i = 0; i< mesh_sub_num; i++ )
			{
				
				g_CMeshLibrary.Render( mesh_array[i],  &matWorld, NULL, NULL );
			}
		}

		/*
		*	��������Ļ���ģ�͵ķ�ʽ�����ڽ���������֮ǰ���������������ȹ��ܣ��������Ѿ���
		*	��Щ���ܶ����ˣ�Ŀǰ�Ŀͻ��˲������������3Dģ�͡�
		*/
		else
		{
			g_CMeshLibrary.Render( mesh_id,  &matWorld, NULL, NULL );
		}
	}
	
	/*
		����Ĵ�����Ϊ�˼�������ģ�ͣ���ÿһ֡ʱ�������ģ�͵ĵ�������ڲ����������������һ��ģ�ͣ�
		����Ĵ������ҳ��Ǹ���ѡ�еĶ��󣬲���ִ����Ӧ�Ĳ�����
		1������ģ�鷢�����ID��
		2�������ģ��ָ��������
		3��֧�ֲü��Ļ�����¼���������
	*/
	STIGA3DMSG msg = g_IGA3DModule.m_IGA3DMsg;
	if( msg.iKey == IGA3D_MSG_LBUTTONDOWN )
	{
		//OutputDebugString("ʰȡ����..........");
		if( m_iPickID > -1 )
		{
			for( listIterator = m_listDrawLibrary.begin(); listIterator != m_listDrawLibrary.end(); listIterator++ )
			{
				//STDrawElement m_DrawElement = (*m_listIterator);
				
				//
				if( m_iUpdataObjID == listIterator->iObjID )
				{
					for( int i = 0; i < listIterator->iMeshSubNum; i++ )
					{
						if( m_iPickID == listIterator->iArrMesh[i] )	//�ҵ�ʰȡ�ϵ�ID
						{
							if( m_pFun3DFocus != NULL ) //����ģ�鷢��ʰȡid
							{
								m_pFun3DFocus(m_iPickID, 0);
							}

							g_IGAShareMem_Date[0].nPickObjID = listIterator->iObjID;

							//�����id��url
							if( listIterator->szUrl[0] != '\0' )
							{
								//if( IS_3D_TOOL )
								//{
								//	OutputDebugString(m_DrawElement.url);
								//}
								//ReleaseCapture();					
								//ShellExecute( GetActiveWindow(), "open", m_DrawElement.url, NULL, NULL, SW_SHOW );
								DWORD dwID = 0;
								::CreateThread(NULL, 0, OpenIEThreadProc, (void*)listIterator->szUrl, 0, &dwID);
								
							}

							if( m_bUseClip )
							{
								listIterator->nPickCount++;
							}
							//OutputDebugString("url.....");
							goto END;
						}
					}
				}
			}
		}	
	}

	//////////////////////////////////////////////////////////////////////////
	//����Ĵ����е������þ���Ӱ����Ϸ�ľ���ֻ�ܷ��������Ȼ�����Ƕ����ģ����ǲ��ܷ������ģ��ǰ�棬
	//����ģ���޷���ʾ
	//////////////////////////////////////////////////////////////////////////
	
	if(  m_hD3D9Search != NULL )
	{
		m_pFunD3D9SearchRender();
	}

	///��ʾ����flash
	m_pShowPic->InitTranslationMatrix();
	m_pShowPic->SetTexture( m_D3D9ManageTexture.D3D9GetTexture() );
	m_pShowPic->RenderPic();

	//ÿһ֡�����ѡ��
END:
	m_iPickID = -1;
	m_iUpdataObjID = -1;
	m_iCurObjID = -2;
}

//---------------------------------
//���ƺ��3D�����ָ�
void CIGA3DModule::PostIGA3DRender()
{
	//GetUI()->Render();

	m_spD3DDevice->SetFVF(m_dwFVF);
	m_spD3DDevice->SetRenderState(D3DRS_LIGHTING, m_dwLighting);
	m_spD3DDevice->SetRenderState(D3DRS_CULLMODE, m_dwCullMode);
	m_spD3DDevice->SetRenderState(D3DRS_ZENABLE, m_dwZEnable);
	m_spD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, m_dwVertexBlend);
	//m_pD3DDevice->SetTexture(0, m_D3DBaseTexture);
	m_spD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, m_dwTexStage0ColorOp);
	m_spD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, m_dwTexStage0ColorArg1);
	m_spD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, m_dwTexStage0ColorArg2);

	//��������
	if( m_iDrawPos == Per_Present )
	{
		m_spD3DDevice->EndScene();
	}

	//״̬�ָ�
    if(m_pkOriginalStateBlock)
	{
		m_pkOriginalStateBlock->Apply();
	}

	//�Ƴ�������Ϣ
	if( m_bGetMsg )
	{
		g_CIGA3DMessageManager.RemoveMsg();
		m_IGA3DMsg.iKey = IGA3D_MSG_UNKNOW;
		m_bGetMsg = false;
	}

	//--------------------------------------------------
	if( g_IGAShareMem_Date[0].bIsReload )

	{
		m_bUpdate = true;
		g_IGAShareMem_Date[0].bIsReload = false;
		g_IGAShareMem_Date.Update();

		m_bCheckXML = false;
	}

	////2008-8-12
	////Ϊ�����ʵʱ����
	if( m_bUpdate )
	{
		m_dwGameID = ReLoad();
		m_bUpdate = false;
	}

	if( g_IGAShareMem_Date[0].bIsShowPickMesh )
	{
		g_bShowPickMesh = true;
	}
	else
	{
		g_bShowPickMesh = false;
	}
}

void CIGA3DModule::SaveShowInfo()
{
	//�����е������ļ�
	//time_t timep;
	//time(&timep);
	//tm* pTime = localtime(&timep);

	//STShowInfoHead	t;
	//t.year		= pTime->tm_year + 1900;
	//t.month		= pTime->tm_mon + 1;
	//t.day		= pTime->tm_mday;

	//t.hour		= pTime->tm_hour;
	//t.minute	= pTime->tm_min;
	//t.sec		= pTime->tm_sec;

	//char fileName[256];
	//sprintf( fileName, "%d_%d_%d.txt", t.year, t.month, t.day );

	////�Ƚ��������ռ�
	//int num = (int)m_listDrawLibrary.size();
	//if( num <= 0 )
	//{
	//	return;	//û�����ݵĻ��ͷ���
	//}
	//
	//t.number = num;
	//STShowInfoHead::ShowInfo* pShowInfo = new STShowInfoHead::ShowInfo[num];
	//if( pShowInfo == NULL )
	//{
	//	return;
	//}

	//int i = 0;
	//for( m_listIterator = m_listDrawLibrary.begin(); m_listIterator != m_listDrawLibrary.end(); m_listIterator++ )
	//{
	//	if( m_listIterator->bStartShow && m_listIterator->StartShowTime > 0 )
	//	{
	//		UINT e_time = GetTickCount() - m_listIterator->StartShowTime;
	//		m_listIterator->ShowTime += e_time;
	//	}

	//	pShowInfo[i].id			= m_listIterator->iObjID;
	//	pShowInfo[i].PickCount	= m_listIterator->PickCount;
	//	pShowInfo[i].ShowTime	= m_listIterator->ShowTime/1000.0f;
	//	i++;

	//	//char text[256];
	//	//sprintf( text, "id:%d ShowTime:%f", m_pDrawNode->obj_id, m_pDrawNode->ShowTime/1000.0f );
	//	//OutputDebugString(text);
	//}

	////�����ݱ�����ļ�
	//ofstream	out;
	//out.open( fileName, ios::app );
	//
	//out<<"----------------------------------------"<<endl;
	//out<<"ͳ��ʱ��: "<<t.year<<"-"<<(int)t.month<<"-"<<(int)t.day<<" "<<(int)t.hour<<":"<<(int)t.minute<<":"<<(int)t.sec<<endl<<endl;
	//for( i = 0; i < num; i++ )
	//{
	//	out<<"����ID:   "<<(int)pShowInfo[i].id<<endl;
	//	out<<"�������: "<<(int)pShowInfo[i].PickCount<<endl;
	//	out<<"��ʾʱ��: "<<pShowInfo[i].ShowTime<<endl<<endl;	
	//}
	//out<<endl;
	//out.close();

	//if( pShowInfo != NULL )
	//{
	//	delete pShowInfo;
	//	pShowInfo = NULL;
	//}
}

int CIGA3DModule::ReLoad()
{
	//�����¼���ǰ��¼��ʾ��Ϣ
	SaveShowInfo();

	char szNameINI[MAX_PATH];
	strcpy( szNameINI, "emmoa.ini" );

	GetModulePath(szNameINI);
	g_CFileIni.Attach(szNameINI);
	DWORD  dwGameID = 0;
	g_CFileIni.GetInt("common","GameID",(int &)dwGameID,0 );

	g_CMeshLibrary.Destory();	//�ͷż��ص�ģ����Դ
	m_listDrawLibrary.clear();

	char szXFilePath[MAX_PATH];
	char szIniFileName[MAX_PATH];
	strcpy( szXFilePath, "InterAdtive\\EMMOA\\3D\\" );
	
	//-----------------------------------------------------
	sprintf( szIniFileName,"%s%s", szXFilePath, "data.ini" );

	GetModulePath( szIniFileName );

	g_CFileIni.Attach(szIniFileName);

	char szTmpBufList[512];
	list<int> listId;
	g_CFileIni.GetString("comm","list",szTmpBufList,512,"" );
	int iOrg = 0;
	int  num = 0;
	char chNum[MAX_PATH] = {'\0'};
	int j = 0;

	while( szTmpBufList[iOrg] != '\0' )
	{
		while( true )
		{
			if( szTmpBufList[iOrg] == ',' || szTmpBufList[iOrg] == '\0'  )
			{
				if( szTmpBufList[iOrg] != '\0' )
				{
					iOrg++;	
					if( iOrg >= 512 )
					{
						return 0;
					}
				}
				break;
			}
			chNum[j] = szTmpBufList[iOrg];
			iOrg++;
			j++;
		}
		chNum[j] = '\0';
		listId.push_back(atoi(chNum));
		j = 0;
		chNum[0] = '\0';
	}

	TCHAR  szTmpBuf[MAX_PATH];
	for( list<int>::iterator iterlistId = listId.begin(); iterlistId != listId.end(); iterlistId++ )
	{
		STDrawElement stDrawElement;
		//memset( & stDrawElement, 0, sizeof( STDrawElement) );
		int index = (int)(*iterlistId);
		TCHAR  szTmpBuf1[MAX_PATH];
		wsprintf(szTmpBuf1,"%u",index );
		if( IS_3D_TOOL )
		{
			OutputDebugString(szTmpBuf1);
		}

		g_CFileIni.GetString(szTmpBuf1,"mesh_id",szTmpBuf,MAX_PATH,"-1" );	//m_DrawElement.mesh_id	= (int)atof(szTmpBuf);
		//�������
		int iOrg = 0;int num = 0;char chNum[MAX_PATH] = {'\0'};int j = 0;
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
			stDrawElement.iArrMesh[num]	= atoi(chNum);
			j = 0;
			chNum[0] = '\0';

			num++;
			if( num >= 100 )
			{
				break;
			}
		}
		stDrawElement.iMeshSubNum = num;

		g_CFileIni.GetString(szTmpBuf1,"IsPaly",szTmpBuf,MAX_PATH,"-1" );	    
		stDrawElement.iTextureID	= (int)atoi(szTmpBuf);

		g_CFileIni.GetString(szTmpBuf1,"s_xyz",szTmpBuf,MAX_PATH,"1.0" );		
		stDrawElement.s_xyz		= (float)atof(szTmpBuf);
		
		g_CFileIni.GetString(szTmpBuf1,"r_x",szTmpBuf,MAX_PATH,"0.0" );		
		stDrawElement.r_x		= (float)atof(szTmpBuf);
		g_CFileIni.GetString(szTmpBuf1,"r_y",szTmpBuf,MAX_PATH,"0.0" );		
		stDrawElement.r_y		= (float)atof(szTmpBuf);
		g_CFileIni.GetString(szTmpBuf1,"r_z",szTmpBuf,MAX_PATH,"0.0" );		
		stDrawElement.r_z		= (float)atof(szTmpBuf);

		g_CFileIni.GetString(szTmpBuf1,"x",szTmpBuf,MAX_PATH,"0.0" );			
		stDrawElement.x			= (float)atof(szTmpBuf);
		g_CFileIni.GetString(szTmpBuf1,"y",szTmpBuf,MAX_PATH,"0.0" );			
		stDrawElement.y			= (float)atof(szTmpBuf);
		g_CFileIni.GetString(szTmpBuf1,"z",szTmpBuf,MAX_PATH,"0.0" );			
		stDrawElement.z			= (float)atof(szTmpBuf);
		g_CFileIni.GetString(szTmpBuf1,"url",stDrawElement.szUrl,MAX_PATH,"" );

		char szTemp[MAX_PATH];
		g_CFileIni.GetString(szTmpBuf1, "VideoFileName", szTemp, MAX_PATH, "" );
		if ( strlen( szTemp ) > 0 )
		{
			sprintf(stDrawElement.szTextureFilePath ,"%s%s", szXFilePath, szTemp  );
			GetModulePath(stDrawElement.szTextureFilePath);
		}
		stDrawElement.iTextureNum = num;
		stDrawElement.iObjID  = index;
		m_listDrawLibrary.push_back(stDrawElement);
	}

	//list<STDrawElement>::iterator	listIterator;
	for ( list<STDrawElement>::iterator listIterator = m_listDrawLibrary.begin(); 
		listIterator != m_listDrawLibrary.end(); listIterator ++ )
	{
		if ( lstrlen( listIterator->szTextureFilePath ) == 0)
		{
			continue;
		}
		listIterator->st_D3D9ManageTexture.D3D9TextureInit(m_spD3DDevice,listIterator->szTextureFilePath, 256, 256);
		listIterator->st_D3D9ManageTexture.D3D9StartPlay();
		listIterator->st_D3D9ManageTexture.D3D9PausePlay();
		listIterator->bIsPlay = false;
	}


	switch (dwGameID)
	{
	case IGA_FENGYICHUANSHUO: //��ӡ��˵
		g_fDistPick = 2000.0f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(8000.0f);
		break;
	case IGA_SHENQI:	//����
		g_fDistPick = 20.0f;
		m_bUseClip = true;
		break;
	case IGA_SHUSHAN :	//��ɽ
		g_fDistPick = 2000.0f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(8000.0f);
		break;
	case IGA_YINGTAOCHENG :	//Ӥ�Գ�
		g_fDistPick = 2000.0f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(8000.0f);
		break;
	case IGA_GV:	//�󺽺�ʱ��
		g_fDistPick = 2000.0f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(8000.0f);
		break;
	case IGA_WLOL:	//����Ⱥ����
		g_fDistPick = 450.0f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(1600.0f);
		break;
	case IGA_SUN:	//�漣����
		g_fDistPick = 20.0f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(50.0f);
		break;
	case IGA_RELIPAIQIU: //��������
		g_fDistPick = 7.5f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(40.0f);
		break;
	case IGA_PAOPAOKADINGCHE: //���ܿ�����
		g_fDistPick = 20.0f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(100.0f);
		break;
	case IGA_TEST:
		g_fDistPick = 20.0f;
		m_bUseClip = true;
		g_CIGACamera.SetClipDistance(100.0f);
		break;

	}

	g_IGAShareMem_Date[0].nPickObjID = -1;

	return dwGameID;
}

void CIGA3DModule::CheckAdInfo()
{
	//STDrawElement	m_DrawElement;
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
						list<STDrawElement>::iterator listIterator;
						for( listIterator = m_listDrawLibrary.begin(); listIterator != m_listDrawLibrary.end(); listIterator++)
						{
							//STDrawElement m_DrawElement	= (*listIterator);
							int mesh_id		= listIterator->iObjID;
	
							for( LP3DXML p = xml_pool.begin(); p != xml_pool.end(); p++ )
							{
								int obj_id = p->id;
								if( mesh_id == obj_id )
								{
									listIterator->bIsShow = true;
									if( p->Close == 1 )
									{
										listIterator->bIsShow = false;
										break;
									}
								}
							}
						}

						for( listIterator = m_listDrawLibrary.begin(); listIterator != m_listDrawLibrary.end(); listIterator++)
						{
							if(!listIterator->bIsShow)
							{
								m_listDrawLibrary.erase( listIterator );
							}
						}
					}
					else
					{
					    //AdInfo.xmlû��3Dģ��Ҫ��ʾ
						m_listDrawLibrary.clear();
					}
				}
			}
		}
		else
		{
			//OutputDebugString("AdInfo.xml ��ʧ�ܣ�");
			//OutputDebugString(fullPath);
			
            //AdInfo.xml����ʧ��Ҳ����ʾ
			m_listDrawLibrary.clear();
		}
	}
}

//--------------------------------------------------
//����״̬
void CIGA3DModule::IGA3DInputState()
{
	g_CIGA3DMessageManager.UpdataMessage();	
}

//IGA3D�ű�����
void CIGA3DModule::RunScript()
{	
	//------------------------------------------------------
	//���нű�
	//GetScript()->RunScript();

	if( IS_3D_TOOL && GetAsyncKeyState('C') )
	{
		GetCameraPos();
	}
}

void GetCameraPos()
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

	//char text[MAX_PATH];
	//sprintf( text, "Camera x:%3.2f, y:%3.2f, z:%3.2f", vCamera.x, vCamera.y, vCamera.z );
	//OutputDebugString(text);
}