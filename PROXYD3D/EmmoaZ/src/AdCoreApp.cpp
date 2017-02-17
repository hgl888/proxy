#include "stdafx.h"

#include "AdCoreApp.h"
#include "AdCoreDef.h"
#include "z_File.h"
#include "z_fileini.h" 
#include "IGAWin.h"
#include "GlobalAPI_Val.h"


/*��������*/
KAdCoreApp*  CreateADApp()
{
   static KAdCoreApp   core;
   return &core;
}

KAdCoreApp::KAdCoreApp():
         m_net(NULL)
{
    Init();
}
KAdCoreApp::~KAdCoreApp()
{
	LOG << "KAdCoreApp::~KAdCoreApp()";
}

/*��ʼ�ӹ�*/
void*  KAdCoreApp::HookD3D   ( void*  pD3D)
{
	static  IGA_Filter_IDirect3D    filterD3D;
	m_3Dfilter.pFilterD3D             = &filterD3D;
	
	static  IGA_Filter_IDirect3DDevice  filterD3DDevice;
	m_3Dfilter.pFilterDevice        = &filterD3DDevice; 
	
	static  IGA_Filter_IDirect3DSurface  filterD3DSurface;
	m_3Dfilter.pFilterSurface        = &filterD3DSurface; 
	//m_3Dfilter.pFilterSurface        = NULL;
	
	static  IGA_Filter_IDirect3DTexture  filterD3DTexture;
	m_3Dfilter.pFilterTexture        = &filterD3DTexture; 	

#if defined(USING_D3D8)
	return InitD3D8(m_3Dfilter,(IDirect3D*)pD3D);
#elif defined(USING_D3D9)
    return InitD3D9(m_3Dfilter,(IDirect3D*)pD3D);
#endif
}

INT  KAdCoreApp::OnInitThread()
{	
	m_hardware.Load();  
	
	BOOL  bRet =  QuerySDK()        ;
	g_bRightIP = bRet;
	

	/* ��ѯ���� */
	bRet =    QueryDriver()         ;

	/* ��ѯ��� */
	bRet =    QueryPlugin()         ;

	/* ��ѯ���� */
	if( QueryConfig() )
	{
		LoadConfig(); 
	}

	/* ��ѯ������� */
	bRet =    QueryAdInfo()         ;

	//Mazhenhui 2008-11-14   ���ұ��ع���ļ��Ƿ���Ҫ����
	if  ( bRet )
	{      
		AD_XML_Info::AD_RECORD_IT    it ;
		for (   it  = m_adManager.m_xml.m_ads.begin() ; it != m_adManager.m_xml.m_ads.end() ; it ++ )
		{	  
			AD_XML_RECORD   xmlRS  = *it ;
			
			if ( !m_adManager.IsNeedUpdate(xmlRS , FALSE ) )  continue;
			
			START_LOG ;
			LOG  << "++UPDATE LOCALFILE  ID= "  << xmlRS.dwID ;
			DownUpdateAD(xmlRS );
			END_LOG   ;
		}		
	}
	
	/* �ϴ�Ӳ����Ϣ */
    bRet =    UploadHardwareInfo()  ;

    /* �ϴ������Ϣ */
//	bRet =    UploadPlayInfo()      ; 	

//	UploadFocus();

	return 0;
}

static  BOOL  s_bUpdate3D           = FALSE;
static  BOOL  s_bUpdateFlash        = FALSE;
static  BOOL  s_bUpdateVideo        = FALSE;
static  BOOL  s_bUpdateActiveFlash  = FALSE;    

//------------------------------------------------------------------------------
//��������XMLУ��ֵ�����仯���������µ�XML�ļ���ɺ�����
//------------------------------------------------------------------------------
INT  KAdCoreApp::OnUpdateByXML ()
{    
	BOOL          bRet                  = FALSE;
	AD_XML_Info::AD_RECORD_IT  it       = m_ServerxmlAD.m_ads.begin();

	//
	//���������ɣ����и�������
	//
	if ( it == m_ServerxmlAD.m_ads.end() )
	{
		ApplyNewXmlEffect();

		//�رո����¼������������ٱ�����
		UnDo();
		return 0;
	}

	//
	//��Ҫ����Ƿ�����
	//
    AD_XML_RECORD    xmlRS = *it;
    int ChangeStatus=0;
	if ( m_adManager.IsNeedUpdate(xmlRS,TRUE,&ChangeStatus) && DownUpdateAD( xmlRS ))
	{
		/*
		��Ҫ���أ������سɹ��ģ����������ӵģ��汾���µģ��޹ʱ�ɾ����
		*/
		if( !xmlRS.IsColsed() )
		{		
			switch( xmlRS.dwAdType )
			{
				//case AD_TEXTURE:   break;
				case AD_FLASH:          s_bUpdateFlash       = TRUE ; break;
				case AD_ACTIVEFLASH:    s_bUpdateActiveFlash = TRUE ; break;
				case AD_VIDEO:          s_bUpdateVideo       = TRUE ; break;
				case AD_3D:             s_bUpdate3D          = TRUE ; break;  
			}
		}
		else
		{
			//���ر�״̬������Ҫ����
		}
	}
	
	//ȷ�� �򿪡��رչ��λ��ʾ�ĸ���
	/*
	״̬�ı�ģ��Ǵ��ڵģ��汾û�б仯����Ŀ
	*/
	if( ChangeStatus ==1)
	{
		switch( xmlRS.dwAdType )
		{
 		case AD_FLASH:       
		   if ( !xmlRS.IsColsed() )
		   {
			   s_bUpdateFlash       = TRUE ;
		   }
		   break;
 		case AD_ACTIVEFLASH:
			if ( !xmlRS.IsColsed() )
			{	
				s_bUpdateActiveFlash = TRUE ; 
			}
			break;
 		case AD_VIDEO:
			if ( !xmlRS.IsColsed() )
			{
				s_bUpdateVideo       = TRUE ; 
			}
			break;
		case AD_3D:
			s_bUpdate3D          = TRUE ; 
			break;  
		default:
			break;
		}
	}

	//
	//ɾ�������
	//
	m_ServerxmlAD.m_ads.erase(it);

	::Sleep(2000);

    return 0;
}
BOOL   KAdCoreApp::ApplyNewXmlEffect()
{
	if(!LoadADXml(m_adManager.m_xml));		
		return FALSE;
		
	//�Ƿ���������3D,flash,video,activeflash
	if  ( s_bUpdate3D )
	{
		if  (g_pSetUpdate != NULL )				
			(*(FUN_SetUpdate)g_pSetUpdate)();
		s_bUpdate3D  = FALSE;
		
		START_LOG;
		LOG << "3D reset ";
		END_LOG;
		
	}
	
	if  (s_bUpdateFlash )
	{
		TCHAR     szCfgName[ _MAX_PATH+1 ];
		m_paths.GetFlashCfgName (   szCfgName ,     _MAX_PATH );
		IGAWIN::ShowFlash       (   szCfgName                 );
		s_bUpdateFlash = FALSE;
		
		START_LOG;
		LOG << "flash reset ";
		END_LOG;
	}
	
	if  (s_bUpdateVideo )
	{
		TCHAR     szCfgName[ _MAX_PATH+1 ];
		m_paths.GetVideoCfgName (  szCfgName ,     _MAX_PATH );
		IGAWIN::ShowVideo       (  szCfgName                 );			
		s_bUpdateVideo = FALSE;
		START_LOG;
		LOG << "video reset ";
		END_LOG;
		
	}
	
	if  (s_bUpdateActiveFlash )
	{
		TCHAR     szCfgName[ _MAX_PATH+1 ];
		m_paths.GetActiveFlashCfgName( szCfgName , _MAX_PATH);
		IGAWIN::ShowActiveFlash      ( szCfgName            );			
		s_bUpdateActiveFlash = FALSE;
		
		START_LOG;
		LOG << "active flash reset ";
		END_LOG;
		
	}
	return TRUE;
}

INT KAdCoreApp::OnTimeOut ()
{
	BOOL  bRet;
	

	/* �ϴ���ע��  */
	bRet = UploadFocus() ;
    bRet = QueryAdInfo() ;

	//
	//���ұ��ع���ļ��Ƿ���Ҫ����,
	//ֻ���Ѽ��صĽ��м��,�൱���Լ죬�͸��º��xml�ļ��е���Ϣ�޹�
	//
	if  ( bRet )
	{      
	  AD_XML_Info::AD_RECORD_IT    it ;
	  for (   it  = m_adManager.m_xml.m_ads.begin() ; it != m_adManager.m_xml.m_ads.end() ; it ++ )
	  {	  
	      AD_XML_RECORD   xmlRS  = *it ;

	      if ( !m_adManager.IsNeedUpdate(xmlRS , FALSE ) )  continue;

		  START_LOG ;
		  LOG  << "Update LocalAD  ID= "  << xmlRS.dwID ;
		  DownUpdateAD(xmlRS );
		  END_LOG   ;
	  }		
	}
    return 0l;
}

/* ���أ���ѹzip ָ���Ĺ��λ*/
BOOL  KAdCoreApp::DownUpdateAD(AD_XML_RECORD&   xmlRS   )
{
	BOOL  bRet = FALSE;

	/* �������� */
	TCHAR  szTmpPath     [ _MAX_PATH +1 ];
	TCHAR  szTmpFilePath [ _MAX_PATH +1 ];

	m_paths.GetDownloadPath(szTmpPath,_MAX_PATH);
    ::wsprintf(szTmpFilePath,_T("%s%s"),szTmpPath,_T("_tmpAdzipFile.tmp"));
	bRet =    DownloadOneAdZip    (xmlRS.szIP,xmlRS.szftpDir,szTmpFilePath)  ;

	if  ( !bRet)
	{
		//ɾ����ʱ�ļ�
		ZFile::DeleteFile(szTmpFilePath);
		return FALSE;
	}
	
	TCHAR  szTmpUnzipPath [ _MAX_PATH+1 ];		

	//���ܣ���ѹ
	if  ( xmlRS.dwAdType  == AD_TEXTURE)
	{			
		m_paths.GetADFilePath ( szTmpUnzipPath  , _MAX_PATH      );				
        
		m_unZip.Unzip         ( szTmpFilePath   , szTmpUnzipPath );

		/*
		//ʵʱ����dds��˼·�ԣ����ǻ�����쳣
		m_NeedUpdateTexture.Lock();
		m_NeedUpdateTexture.add(xmlRS);
		m_NeedUpdateTexture.Unlock();	
		*/

		bRet = TRUE;
	}
	else if   ( xmlRS.dwAdType  == AD_FLASH)
	{
		m_paths.GetFlashPath  ( szTmpUnzipPath  , _MAX_PATH  );
		ZFile::DeletePath     ( szTmpUnzipPath  , TRUE       );
		ZFile::CreatePath     ( szTmpUnzipPath               );

        if ( m_unZip.Unzip(szTmpFilePath,szTmpUnzipPath) )
		{				
			TCHAR     szCfgName[ _MAX_PATH+1 ];
			m_paths.GetFlashCfgName(szCfgName,_MAX_PATH);

			ZFileIni  cfg(szCfgName);
			cfg.WriteInt(_T("comm"),_T("id"),xmlRS.dwID);					
			m_adManager.UpdateFlash(xmlRS,NULL);
			bRet   = TRUE;

		}
		else
		{
			LOG  << ("flash unzip failed.");
		}
	}

	else if   ( xmlRS.dwAdType  == AD_SEARCH)
	{
		m_paths.GetFlashPath  ( szTmpUnzipPath  , _MAX_PATH  );
		ZFile::CreatePath     ( szTmpUnzipPath               );
		
        if ( m_unZip.Unzip(szTmpFilePath,szTmpUnzipPath) )
		{				
			//m_adManager.UpdateFlash(xmlRS,NULL);
			bRet   = TRUE;
			
		}
		else
		{
			LOG  << ("search unzip failed.");
		}
	}
	else if   ( xmlRS.dwAdType  == AD_ACTIVEFLASH )
	{
		m_paths.GetActiveFlashPath  (  szTmpUnzipPath ,    _MAX_PATH      );
		
        if  ( m_unZip.Unzip(  szTmpFilePath,szTmpUnzipPath ))
		{
			TCHAR     szCfgName[ _MAX_PATH+1 ];
			m_paths.GetActiveFlashCfgName(szCfgName,_MAX_PATH);
			ZFileIni  cfg(szCfgName);
			cfg.WriteInt(_T("comm"),_T("id"),xmlRS.dwID);
			m_adManager.UpdateActiveFlash(xmlRS,NULL);
			
			bRet = TRUE ;
		}
		else
		{
			LOG << "active unzip failed";
		}            
	}	
	else if   ( xmlRS.dwAdType  == AD_VIDEO)
	{
		m_paths.GetVideoPath  ( szTmpUnzipPath ,   _MAX_PATH );
		ZFile::DeletePath     ( szTmpUnzipPath ,   TRUE      );
		ZFile::CreatePath     ( szTmpUnzipPath               );

        if ( m_unZip.Unzip(szTmpFilePath,szTmpUnzipPath) )
		{			
			TCHAR     szCfgName[ _MAX_PATH+1 ];
			m_paths.GetVideoCfgName(szCfgName,_MAX_PATH);

			ZFileIni  cfg(szCfgName);
			cfg.WriteInt(_T("comm"),_T("id"),xmlRS.dwID);					
			m_adManager.UpdateVideo(xmlRS,NULL);
			bRet   = TRUE;
			
		}
		else
		{
			LOG  << ("video unzip failed.");
		}			
	}

	else if   ( xmlRS.dwAdType  == AD_3D)
	{
		m_paths.Get3DPath   (  szTmpUnzipPath ,   _MAX_PATH);
		
        if (  m_unZip.Unzip(szTmpFilePath,szTmpUnzipPath) )
		{
			//ģ����ʱ�����ļ�
            TCHAR   szTmpCfgName[ _MAX_PATH +1 ];
			wsprintf(szTmpCfgName,_T("%s%s"),szTmpUnzipPath,_T("mesh.ini"));

			//ģ�������ļ�
			TCHAR   szCfgName[ _MAX_PATH +1 ];
			wsprintf(szCfgName,_T("%s%s"),szTmpUnzipPath,_T("data.ini"));	

			ZFileIni    meshCfg( szCfgName    );				
			ZFileIni    TmpCfg ( szTmpCfgName );						

			int    nMeshID;						
            TCHAR  szBuf     [ 256 ] ;				
			TCHAR  szSection [ 32  ] ;

            TmpCfg.GetString(_T("mesh"),_T("mesh_id"),szSection,31,_T("0"));						
            nMeshID  = atoi(szSection);
			if  ( nMeshID != 0 )  
			{   
				meshCfg.WriteInt(_T("comm"),_T("gameid"),m_game.m_dwGameID);
				meshCfg.WriteInt(szSection,_T("mesh_id"),nMeshID);

				TmpCfg.GetString(_T("mesh"),_T("x"),szBuf,31,_T("0"));
				meshCfg.WriteString(szSection,_T("x"),szBuf);
				
				TmpCfg.GetString(_T("mesh"),_T("y"),szBuf,31,_T("0"));
				meshCfg.WriteString(szSection,_T("y"),szBuf);

				TmpCfg.GetString(_T("mesh"),_T("z"),szBuf,31,_T("0"));
				meshCfg.WriteString(szSection,_T("z"),szBuf);

				TmpCfg.GetString(_T("mesh"),_T("s_xyz"),szBuf,31,_T("0"));
				meshCfg.WriteString(szSection,_T("s_xyz"),szBuf);

				TmpCfg.GetString(_T("mesh"),_T("r_x"),szBuf,31,_T("0"));
				meshCfg.WriteString(szSection,_T("r_x"),szBuf);

				TmpCfg.GetString(_T("mesh"),_T("r_y"),szBuf,31,_T("0"));
				meshCfg.WriteString(szSection,_T("r_y"),szBuf);

				TmpCfg.GetString(_T("mesh"),_T("r_z"),szBuf,31,_T("0"));
				meshCfg.WriteString(szSection,_T("r_z"),szBuf);


				TmpCfg.GetString(_T("mesh"),_T("url"),szBuf,255,_T("0"));
				meshCfg.WriteString(szSection,_T("url"),szBuf);

				/*
				�Բ�����Ƶ�ļ���֧��
				*/
				int nDisplay=0;
				TmpCfg.GetInt(_T("mesh"),_T("IsPaly"),nDisplay,0);
				if(nDisplay==1)
				{
					meshCfg.WriteString(szSection,_T("IsPaly"),"1");
					
					TmpCfg.GetString(_T("mesh"),_T("VideoFileName"),szBuf,255,_T(""));
					meshCfg.WriteString(szSection,_T("VideoFileName"),szBuf);
				}
				

				meshCfg.GetString(_T("comm"),_T("list"),szBuf,240,_T(""));

				TCHAR     szCurID [ 64 ];
				wsprintf(szCurID,"%u,",nMeshID);
				CString   strList = szBuf;
				if  ( -1 == strList.Find(szCurID,0) )
				{
					TCHAR  szValue [ 256 ];
					wsprintf(szValue,_T("%s%s"),strList,szCurID);
					meshCfg.WriteString(_T("comm"),_T("list"),szValue);								
				}

				ZFile::DeleteFile( szTmpCfgName );
				bRet  =  TRUE ;                            
			}			
			m_adManager.Update3D_Mesh(xmlRS,NULL);
		}
		else
		{
	        LOG << "3D unzip failed";
		}
		
		ZFile::DeletePath(szTmpUnzipPath);
	}		

	//ɾ����ʱ�ļ�
	ZFile::DeleteFile(szTmpFilePath);

	return bRet;
}

INT KAdCoreApp::OnExit ()
{	
	
// 	START_LOG;
// 	LOG << "��Ϸ����ǰ�ϴ������Ϣ";
// 	END_LOG;
	
	return 0l;
}



VOID  KAdCoreApp::Init()
{	
#if defined(USING_D3D9)
	m_dwHookType   = 9;
	::_tcscpy(m_szHookType,_T("DirectX 9.0"));

#elif defined(USING_D3D8)
    m_dwHookType   = 8;
	::_tcscpy(m_szHookType,_T("DirectX 8.0"));
#else
	m_dwHookType   = 0;
	::_tcscpy(m_szHookType,_T("Unknow "));

#endif
	
	//time_t tt = (time_t)m_dwStartServerTime;
	::time((time_t*)&m_dwStartServerTime);
	
	m_dwStarLocalTime  = ::GetTickCount();
	m_dwRunTime    = 0;

	 
	m_3Dfilter.pFilterD3D          = NULL ;
	m_3Dfilter.pFilterDevice       = NULL ;
    m_3Dfilter.pFilterTexture      = NULL ; 
    m_3Dfilter.pFilterVertexBuffer = NULL ;
    m_3Dfilter.pFilterIndexBuffer  = NULL ; 
    m_3Dfilter.pFilterSurface      = NULL ; 
    m_3Dfilter.pFilterSwapChain    = NULL ; 	
	

	/*
	��һ�μ���XML�ļ�
	*/
    LoadADXml(m_adManager.m_xml);

	/*
	��һ�μ���emmoa.ini
	*/
	LoadConfig ();	

	GetGameVersion(m_game.m_dwGameID,
					m_game.m_GameVersion,sizeof(m_game.m_GameVersion));

	m_game.m_dwAreaID    = -1;
	m_game.m_dwChannelID = -1;

	/*
	�������
	*/
	ClearFilesByXML();

}

BOOL  KAdCoreApp::LoadConfig()
{
	//ȡϵͳ�����ļ�����
    TCHAR       TmpBuf[256];    
	m_paths.GetSysCfgName(TmpBuf,255);
    /*���ܽ�ѹ�ļ� */ 


	ZFileIni    ini(TmpBuf);    
	/*Ĭ��URL*/
	ini.GetString(IDS_SECTION_COMMON,IDS_KEY_DEFAULT_URL,TmpBuf,255);

	int nLen = strlen(TmpBuf);
	if(TmpBuf[nLen-1]!='/')
		strcat(TmpBuf,"/");
	m_net.SetTopURL(TmpBuf);

	//Ĭ��ip����ͨ��urlת��
    ini.GetString(IDS_SECTION_COMMON,_T("FtpIP"),m_szFTPIP,20,_T("127.0.0.1"));


	//����ͨѶЭ��
    m_net.SetProtocol(1);

	//ȡ��Ϸid
	int  nGameID;
	ini.GetInt(IDS_SECTION_COMMON,IDS_KEY_GAMEID,nGameID,0);
	m_game.m_dwGameID = nGameID;

	//ɨ��ʱ��    s
	int  nScanTime;
    ini.GetInt(IDS_SECTION_COMMON,_T("ScanTime"),nScanTime,5*60);
	nScanTime     = max(nScanTime,60);
	m_dwTimeOut   = nScanTime * 1000;

	/*
	���g_nDebug�Ѿ�������Ϊ1�����ٸ���
	*/
	if(g_nDebug<=0)
		ini.GetInt(IDS_SECTION_COMMON,IDS_KEY_DEBUG,g_nDebug,0);

	return  TRUE;
}

void KAdCoreApp::GetGameVersion(int nGameId,char *lpBuf, int nBufferLen)
{
	//��ʼ��
	lpBuf[0]=0;

	//���������Ŀ¼��version.dat
	if(nGameId == IGA_WUJIEQU_MLXY
		|| nGameId == IGA_WUJIEQU_XJP
		|| nGameId == IGA_WUJIEQU_FLB
		|| nGameId == IGA_WUJIEQU)
	{
		char szVersionRoot[MAX_PATH];
		::GetModuleFileName(NULL,szVersionRoot,MAX_PATH);
		
		char* p = strrchr(szVersionRoot,'\\');
		if(!p)
			return;
		*p =0;

		p = strrchr(szVersionRoot,'\\');
		if(!p)
			return;
		p++;
		*p=0;

		//��װ�ļ�ȫ·��
		lstrcat(szVersionRoot,"Version.dat");

		CFile file;
		if(!file.Open(szVersionRoot,CFile::modeRead|CFile::shareDenyNone))
			return;

		if(file.GetLength()<4)
		{
			file.Close();
			return;
		}
		DWORD dwFileVersion=0;
		file.Read(&dwFileVersion,4);
		file.Close();
		wsprintf(lpBuf,"0.%d",dwFileVersion);
		return;
	}

}


#include "tinyxml.h"
#pragma comment(lib,"..\\lib\\tinyxml.lib")
/*����������ļ�*/
BOOL  KAdCoreApp::LoadADXml(AD_XML_Info& adInfo)
{	
	//ȡϵͳ�����ļ�����
    TCHAR       TmpBuf[256];    
	m_paths.GetAdCfgName(TmpBuf,255);
	
    /*���ܽ�ѹ�ļ� */ 	 	
	char*   pTarget = NULL;
	ZFile   file;
    if ( file.Open(TmpBuf,OPEN_EXISTING) )
	{
        return FALSE;    
	}

	DWORD  dwFileLen = file.GetLength();
	if  ( dwFileLen ==0 ) 
	{
		return FALSE;
	}
	pTarget   = new char[dwFileLen];
    file.Read(pTarget,dwFileLen);
	file.Close();
	

	TCHAR       TmpPath[256];    
	
    /* ��xml�ļ�*/
 	TiXmlDocument	doc;
 	doc.Parse(pTarget); 
 	TiXmlHandle  hDoc(&doc); 		

	TiXmlElement* pElem = hDoc.FirstChildElement().Element();
	if (!pElem) 
	{
		delete []pTarget;
		return	FALSE;		
	}
	
	TiXmlHandle  hRoot(0);
	hRoot = TiXmlHandle(pElem);

	static	const CHAR	szBranch[]	= "IGA_2D";
	static	const CHAR	szKey[]		= "AD";

	//
	//�����������
	//
	adInfo.clear();

	AD_XML_RECORD ad;	
	const char*   pTmp;
	TiXmlElement* pAdsTextureNode = hRoot.FirstChild(szBranch).FirstChild().Element();
	for (pAdsTextureNode; pAdsTextureNode != NULL; pAdsTextureNode = pAdsTextureNode->NextSiblingElement(szKey))
	{	
		//���id
		pAdsTextureNode->Attribute("id", (INT*)&ad.dwID);
		
		//���汾
		pAdsTextureNode->Attribute("AdVersion", (INT*)&ad.dwAdVersion);

		//ƥ���ļ��汾
		pAdsTextureNode->Attribute("OrigFileVersion", (INT*)&ad.dwOrigFileVersion);

		//�Ƿ�ر�
		pAdsTextureNode->Attribute("Closed", (INT*)&ad.dwClose);
		
		//ftp  ip
		pTmp = pAdsTextureNode->Attribute("IP");
		if( NULL != pTmp )
			::lstrcpyA(ad.szIP, pTmp);

		//ftp  ip
		pTmp = pAdsTextureNode->Attribute("ftpDir");
		if( NULL != pTmp )
			::lstrcpyA(ad.szftpDir, pTmp);

		//�������
		pTmp = pAdsTextureNode->Attribute("AdType");
		if  ( strcmp(pTmp,"FLASH") == 0)
		{
			ad.dwAdType = AD_FLASH;

			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)  ::wsprintf( ad.szAdFile   ,  _T("%s") ,   pTmp );					
		}
		else if  ( strcmp(pTmp,"SEARCH") == 0)
		{
			ad.dwAdType = AD_SEARCH;
			
			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)  ::wsprintf( ad.szAdFile   ,  _T("%s") ,   pTmp );					
		}
		else if ( strcmp(pTmp,"ActiveFlash") == 0)
		{
            ad.dwAdType = AD_ACTIVEFLASH;
			
			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)  ::wsprintf( ad.szAdFile   ,  _T("%s") ,   pTmp );					
			
		}
		else if ( strcmp(pTmp,"VIDEO") == 0)
		{
            ad.dwAdType = AD_VIDEO;

			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)  ::wsprintf( ad.szAdFile   ,  _T("%s") ,   pTmp );
		}
		else if ( strcmp(pTmp,"3D") == 0)
		{
			ad.dwAdType = AD_3D;

			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)  ::wsprintf( ad.szAdFile   ,  _T("%s") ,   pTmp );					
		}
		else if ( strcmp(pTmp,"DDS") == 0)
		{
            ad.dwAdType = AD_TEXTURE;

			m_paths.GetMatchFilePath  (  TmpPath  ,   255  ) ;

			pTmp  = pAdsTextureNode->Attribute("OrigFile");
			if(pTmp != NULL)   ::wsprintf( ad.szOrigFile ,  _T("%s%s") ,  TmpPath ,  pTmp );				

			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)  ::wsprintf( ad.szAdFile   ,  _T("%s%s") ,  TmpPath ,  pTmp );					
		}		
		else
		{
			LOG  << "XML error line";
            continue;
		}		

		adInfo.add(ad);
	}
	
	adInfo.dwCRC = ZFile::GetFileCRC32(TmpBuf);

 	delete []pTarget;
 	pTarget = NULL;

	return	TRUE;
}

void KAdCoreApp::UpdateXmlCRC(AD_XML_Info& adInfo)
{
	//ȡϵͳ�����ļ�����
    TCHAR       FilePathName[256];    
	m_paths.GetAdCfgName(FilePathName,255);
	adInfo.dwCRC = ZFile::GetFileCRC32(FilePathName);
}


/*  ȡsdk�汾�� */
VOID   KAdCoreApp::LoadSDKVersion(TCHAR*  szVersion)       
{
	TCHAR   szCoreName [ _MAX_PATH +1 ];
	
	
	m_paths.GetWorkingPath(szCoreName,_MAX_PATH);
	::_tcscat(szCoreName,_T("emmoaz.iaf"));

	DWORD   dwSize = 0;
	ZFile::GetFileVersion(szCoreName,szVersion,dwSize);    
}

void KAdCoreApp::ClearFilesByXML()
{
	try
	{

	CStringArray fileD3DArray;
	CStringArray fileDDSArray;
	CStringArray fileFlashArray;
	CStringArray fileVedioArray;
	
	GetFilesFromXML(fileD3DArray,fileDDSArray,fileFlashArray,fileVedioArray);
	
	/*
	����ļ�
	*/

	CString strPath;
    TCHAR       TmpBuf[256];    
	m_paths.Get3DPath(TmpBuf,255);
	strPath = TmpBuf;
	ClearFilesByPath(strPath,fileD3DArray);

	m_paths.GetFlashPath(TmpBuf,255);
	strPath = TmpBuf;
	ClearFilesByPath(strPath,fileFlashArray);

	m_paths.GetADFilePath(TmpBuf,255);
	strPath = TmpBuf;
	ClearFilesByPath(strPath,fileDDSArray);

	m_paths.GetVideoPath(TmpBuf,255);
	strPath = TmpBuf;
	ClearFilesByPath(strPath,fileVedioArray);
	}
	catch(...)
	{
		START_LOG;
		LOG << "ClearFilesByXML Error";
		END_LOG;
	}

}
void KAdCoreApp::GetFilesFromXML( CStringArray &fileD3DArray,
								  CStringArray &fileDDSArray,
								  CStringArray &fileFlashArray,
								  CStringArray &fileVedioArray)
{
	//ȡϵͳ�����ļ�����
    TCHAR       TmpBuf[256];    
	m_paths.GetAdCfgName(TmpBuf,255);
	
    /*
	��ȡ�����ܽ�ѹ���ļ�
	*/ 	 	

	char*   pTarget = NULL;
	ZFile   file;
    if ( file.Open(TmpBuf,OPEN_EXISTING) )
	{
        return;    
	}

	DWORD  dwFileLen = file.GetLength();
	if  ( dwFileLen ==0 ) 
	{
		return ;
	}
	pTarget   = new char[dwFileLen];
    int nReaded = file.Read(pTarget,dwFileLen);
	if(nReaded!=dwFileLen)
	{
		delete []pTarget;
		pTarget= NULL;
		file.Close();
		return;
	}
	file.Close();

    /* 
	����xml�ļ�
	*/

 	TiXmlDocument	doc;
 	doc.Parse(pTarget); 
 	TiXmlHandle  hDoc(&doc); 		

	TiXmlElement* pElem = hDoc.FirstChildElement().Element();
	if (!pElem) 
	{
		delete []pTarget;
		return	;		
	}
	
	TiXmlHandle  hRoot(0);
	hRoot = TiXmlHandle(pElem);

	static	const CHAR	szBranch[]	= "IGA_2D";
	static	const CHAR	szKey[]		= "AD";

	/*
	��ȡ�����ļ�
	*/
	const char*   pTmp;
	TiXmlElement* pAdsTextureNode = hRoot.FirstChild(szBranch).FirstChild().Element();
	for (; pAdsTextureNode != NULL; pAdsTextureNode = pAdsTextureNode->NextSiblingElement(szKey))
	{	
		//�������
		pTmp = pAdsTextureNode->Attribute("AdType");
		if  ( strcmp(pTmp,"FLASH") == 0)
		{
			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)  
			{
				CString str = pTmp;
				fileFlashArray.Add(str);
			}
		}
		else if ( strcmp(pTmp,"ActiveFlash") == 0)//������Ͳ��Ǵ�д
		{
			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)
			{

			}
			
		}
		else if ( strcmp(pTmp,"SEARCH") == 0)
		{
			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)
			{
				TCHAR   szFileList [ _MAX_PATH ];
				lstrcpy(szFileList,pTmp);

				TCHAR*  pSeparator = szFileList ;
				TCHAR*  pNext      = 0          ;
				
				/*
				ѭ������
				*/
				while ( pSeparator )
				{
					pNext  = ::_tcschr( pSeparator , ';' )    ;
					if ( pNext )       
						*pNext = 0;
					
					CString str = pSeparator;
					fileFlashArray.Add(str);
					
					if  ( pNext == 0 )  
						break;
					
					pSeparator = pNext+1 ;
				}
			}
			
		}
		else if ( strcmp(pTmp,"VIDEO") == 0)
		{
			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL) 
			{
				CString str = pTmp;
				fileVedioArray.Add(str);
			}
		}
		else if ( strcmp(pTmp,"3D") == 0)
		{
			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)
			{
				TCHAR   szFileList [ _MAX_PATH ];
				lstrcpy(szFileList,pTmp);

				TCHAR*  pSeparator = szFileList ;
				TCHAR*  pNext      = 0          ;
				
				/*
				ѭ������
				*/
				while ( pSeparator )
				{
					pNext  = ::_tcschr( pSeparator , ';' )    ;
					if ( pNext )       
						*pNext = 0;
					
					CString str = pSeparator;
					fileD3DArray.Add(str);
					
					if  ( pNext == 0 )  
						break;
					
					pSeparator = pNext+1 ;
				}
			}
		}
		else if ( strcmp(pTmp,"DDS") == 0)
		{
			pTmp  = pAdsTextureNode->Attribute("OrigFile");
			if(pTmp != NULL)
			{
				CString str = pTmp;
				fileDDSArray.Add(str);
			}

			pTmp  = pAdsTextureNode->Attribute("AdFile");
			if (pTmp != NULL)  
			{
				CString str = pTmp;
				fileDDSArray.Add(str);
			}
				
		}		
		else
		{
			LOG  << "XML error line";
		}		
	}
	
 	delete []pTarget;
 	pTarget = NULL;

}

void KAdCoreApp::ClearFilesByPath(CString& strPath,CStringArray &fileArray)
{
	char szTmpPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szTmpPath);

	CFileFind finder;

	if(strPath.Right(1)!='\\')
	{
		strPath+="\\";
	}

	CString strFind =strPath;
	strFind+="*";

	BOOL bWorking = finder.FindFile(strFind);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		if(finder.IsDots()) continue;

		if(finder.IsDirectory()) continue;

		//����ini�ļ�
		if(finder.GetFileName().GetLength()>=4)
			if(finder.GetFileName().Right(4).CompareNoCase(".ini")==0)
				continue;

		BOOL bLeagle = FALSE;
		for(int i=0; i< fileArray.GetSize(); i++)
		{
			if(fileArray.GetAt(i).CollateNoCase(finder.GetFileName())==0)
			{
				bLeagle=TRUE;
				break;
			}
		}
		if(!bLeagle)
		{
			/*
			ɾ���ļ�
			*/
			CString strDesFile = strPath+finder.GetFileName();
			DWORD dwAttrs = GetFileAttributes((LPCSTR)strDesFile);
			dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes((LPCSTR)strDesFile,dwAttrs);
			
			DeleteFile((LPCSTR)strDesFile);
		}
	}
}

#pragma pack(push, 1)

struct IGA_SDK_REPONSION
{
	DWORD        m_dwValidate;    //�Ƿ���Ч��ftp
	DWORD        m_dwServerTime;

	TCHAR        m_szSDK_FTP_IP       [ 20        ];  //ftp��ַ
	TCHAR        m_szSDK_FTP_DIR      [ _MAX_PATH ];  //ftpĿ¼

	//3o3afe1  2008 - 09-18���Ϊ��ȡ����ip,��������ֵ
	//TCHAR        m_clientIP           [ 20        ];  //�ͻ�����ip

};

struct IGA_CONFIG_REPONSION
{
	DWORD        m_dwValidate;    //�Ƿ���Ч��ftp
	TCHAR        m_sz_FTP_IP       [ 20        ];  //ftp��ַ
	TCHAR        m_sz_FTP_DIR      [ _MAX_PATH ];  //ftpĿ¼
};

struct IGA_ADINFO_REPONSION
{
	DWORD        m_dwValidate;    //�Ƿ���Ч��ftp
	TCHAR        m_sz_FTP_IP       [ 20        ];  //ftp��ַ
	TCHAR        m_sz_FTP_DIR      [ _MAX_PATH ];  //ftpĿ¼
};

struct  IGA_GAME_AD_CRC
{
    DWORD        m_dwGameID;
	DWORD        m_dwADCRC;
};



#pragma pack(pop)


/* ��ѯsdk */
BOOL   KAdCoreApp::QuerySDK() 
{	
	TCHAR  szTmpFileDirectory  [ _MAX_PATH+1 ];
    TCHAR  szTmpFile           [ _MAX_PATH+1 ];
	
	m_paths.GetCachePath(szTmpFileDirectory,_MAX_PATH);
	wsprintf(szTmpFile,_T("%s%s"),szTmpFileDirectory,_T("_Emmoa_sdk.tmp"));
	if  ( ZFile::IsExisted(szTmpFile) )   ZFile::DeleteFile(szTmpFile);
	
	TCHAR    szSDKVersion   [ 256 ];
    LoadSDKVersion(szSDKVersion);

	BOOL bRet = m_net.QuerySDK(m_game.m_dwGameID,
		                       m_game.m_GameVersion,
		                       szSDKVersion,
							   m_dwHookType,
		                       m_hardware.GetOnlyID(),
		                       m_game.m_dwChannelID,
		                       szTmpFile);

	START_LOG ;
    LOG << "��ѯSDK="<< (bRet ? "TRUE" : "FALSE");
	
	if ( bRet )
	{
		//����	
		IGA_SDK_REPONSION     header;
		memset((BYTE*)&header,0,sizeof(IGA_SDK_REPONSION));

		ZFile   file ;
		file.Open(szTmpFile,OPEN_EXISTING);

		/*�ṹ����Ҫ�ͷ�����һ��*/
        if  ( file.GetLength() >= sizeof(IGA_SDK_REPONSION) )
		{
            file.Read(&header,sizeof(IGA_SDK_REPONSION));            
		}
		else
		{
			LOG << "        sdk�ļ����ݲ���ȷ";
			bRet = FALSE;
		}	
		file.Close();
		ZFile::DeleteFile(szTmpFile);
      
		//ftp����
		LOG << "        SDK�Ƿ���Ҫ���� = " << ( (header.m_dwValidate  != 0) ? "TRUE" : "FALSE");
		if  ( header.m_dwValidate  != 0 )
		{
			bRet= FALSE;
			wsprintf(szTmpFile,_T("%s%s"),szTmpFileDirectory,_T("_Emmoa_sdk_remote.tmp"));
			if  ( ZFile::IsExisted(szTmpFile) )   ZFile::DeleteFile(szTmpFile);

			if  ( m_net.QueryFileFromFtp(header.m_szSDK_FTP_IP,header.m_szSDK_FTP_DIR,szTmpFile,NULL,NULL))	
			{
				TCHAR  szTargetFile        [ _MAX_PATH+1 ];
				m_paths.GetSDKZIPFileName(szTargetFile,_MAX_PATH);
				if(ZFile::CopyFile(szTmpFile,szTargetFile,FALSE))
					bRet  = TRUE;
				ZFile::DeleteFile(szTmpFile);
				
			}

			if(bRet)
			{
				LOG << "        �����������ɹ�";
			}
			else
			{
                LOG << "        ����������ʧ��";
			}
		}
	
		//m_game.m_dwAreaID = m_game.m_dwChannelID     = m_IPAREA.GetAreaID(header.m_clientIP);
		        
		//LOG << " ��������"  << header.m_clientIP << " ����:"<< m_game.m_dwAreaID;

 		m_dwStartServerTime      = header.m_dwServerTime;
		m_dwStarLocalTime        = ::GetTickCount();
		m_dwRunTime              = 0;
	}

	END_LOG;

	return bRet;
}           

/* ��ѯ���� */
BOOL   KAdCoreApp::QueryDriver()         
{
	return TRUE;
}
/* ��ѯ��� */
BOOL   KAdCoreApp::QueryPlugin()  
{
	return TRUE;
}       

/* ��ѯ���� */
BOOL   KAdCoreApp::QueryConfig()     
{
	START_LOG;
	TCHAR  szTmpFileDirectory  [ _MAX_PATH+1 ];
    TCHAR  szTmpFile           [ _MAX_PATH+1 ];    

	m_paths.GetCachePath(szTmpFileDirectory,_MAX_PATH);
    wsprintf(szTmpFile,_T("%s%s"),szTmpFileDirectory,_T("_Emmoa_cfg.tmp"));
    if  ( ZFile::IsExisted(szTmpFile) )   ZFile::DeleteFile(szTmpFile);
	
	//HTTP����

	BOOL bRet = m_net.QueryConfig(m_game.m_dwGameID,
		                          m_game.m_GameVersion,
							      -1,
							      m_hardware.GetOnlyID(),
							      m_game.m_dwChannelID,	
								  szTmpFile);

	LOG << "��ѯϵͳ�����ļ� ��" << (bRet ? "TRUE" : "FALSE");

	//�鵽����ȡ��ʱ�ļ�����

	if ( bRet )
	{
		IGA_CONFIG_REPONSION     header;
		memset((BYTE*)&header,0,sizeof(IGA_CONFIG_REPONSION));

		ZFile   file ;
		file.Open(szTmpFile,OPEN_EXISTING);
        if  ( file.GetLength() >= sizeof(IGA_CONFIG_REPONSION) )
		{
            file.Read(&header,sizeof(IGA_CONFIG_REPONSION));            
		}
		else
		{
			bRet = FALSE;
		}	
		
		file.Close();
		ZFile::DeleteFile(szTmpFile);
		
		//ȡ�ýṹ�е����ݣ���ʼ����Ŀ���ļ�

		if(!bRet)
		{
			LOG << "ϵͳ�����ļ����ݴ���" ;
		}
		else
		{
			//ftp����

			if  ( header.m_dwValidate  != 0 )
			{
				bRet = FALSE;

				wsprintf(szTmpFile,_T("%s%s"),szTmpFileDirectory,_T("_Emmoa_cfg_remote.tmp"));
				if  ( ZFile::IsExisted(szTmpFile) )   
					ZFile::DeleteFile(szTmpFile);

				//���Դ�FTP����������chn.ini

				if  ( m_net.QueryFileFromFtp(header.m_sz_FTP_IP,header.m_sz_FTP_DIR,szTmpFile,NULL,NULL))
				{				
					TCHAR  szTargetFile        [ _MAX_PATH+1 ];
					m_paths.GetSysCfgName(szTargetFile,_MAX_PATH);
					ZFile::CopyFile(szTmpFile,szTargetFile,FALSE);
#ifdef NEED_OUTPUT_MSG
					OutputDebugString(szTmpFile);
					OutputDebugString(szTargetFile);
#endif
					ZFile::DeleteFile(szTmpFile);
					bRet  = TRUE;		
				}

			}
			else
			{
				LOG << "��Ϊ����Ҫ����" ;
			}
		}
		LOG << " FTP�������� ��" << (bRet ? "  TRUE" : "  FALSE");
	}
    END_LOG;
	return bRet;
}    

/* ��ѯ������� */
BOOL   KAdCoreApp::QueryAdInfo() 
{
	START_LOG;
	
	TCHAR  szTmpFileDirectory  [ _MAX_PATH+1 ];
    TCHAR  szTmpFile           [ _MAX_PATH+1 ];

	m_paths.GetCachePath(szTmpFileDirectory,_MAX_PATH);
	wsprintf(szTmpFile,_T("%s%s"),szTmpFileDirectory,_T("_Emmoa_adxml.tmp"));
	if  ( ZFile::IsExisted(szTmpFile) )  
		ZFile::DeleteFile(szTmpFile);
	
	/*����XML�ļ�У��ֵ����ɾ�������޸Ķ�����������*/
	UpdateXmlCRC(m_adManager.m_xml);

	BOOL bRet = m_net.QueryAdInfo(m_game.m_dwGameID,
		                          m_game.m_dwAreaID,
		                          0,
							      m_hardware.GetOnlyID(),
							      m_game.m_dwChannelID,
							      szTmpFile,
								  (LPVOID)m_adManager.GetXMLCRC());

	LOG << "��ѯ�������  "  << "bRet = " << (bRet ? "TRUE" : "FALSE");
	if ( bRet )
	{
		IGA_ADINFO_REPONSION     header;
		memset((BYTE*)&header,0,sizeof(IGA_ADINFO_REPONSION));

		ZFile   file ;
		file.Open(szTmpFile,OPEN_EXISTING);
        if  ( file.GetLength() >= sizeof(IGA_ADINFO_REPONSION) )
		{
            file.Read(&header,sizeof(IGA_ADINFO_REPONSION));            
		}
		else
		{
			bRet = FALSE;
		}	
		
		file.Close();
		ZFile::DeleteFile(szTmpFile);
		
		if(!bRet)
		{
			LOG << "ϵͳ�����ļ����ݴ���" ;
		}
		else
		{	//ftp����
			if  ( header.m_dwValidate  != 0 )
			{
				wsprintf(szTmpFile,_T("%s%s"),szTmpFileDirectory,_T("_Emmoa_adxml_remote.tmp"));
				if  ( ZFile::IsExisted(szTmpFile) )   
					ZFile::DeleteFile(szTmpFile);

				LOG << " FTP���� ADINFO��";
				if  ( m_net.QueryFileFromFtp(header.m_sz_FTP_IP,header.m_sz_FTP_DIR,szTmpFile,NULL,NULL))			
				{

					LOG << header.m_sz_FTP_IP  << "\\" << header.m_sz_FTP_DIR <<   "(�ɹ�.)";
					TCHAR  szTargetFile        [ _MAX_PATH+1 ];
					m_paths.GetAdCfgName(szTargetFile,_MAX_PATH);
					
					if(ZFile::CopyFile(szTmpFile,szTargetFile,FALSE))
						LOG << "OK" ;	

					if(!LoadADXml(m_ServerxmlAD))
						LOG << "��������ļ�����ʧ��";

					ZFile::DeleteFile(szTmpFile);
					
					//���ø����¼�,����OnUpdateByXML
					DoUpdate();
					bRet  = TRUE;
				}
				else
				{
					LOG << header.m_sz_FTP_IP  << "\\" << header.m_sz_FTP_DIR <<   "(ʧ��.)";   
					
				}
			}
			else
			{
				LOG << "��������ļ���������";
			}
		}
	}
	END_LOG;
	return bRet;
}        

/* ��ѯ��� */
BOOL   KAdCoreApp::DownloadOneAdZip    (LPCTSTR szIP,LPCTSTR szDir,LPCTSTR  szLocalName )         
{
    BOOL  bResult  = FALSE;
   if  ( m_net.QueryFileFromFtp(szIP,szDir,szLocalName,NULL,NULL))   
   {
       bResult  = TRUE;
   }
   START_LOG;
   LOG << "���ع��"   << szDir << (bResult ? "   TRUE" : "   FALSE");
   END_LOG;
   return bResult;
}      

/* �ϴ�Ӳ����Ϣ */
BOOL   KAdCoreApp::UploadHardwareInfo()  
{
	START_LOG;
	TCHAR  szTmpFileDirectory  [ _MAX_PATH+1 ];
    TCHAR  szTmpFile           [ _MAX_PATH+1 ];

	m_paths.GetDefaultTempPath( szTmpFileDirectory,_MAX_PATH);
	wsprintf(szTmpFile,_T("%s%s"),szTmpFileDirectory,_T("_Emmoa_hardware.tmp"));
	if  ( ZFile::IsExisted(szTmpFile) )
		ZFile::DeleteFile(szTmpFile);
	
	
	BOOL bRet = m_net.UploadHardwareInfo(m_game.m_dwGameID,
		                                 m_hardware.GetOnlyID(),
							             m_hardware.GetMemorySize(),
							             m_hardware.GetCPU(),
							             m_hardware.GetDisplay(),
										 m_hardware.GetOperatorSystem(),
										 m_szHookType,
										 m_game.m_dwChannelID,
										 m_hardware.GetIP(0),
										 szTmpFile);
	LOG << "�ϴ�Ӳ����Ϣ ��" << (bRet ? "TRUE" : "FALSE");
	if  ( bRet )
	{
        ZFile::DeleteFile(szTmpFile);
	}

	END_LOG;
	return  bRet;
}

/* �ϴ������Ϣ */
BOOL   KAdCoreApp::UploadPlayInfo()     
{
	START_LOG;
	TCHAR  szTmpFileDirectory  [ _MAX_PATH+1 ];
    TCHAR  szTmpFile           [ _MAX_PATH+1 ];

	m_paths.GetDefaultTempPath( szTmpFileDirectory,_MAX_PATH);
	wsprintf(szTmpFile,_T("%s%s"),szTmpFileDirectory,_T("_Emmoa_playinfo.tmp"));
	if  ( ZFile::IsExisted(szTmpFile) )   
		ZFile::DeleteFile(szTmpFile);
	
	DWORD   dwPlayTime   =  (::GetTickCount() - m_dwStarLocalTime);
	m_dwStarLocalTime    = ::GetTickCount();
    BOOL bRet = m_net.UploadPlayInfo(m_game.m_dwGameID,
									dwPlayTime,
									m_dwStartServerTime,
									m_hardware.GetOnlyID(),
									m_game.m_dwAreaID,
									m_game.m_dwChannelID,
									szTmpFile);
	LOG << "UploadPlayInfo : " << m_game.m_dwGameID << dwPlayTime  <<" || ret = "<< (bRet? "TRUE" : "FALSE");
   if  ( bRet )
   {
       ZFile::DeleteFile(szTmpFile);
   }

   END_LOG;
   return bRet;
} 

/* �ϴ���ע��  */
BOOL   KAdCoreApp::UploadFocus()         
{
	static   DWORD   dwFlag = 0;

	if ( dwFlag != 0)  return FALSE;
	dwFlag =1 ;

	DWORD  dwLen  = 1024*4  ;
	char   pData [  1024*4 ];   
	
	
	if  ( m_adManager.GetFocusDataToBuf(pData,dwLen)>0)
	{

		START_LOG;
		LOG  << "��ʼ�ϴ���ע�� ��ԭʼ����= " << dwLen;

		IGA_FOCUS_RESPONSE*   pRequest =(IGA_FOCUS_RESPONSE*) pData;
		::_tcsncpy(pRequest->szMacID,m_hardware.GetOnlyID(),17);//!!!!!!!!!!!Ӧ���ú궨��
		pRequest->szMacID[17]=0;

		pRequest->dwAddress  = m_hardware.GetDWORD_IP(0);

		pRequest->dwGameID  = m_game.m_dwGameID;
		pRequest->dwAreaID  = m_game.m_dwAreaID;
	

		//�����ϴ�����
		DWORD dwEncryptLen = m_encrypt.CalculateRecquiredEncodeOutputBufferSize(dwLen);
		char*  pEncryptData = new char[dwEncryptLen+1];
		
		
		if  ( NULL != pEncryptData )
		{		
			m_encrypt.EncodeBuffer(pData,dwLen,pEncryptData);

			LOG  << "���ܳ���= " << dwEncryptLen;
			 m_net.UploadData(pEncryptData,dwEncryptLen,"c:\\123.html");
			 delete []pEncryptData;
		}
		else
		{
			LOG << "�ڴ��������";
		}
		END_LOG;		
	}
    
	dwFlag = 0;

   return TRUE;
}

/* ���ع�浽�豸�� */
DWORD KAdCoreApp::LoadAD2Device( VOID* pDevice)
{   
	return m_adManager.Load(pDevice);
}
VOID KAdCoreApp::DeleteAdFromDevice( VOID* pDevice)
{	
	m_adManager.Clear(pDevice);
}
