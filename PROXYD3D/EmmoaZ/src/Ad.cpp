#include "stdafx.h"
#include "z_File.h"
#include "z_FileIni.h"
#include "ad.h"
#include "D3DHelpFun.h"
#include "AdCoreApp.h"
#include "IGAWin.h"
#include "GlobalAPI_Val.h"



KADManger::KADManger()
{
	m_dwReplaceADTextureCount = 0;
	m_bLoadTexture = FALSE;
    ::InitializeCriticalSection(&m_cr);
}
KADManger::~KADManger()
{
    //
	//�ͷ����й��λ��Դ
	//
	ClearAll();	

	::DeleteCriticalSection(&m_cr)    ;
}

//
//���ر����浽�б���,ֻ�����ڴ���D3D�豸ʱ
//
DWORD  KADManger::Load(VOID*  pDevice)
{
	if(pDevice == NULL )
		return 0;
	int nAdCountLoaded =0;
	try
	{
		//
		//���豸�Ѿ����أ���ֹ�ظ�����
		//
		DEV_COUNT_MAP_IT  p =  m_dev_count.find(pDevice);
		if ( p != m_dev_count.end())   
			return 0;

		//
		//�����豸ָ��
		//
		m_dev_count.insert(DEV_COUNT_MAP::value_type(pDevice));

		m_bLoadTexture    = TRUE;
		this->Lock();

		//��XML�����м��ع��
		AD_WAIT_MATCH_ID   waitMatch;
		
		for (AD_XML_Info::AD_RECORD_IT it  = m_xml.m_ads.begin(); it != m_xml.m_ads.end() ; it++ )
		{		
			 AD_XML_RECORD&   xmlRs = *it;	
			 
			 /*
			 �رպ󲻼���
			 */
			 if  ( xmlRs.dwClose )
				 continue;   

			 AD_MAP_IT  adIT = m_ads.find(xmlRs.dwID);

			 KAd*  pAD = NULL;
			 //
			 //�ù��ID��һ�α�����
			 //
			 if  ( adIT == m_ads.end() )
			 {
				 if ( xmlRs.dwAdType == AD_FLASH)
				 {	
					 KFlashAD*  pFlashAd   = new KFlashAD;					
					 pAD  = pFlashAd;
					 IGAPath   path;
					 TCHAR     szCfgName[ _MAX_PATH+1 ];
					 path.GetFlashCfgName(szCfgName,_MAX_PATH);
					 IGAWIN::ShowFlash(szCfgName);
	
					 pAD->m_bLoad =1;
					 nAdCountLoaded++;
				 }
				 else if ( xmlRs.dwAdType == AD_VIDEO)
				 {		
					 KVideoAD* pVideoAD   = new KVideoAD;
					 pAD  = pVideoAD;				
					 IGAPath   path;
					 TCHAR     szCfgName[ _MAX_PATH+1 ];
					 path.GetVideoCfgName(szCfgName,_MAX_PATH);
					 IGAWIN::ShowVideo(szCfgName);
					 
					 pAD->m_bLoad =1;
					 nAdCountLoaded++;
				 }
				 else if ( xmlRs.dwAdType == AD_ACTIVEFLASH)
				 {
					 KActiveFlashAD* pActiveFlash   = new KActiveFlashAD;
					 pAD  = pActiveFlash;
					 IGAPath   path;
					 TCHAR     szCfgName[ _MAX_PATH+1 ];
					 path.GetActiveFlashCfgName(szCfgName,_MAX_PATH);
					 IGAWIN::ShowActiveFlash(szCfgName);
					 pAD->m_bLoad =1;
					 nAdCountLoaded++;
				 }			
				 else if ( xmlRs.dwAdType == AD_3D)
				 {
					 K3DAD*  p3DAd= new K3DAD;
					 pAD  = p3DAd;
					 pAD->m_bLoad =1;
					 nAdCountLoaded++;
				 }			
				 else if ( xmlRs.dwAdType == AD_SEARCH)
				 {
					 KSearchAD*  pSearchAd= new KSearchAD;
					 pAD  = pSearchAd;
					 pAD->m_bLoad =1;
					 nAdCountLoaded++;
				 }			
				 else if ( xmlRs.dwAdType == AD_TEXTURE)
				 {
					 KTextureAD* pTextureAD   = new KTextureAD;	

					 ::_tcscpy(pTextureAD->m_adFileName,xmlRs.szAdFile);
					 ::_tcscpy(pTextureAD->m_MatchFileName ,xmlRs.szOrigFile);	 
					 //��������				 				 
					 LPDIRECT3DDEVICE    pTmpDevice   = (LPDIRECT3DDEVICE) pDevice;
					 IDirect3DTexture*   pTmpTexture  = NULL;				
					 IDirect3DTexture*   pTmpTexture1 = NULL;
					 
					 pTextureAD->m_bLoad     = 0;
					 if ( IGA_D3D_HELP::CreateTexture(pTmpDevice,pTextureAD->m_MatchFileName,&pTmpTexture)	)				
					 {		
						 IGA_D3D_HELP::AddDeviceRefCount(pTmpDevice,1);
						 if ( IGA_D3D_HELP::CreateTexture(pTmpDevice, pTextureAD->m_adFileName,&pTmpTexture1,pTmpTexture))
						 {
							 IGA_D3D_HELP::AddDeviceRefCount(pTmpDevice,1);
							 TextureADContent   content;
							 content.m_pMexture      = NULL;
							 content.m_pSurface      = NULL;
							 content.m_pADTexture    = pTmpTexture1;
							 content.m_pMatchTexture = pTmpTexture;
							 
							 pTextureAD->devs[ pTmpDevice ]  = content;				 
							 
							 pTextureAD->m_bLoad     = 1;
							 
							 //
							 //׼��ȥ�����滻
							 //
							 waitMatch.insert(xmlRs.dwID);	
							 nAdCountLoaded++;
							 pAD  = pTextureAD;			 

						 }
						 else
						 {
							 pTmpTexture->Release();
							 IGA_D3D_HELP::AddDeviceRefCount(pTmpDevice,-1);
							 delete pTextureAD;
						 }					 
					 }
					 else
					 {
						 delete pTextureAD;
					 }

				 }
				 else
				 {
					START_LOG;
					LOG << "KADManger::Load Error Type"  ;
					END_LOG;
				 }

				 if(pAD)
				 {
					 pAD->m_dwAdID         = xmlRs.dwID;
					 pAD->type             = (_tagADType)xmlRs.dwAdType;
					 pAD->m_dwAdVersion    = xmlRs.dwAdVersion;
					 pAD->m_dwMatchVersion = xmlRs.dwOrigFileVersion;


					 pAD->m_pFocus              = new tagFocus;
					 memset ( pAD->m_pFocus,0,sizeof(tagFocus));
					 pAD->m_pFocus->dwAdID            = xmlRs.dwID;   //���id
					 pAD->m_pFocus->dwDownloadCount   = 0 ;           //������ش��� 
					 pAD->m_pFocus->dwLoadCount       = 1 ;           //�����ش���			 
					 
					 //����һ�����
					 m_ads [ xmlRs.dwID ] = pAD;
				 }
			 }
			 else
			 {
				 START_LOG;
				 LOG << "KADManger::Load �ù��ID�ڶ��α�����"  ;
				 END_LOG;

				 if ( xmlRs.dwAdType == AD_TEXTURE)
				 {
					 KTextureAD* pTextureAD =(KTextureAD*)(adIT->second);			
					 //��������				 				 
					 LPDIRECT3DDEVICE    pTmpDevice   = (LPDIRECT3DDEVICE) pDevice;
					 IDirect3DTexture*   pTmpTexture  = NULL;				
					 IDirect3DTexture*   pTmpTexture1 = NULL;
					 
					 if ( IGA_D3D_HELP::CreateTexture(pTmpDevice,pTextureAD->m_MatchFileName,&pTmpTexture)	)				
					 {					 
						 IGA_D3D_HELP::AddDeviceRefCount(pTmpDevice,1);
						 if ( IGA_D3D_HELP::CreateTexture(pTmpDevice, pTextureAD->m_adFileName,&pTmpTexture1,pTmpTexture))
						 {
							 IGA_D3D_HELP::AddDeviceRefCount(pTmpDevice,1);
							 TextureADContent   content;
							 content.m_pMexture      = NULL;
							 content.m_pSurface      = NULL;
							 content.m_pADTexture    = pTmpTexture1;
							 content.m_pMatchTexture = pTmpTexture;

							 pTextureAD->m_pFocus->dwLoadCount       += 1 ;           //�����ش���						 

							 pTextureAD->devs [ pTmpDevice ]  = content;
						 }
						 else
						 {
							 pTmpTexture->Release();
							 IGA_D3D_HELP::AddDeviceRefCount(pTmpDevice,-1);
						 }					 
					 }				
				 }
			 }
		}

		//asdasd	
		m_dwNeedReplaceTexture.d_array [ pDevice ] = waitMatch;

		this->Unlock();	
		m_bLoadTexture = FALSE;
		
	}catch(...)
	{
		START_LOG;
		LOG << "KADManger::Load Exception"  ;
		END_LOG;
	}

	if(g_nDebug)
	{
		START_LOG;
		char szTmp[200];
		wsprintf(szTmp,"KADManger::Load %d",nAdCountLoaded);
		LOG << szTmp  ;
		END_LOG;
	}
    return  0;
}

VOID  KADManger::Clear(VOID*  pDevice)
{
	ASSERT(pDevice != NULL );
    DEV_COUNT_MAP_IT  p =  m_dev_count.find(pDevice);	 
	if ( p == m_dev_count.end())  return ;
	
	m_bLoadTexture = TRUE;
	BOOL   bDelete = FALSE;
	if  ( m_dev_count.size() == 1 )
	{
		//�ϴ�һ�¹�ע��
		bDelete = TRUE;
	}	
	
	for(AD_MAP_IT  it = m_ads.begin(); it != m_ads.end() ; it++ )
	{	
		KAd*  pAd =  it->second;	
		if ( pAd->type == AD_FLASH)
		{	
			KFlashAD*  pFlashAd   = (KFlashAD*)pAd;	
			if  ( bDelete )  
			{
				delete pFlashAd->m_pFocus;
				delete pFlashAd;
			}

		}
		else if ( pAd->type == AD_VIDEO)
		{		
			KVideoAD* pVideoAD   = (KVideoAD*) pAd;					
			if  ( bDelete )  
			{
				delete pVideoAD->m_pFocus;
				delete pVideoAD;
			}

		}
		else if ( pAd->type == AD_ACTIVEFLASH)
		{
			KActiveFlashAD* pActiveFlash   = (KActiveFlashAD*)pAd;			
			if  ( bDelete )  
			{
				delete pActiveFlash->m_pFocus;
				delete pActiveFlash;
			}

		}			
		else if ( pAd->type == AD_3D)
		{
			K3DAD*  p3DAd= (K3DAD*)pAd;		
			if  ( bDelete )  
			{
				delete p3DAd->m_pFocus;
				delete p3DAd;
			}

		}			
		else if ( pAd->type == AD_SEARCH)
		{
			KSearchAD*  pSearchAd= (KSearchAD*)pAd;		
			if  ( bDelete )  
			{
				delete pSearchAd->m_pFocus;
				delete pSearchAd;
			}
			
		}			
		else if ( pAd->type == AD_TEXTURE)
		{

			KTextureAD* pTextureAD = (KTextureAD*)pAd;						
			KTextureAD::DEV_MAP_IT  devIT = pTextureAD->devs.find(pDevice);
			if  ( devIT !=  pTextureAD->devs.end())
			{
				if(g_nDebug>1)
				{
					START_LOG;
					char szTmp[200];
					wsprintf(szTmp,"KADManger::Clear %d",pAd->m_dwAdID);
					LOG << szTmp  ;
					END_LOG;
				}
                TextureADContent   content = devIT->second;

				if ( content.m_pADTexture    != NULL )
				{
                   ((IDirect3DTexture*)(content.m_pADTexture ))->Release();
				}
				if ( content.m_pMatchTexture != NULL  )
				{							
				   ((IDirect3DTexture*)(content.m_pMatchTexture))->Release();
				}
				pTextureAD->devs.erase(devIT);
			}

			if  ( bDelete )  
			{
				delete pTextureAD->m_pFocus;
				delete pTextureAD;
				
			}

		}	
	}
	
	m_dev_count.erase(p);
	if  ( bDelete )  
	{			
		m_dev_count.clear();
		m_ads.clear();
	}
	m_bLoadTexture = FALSE;
}

VOID   KADManger::ClearAll()
{	
	int nSize = m_dev_count.size();
	if ( 0 ==  nSize ) return;

	DEV_COUNT_MAP_IT  p =  m_dev_count.end();
	p--;
	
	for( ; p != m_dev_count.begin(); )
	{
		DEV_COUNT_MAP_IT tmp = p;
		p--;
        Clear(*tmp);
	}

	if(p == m_dev_count.begin())
	{
		Clear(*p);
	}
	
	/*���Ի�������
	while( 1 )
	{
		p = m_dev_count.begin();
		if(p == m_dev_count.end())
			break;

		Clear(*p);
	}
	*/
	if(g_nDebug)
	{
		START_LOG;
		char szTmp[200];
		wsprintf(szTmp,"KADManger::ClearAll");
		LOG << szTmp  ;
		END_LOG;
	}

}

//---------------------------------------------------------------
//����3dģ��
//---------------------------------------------------------------

BOOL   KADManger::Update3D_Mesh(AD_XML_RECORD& ad,VOID*  pDevice )
{
   if ( ad.dwAdType  != AD_3D )  return FALSE;	   

   AD_MAP_IT p  = m_ads.find( ad.dwID );
   if ( m_ads.end() == p )
   {
	   K3DAD*   pMesh = new K3DAD;
	   ASSERT( pMesh!= NULL );

	   pMesh->m_dwAdID         = ad.dwID;
	   pMesh->type             = (_tagADType)ad.dwAdType;
	   pMesh->m_dwAdVersion    = ad.dwAdVersion;
	   pMesh->m_dwMatchVersion = ad.dwOrigFileVersion;
		
		
	   pMesh->m_pFocus     = new tagFocus;
	   memset ( pMesh->m_pFocus,0,sizeof(tagFocus));
	   pMesh->m_pFocus->dwAdID            = ad.dwID;   //���id
	   pMesh->m_pFocus->dwDownloadCount   = 1 ;           //������ش��� 
	   pMesh->m_pFocus->dwLoadCount       = 1 ;           //�����ش���
		
	   m_ads [ad.dwID] = pMesh;		
   }
   else
   {
       K3DAD*   pMesh  = (K3DAD*)(p->second);
	   ASSERT(pMesh != NULL );
	   pMesh->m_pFocus->dwDownloadCount ++;
   }

   return FALSE;
}

//---------------------------------------------------------------
//����flash
//---------------------------------------------------------------
BOOL   KADManger::UpdateFlash(AD_XML_RECORD& ad,VOID*  pDevice )
{
	if ( ad.dwAdType  != AD_FLASH )  return FALSE;	   
	
	AD_MAP_IT p  = m_ads.find( ad.dwID );
	if ( m_ads.end() == p )
	{
		KFlashAD*   pFlashAD = new KFlashAD;
		ASSERT( pFlashAD!= NULL );
		
		pFlashAD->m_dwAdID         = ad.dwID;
		pFlashAD->type             = (_tagADType)ad.dwAdType;
		pFlashAD->m_dwAdVersion    = ad.dwAdVersion;
		pFlashAD->m_dwMatchVersion = ad.dwOrigFileVersion;
		
		
		pFlashAD->m_pFocus     = new tagFocus;
		memset ( pFlashAD->m_pFocus,0,sizeof(tagFocus));
		pFlashAD->m_pFocus->dwAdID            = ad.dwID;   //���id
		pFlashAD->m_pFocus->dwDownloadCount   = 1 ;           //������ش��� 
		pFlashAD->m_pFocus->dwLoadCount       = 1 ;           //�����ش���
		
		m_ads [ad.dwID] = pFlashAD;		
	}
	else
	{
		KFlashAD*   pFlashAD  = (KFlashAD*)(p->second);
		ASSERT(pFlashAD != NULL );
		pFlashAD->m_pFocus->dwDownloadCount ++;
	}
	
   return FALSE;
}

//---------------------------------------------------------------
//����video
//---------------------------------------------------------------

BOOL   KADManger::UpdateVideo(AD_XML_RECORD& ad,VOID*  pDevice )
{
	if ( ad.dwAdType  != AD_VIDEO )  return FALSE;	   
	
	AD_MAP_IT p  = m_ads.find( ad.dwID );
	if ( m_ads.end() == p )
	{
		KVideoAD*   pVideoAD= new KVideoAD;
		ASSERT( pVideoAD!= NULL );
		
		pVideoAD->m_dwAdID         = ad.dwID;
		pVideoAD->type             = (_tagADType)ad.dwAdType;
		pVideoAD->m_dwAdVersion    = ad.dwAdVersion;
		pVideoAD->m_dwMatchVersion = ad.dwOrigFileVersion;
		
		
		pVideoAD->m_pFocus     = new tagFocus;
		memset ( pVideoAD->m_pFocus,0,sizeof(tagFocus));
		pVideoAD->m_pFocus->dwAdID            = ad.dwID;   //���id
		pVideoAD->m_pFocus->dwDownloadCount   = 1 ;           //������ش��� 
		pVideoAD->m_pFocus->dwLoadCount       = 1 ;           //�����ش���
		
		m_ads [ad.dwID] = pVideoAD;		
	}
	else
	{
		KVideoAD*   pVideoAD  = (KVideoAD*)(p->second);
		ASSERT(pVideoAD != NULL );
		pVideoAD->m_pFocus->dwDownloadCount ++;
	}
	
   return FALSE;
}

//---------------------------------------------------------------
//����acitve flash
//---------------------------------------------------------------
BOOL   KADManger::UpdateActiveFlash(AD_XML_RECORD& ad,VOID*  pDevice )
{
	if ( ad.dwAdType  != AD_ACTIVEFLASH )  return FALSE;	   
	
	AD_MAP_IT p  = m_ads.find( ad.dwID );
	if ( m_ads.end() == p )
	{
		KActiveFlashAD*   pActiveFlashAD = new KActiveFlashAD;
		ASSERT( pActiveFlashAD!= NULL );
		
		pActiveFlashAD->m_dwAdID         = ad.dwID;
		pActiveFlashAD->type             = (_tagADType)ad.dwAdType;
		pActiveFlashAD->m_dwAdVersion    = ad.dwAdVersion;
		pActiveFlashAD->m_dwMatchVersion = ad.dwOrigFileVersion;
		
		
		pActiveFlashAD->m_pFocus     = new tagFocus;
		memset ( pActiveFlashAD->m_pFocus,0,sizeof(tagFocus));
		pActiveFlashAD->m_pFocus->dwAdID            = ad.dwID;		//���id
		pActiveFlashAD->m_pFocus->dwDownloadCount   = 1 ;           //������ش��� 
		pActiveFlashAD->m_pFocus->dwLoadCount       = 1 ;           //�����ش���
		
		m_ads [ad.dwID] = pActiveFlashAD;		
	}
	else
	{
		KActiveFlashAD*   pActiveFlashAD  = (KActiveFlashAD*)(p->second);
		ASSERT(pActiveFlashAD != NULL );
		pActiveFlashAD->m_pFocus->dwDownloadCount ++;
	}
	
   return FALSE;
}

//---------------------------------------------------------------
//�Ƿ���Ҫ����
//���bForceCmpΪFALSE,��Ա���XML��ֻ�Ǽ���ļ��Ƿ���ڣ�
//���bForceCmpΪTRUE,���XML�����غ��ܷ��������ӹ��λ���ܲ����汾�Ƚ�
//---------------------------------------------------------------
BOOL   KADManger::IsNeedUpdate(AD_XML_RECORD& rs ,BOOL bForceCmp,int* lpChange )
{
   AD_XML_RECORD   localRS  = rs   ;

   //
   //�����Ҫ���������Ѽ��صĹ��Ƚ�
   //
   if   ( bForceCmp )
   {
	   BOOL   bFind   = FALSE ;
       for (AD_XML_Info::AD_RECORD_IT it  = m_xml.m_ads.begin(); it != m_xml.m_ads.end() ; it++ ) 
	   {
		   localRS  = *it;
	       if ( rs.dwID == localRS.dwID) 
		   {
			   bFind  = TRUE ;
			   break         ;
		   }
		   bFind      = FALSE;
	   }
	   if  ( !bFind )  return TRUE ;
   }        
   
   //�汾�Ƚ�
   if  ( rs.dwAdVersion != localRS.dwAdVersion || rs.dwOrigFileVersion != localRS.dwOrigFileVersion)   return TRUE;

   //�����ļ�����
   if  ( localRS.dwAdType == AD_TEXTURE )
   {
	   return !( ZFile::IsExisted ( localRS.szAdFile ) &&  ZFile::IsExisted ( localRS.szOrigFile ) );
   }   
 
   //�������е�xml�ļ��е��� �Ƿ���Ҫ֪ͨ���� �رջ�����ʾ��ת��
   if(lpChange)
   {
	   if(localRS.dwClose != rs.dwClose )
		   *lpChange =1;
	   else
		   *lpChange =0;
   }
  

  //���������ļ�����
   static  IGAPath     paths;
   TCHAR   szFileDir   [ _MAX_PATH   ];
	 
   if      ( localRS.dwAdType == AD_FLASH       )       paths.GetFlashPath      ( szFileDir,_MAX_PATH );
   else if ( localRS.dwAdType == AD_ACTIVEFLASH )       paths.GetActiveFlashPath( szFileDir,_MAX_PATH );
   else if ( localRS.dwAdType == AD_VIDEO       )       paths.GetVideoPath      ( szFileDir,_MAX_PATH );
   else if ( localRS.dwAdType == AD_3D          )       paths.Get3DPath         ( szFileDir,_MAX_PATH );
   else if ( localRS.dwAdType == AD_SEARCH      )       paths.GetFlashPath      ( szFileDir,_MAX_PATH );
   else return  FALSE;

   const  int MAX_LIST_LEN   = 1024;
   TCHAR   szFileList [ MAX_LIST_LEN ]; 
   ::_tcscpy ( szFileList ,  localRS.szAdFile   );
   
   if  ( szFileList [ 0] == 0               )  return TRUE;
   if  ( ::_tcsstr(szFileList , _T(".zip")) )  return FALSE;   //������ǰ�汾�ļ���


   TCHAR   szFileName [ _MAX_PATH ];
   TCHAR*  pSeparator = szFileList ;
   TCHAR*  pNext      = 0          ;

   while ( pSeparator )
   {
	   pNext  = ::_tcschr( pSeparator , ';' )    ;
	   if ( pNext )       *pNext = 0             ;
	   
	   ::wsprintf(szFileName,_T("%s%s"),szFileDir,pSeparator);
	   if  ( !ZFile::IsExisted( szFileName ))  return TRUE;

	   if  ( pNext == 0 )  break;
	   
	   pSeparator = pNext+1 ;
   }

   return FALSE ;
}


//---------------------------------------------------------------
//��ע�ȴ���
//---------------------------------------------------------------
BOOL  KADManger::GetInforFromAD(RESPONSE_FOCUS2* pOutFocus,tagFocus*  pFocus,_tagADType Type)
{
	pOutFocus->dwAdID            = pFocus->dwAdID          ; //	���ID��
	pOutFocus->dwDownloadCount   = pFocus->dwDownloadCount ; //	���ش���
	pOutFocus->dwLoadCount       = pFocus->dwLoadCount     ; //	�����Դ�����صĴ���				
	pOutFocus->dwClickCount      = pFocus->dwClickCount    ; //	��汻����Ĵ���
	pOutFocus->dwPlayCount       = pFocus->dwShowCount     ; //	�������ʾ����
	pOutFocus->dwFullPlayCount   = 0                       ; //	��������������
	
	//
	//������ʾʱ��,��������� �����رպ�δ���ر�
	//

	BOOL bClose = FALSE;
	DWORD   dwCurrentTime     = ::GetTickCount();

	//�в���ʱ�����ݣ���ΪFlash DDS���û�������ر�
	if( pFocus->dwPlayTime )
	{
		pOutFocus->dwPlayTime        = pFocus->dwPlayTime ;	 
		bClose = TRUE;
	}
	else
	{
		//Ŀ��û�б���ʼ��,��������һ��ֵ����������Ĳ���ʱ��Ϊ��
		if(pFocus->dwStartPalyTime==0)
			pFocus->dwStartPalyTime=dwCurrentTime;

		if(dwCurrentTime >= pFocus->dwStartPalyTime)
		{
			pOutFocus->dwPlayTime   = dwCurrentTime - pFocus->dwStartPalyTime ;	
		}
		else
		{
			//���ǵ�GetTickCount����㣬Ȼ����¿�ʼ
			pOutFocus->dwPlayTime   = (0xFFFFFFFF- pFocus->dwStartPalyTime) + dwCurrentTime +1  ;	
		}
	}

	//
	//���������ݣ����ù��λ������
	//
	
	pFocus->dwShowCount     = 0;
	pFocus->dwClickCount    = 0;
	pFocus->dwPlayTime      = 0;
	pFocus->dwDownloadCount = 0;
	pFocus->dwLoadCount     = 0;

	//
	//������ʼʱ��
	//
	if(bClose)
	{
		//Flash DDS���û�������ر�
		pFocus->dwStartPalyTime = 0;
	}
	else if(Type == AD_3D || Type==AD_SEARCH)
	{
		//3Dģ�ͺ�Search����Ҫ����ʱ��,��ֹ�����ͣ��ͳ�Ʋ���ʱ��
		pFocus->dwStartPalyTime = 0;
	}
	else
	{
		//Flash DDSû�б��ر�
		pFocus->dwStartPalyTime = dwCurrentTime;
	}
	
	return TRUE;
}


//---------------------------------------------------------------
//������Ч�Ĺ�ע�ȵ�������
//---------------------------------------------------------------
int   KADManger::GetFocusDataToBuf(char* pData,DWORD&  dwLen)
{
	int  nResult = 0;  

	//
	//��ʼ��ͷ����
	//
	IGA_FOCUS_RESPONSE*   pRequest =(IGA_FOCUS_RESPONSE*) pData;
	memset(pRequest,0,sizeof(IGA_FOCUS_RESPONSE));        
	pRequest->dwLength = sizeof(IGA_FOCUS_RESPONSE);

	//
	//���λ���ݵ�Ԫ
	//
	RESPONSE_FOCUS2* pOutFocus = (RESPONSE_FOCUS2*)(pRequest+1);   

	for (AD_MAP_IT it  = m_ads.begin(); it != m_ads.end() ; it++ ) 
	{	
		//
		//���������
		//
		if  ( pRequest->dwLength +sizeof(RESPONSE_FOCUS2) > dwLen)
			break;

		KAd*       pAD    = it->second;

		//
		//��Ч�Լ��
		//
		if(!pAD)
		   continue;

		tagFocus*  pFocus = pAD->m_pFocus;	   
		if  ( !pFocus)
		   continue;

		//
		//��Ч���ݼ��,��������������˿�ʼʱ�䣬���ڲ���ʱ�䣬����ʾ
		//
		if ( pFocus->dwClickCount     == 0 && 
			pFocus->dwStartPalyTime  == 0 &&
			pFocus->dwPlayTime       == 0 &&
			pFocus->dwShowCount      == 0 )
			continue;


		memset(pOutFocus,0,sizeof(RESPONSE_FOCUS2));
		
		//
		//��ȡ����
		//
		GetInforFromAD(pOutFocus,pFocus,pAD->type);

		//
		//��־����
		//
		if(g_nDebug>1)
		{
			START_LOG;
			char szTmp[300];
			wsprintf(szTmp,
				"Focus Data: [AD]%d [DC]%d [LC]%d [CC]%d [SC]%d [PT]%d ",
				pOutFocus->dwAdID,
				pOutFocus->dwDownloadCount,
				pOutFocus->dwLoadCount,
				pOutFocus->dwClickCount,
				pOutFocus->dwPlayCount,
				pOutFocus->dwPlayTime);
			LOG << szTmp;
			END_LOG;
		}

		//
		//��һ�����λ���ݵ�Ԫ
		//
		pOutFocus++;

		//
		//����ͷ����
		//
		pRequest->dwLength   += sizeof(RESPONSE_FOCUS2);
		(pRequest->dwAdCount)++;

		nResult++;
   }

   dwLen = pRequest->dwLength;

   //
   //��־����
   //
   if(g_nDebug>1)
   {
	   START_LOG;
	   char szTmp[200];
	   wsprintf(szTmp,
		   "Get Focus Data: [BUF]%d [COUNT]%d ",
		   pData,	nResult);
	   LOG << szTmp;
	   END_LOG;
   }

   return nResult;
}


/*****************************************************************************************************************
 *  flash  ��video��activeflash����ʱ���Ļص�
 * dwID      :  ���id��
 * dwType    :  1:��ʼ���ţ�2����������
 * dwCurTime :  ��ǰʱ��
 * lpReserve : ����
****************************************************************************************************************/
VOID WINAPI CallBackPlayTimeFocus(DWORD  dwID,DWORD dwType,DWORD  dwCurTime,LPVOID  lpReserve )
{
	if(g_nDebug>1)
		::OutputDebugString("CallBackPlayTimeFocus");

    KAdCoreApp* pApp = CreateADApp();

	KADManger::AD_MAP_IT it  = pApp->m_adManager.m_ads.begin();
	for (; it != pApp->m_adManager.m_ads.end() ; it++ ) 
	{			
		KAd*       pAD    = it->second;

		//
		//��Ч�Լ��
		//
		if(!pAD)
			continue;

		tagFocus*  pFocus = pAD->m_pFocus;	   
		if  ( !pFocus)
			continue;

		//
		//�ȽϹ��ID
		//
		if  ( pAD->m_dwAdID != dwID ) 
			continue;
		//
		//����Flash ���Flash������Ƶ������Ч
		//
		if  ( pAD->type != AD_FLASH && pAD->type != AD_ACTIVEFLASH &&pAD->type != AD_VIDEO)    
			continue;

		
		//
		//1:��ʼ���ţ����õ�ǰʱ�䣬����ȡ���λ����
		//
		if ( dwType == 1 )
		{
			pFocus->dwStartPalyTime  =  dwCurTime;
		}

		//
		//2:�������ţ����㲥��ʱ��
		//
		if ( dwType == 2 )
		{
			if(dwCurTime > pFocus->dwStartPalyTime)
				pFocus->dwPlayTime    = dwCurTime - pFocus->dwStartPalyTime;
			else
				pFocus->dwPlayTime    = dwCurTime +1+ (0xFFFFFFFF- pFocus->dwStartPalyTime);
		}

		//
		//��־����
		//
		if(g_nDebug>1)
		{
			START_LOG;
			char szTmp[200];
			wsprintf(szTmp,
				"CallBackPlayTimeFocus: [ID]%d [TYPE]%d [TIME]%d [CUTIME]%d [ST]%d",
				pFocus->dwAdID,
				dwType,
				pFocus->dwPlayTime,
				dwCurTime,
				pFocus->dwStartPalyTime);
			LOG << szTmp;
			END_LOG;

		}

		//�˳�ѭ��
		break;
   }
}


/*****************************************************************************************************************
 *  ģ�͵����Search����Ļص�
 * dwID      :  ģ��id��
 * lpReserve : ����
****************************************************************************************************************/
VOID WINAPI CallBack3DFocus(DWORD  dwID,LPVOID  lpReserve )
{
	if(g_nDebug>1)
		::OutputDebugString("CallBack3DFocus");

    KAdCoreApp* pApp = CreateADApp();

	KADManger::AD_MAP_IT it  = pApp->m_adManager.m_ads.begin();
	for (; it != pApp->m_adManager.m_ads.end() ; it++ ) 
	{			
		KAd*       pAD    = it->second;

		//
		//��Ч�Լ��
		//
		if(!pAD)
			continue;

		tagFocus*  pFocus = pAD->m_pFocus;	   
		if  ( !pFocus )
			continue;

		//
		//�ȽϹ��ID
		//
		if  ( pAD->m_dwAdID != dwID ) 
			continue;

		//
		//������3D���͵ĵ�� ��SEARCH���͵ĵ����Ч
		//
		if  ( pAD->type != AD_3D && pAD->type != AD_SEARCH) 
			continue;
		
		//
		//����ۼ�
		//
		pFocus->dwClickCount ++;

		//
		//��־����
		//
		if(g_nDebug>1)
		{
			START_LOG;
			char szTmp[200];
			wsprintf(szTmp,
				"CallBack3DFocus: [ID]%d [TYPE]%d [COUNT]%d",
				pFocus->dwAdID,
				pAD->type,
				pFocus->dwClickCount);
			LOG << szTmp;
			END_LOG;
		}

		//�˳�ѭ��
		break;
   }
}


/*****************************************************************************************************************8
 *  �������ü����Ļص�
 * pDevice   :  �豸ָ��
 * lnCount   :  ���ĵ����ü���
 * lpReserve :  ����
****************************************************************************************************************/
#include "D3DHelpFun.h"
VOID WINAPI CallBackDeviceRefCount(VOID*  pDevice,long lnCount,LPVOID lpReserve)
{
	if(g_nDebug>1)
		::OutputDebugString("CallBackDeviceRefCount");

	if  ( lnCount == 0 || !pDevice )
		return;

	IGA_D3D_HELP::AddDeviceRefCount((LPDIRECT3DDEVICE)pDevice,lnCount);
		
	if(g_nDebug>1)
	{
		START_LOG;
		char szTmp[200];
		wsprintf(szTmp,
			"CallBackDeviceRefCount: [DEVICE]%08X [COUNT]%d",
			pDevice,
			lnCount);
		LOG << szTmp;
		END_LOG;
	}
}