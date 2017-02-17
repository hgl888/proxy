#include "StdAfx.h"
#include <WinSock2.h>
#include "z_fileini.h"
#include "Ad_3DFilter.h"
#include "GlobalAPI_Val.h"
#include "IGAWin.h"
#include "AdCoreApp.h"
#include "D3DHelpFun.h"

BOOL CheckIP()
{
	return g_bRightIP;
}

BOOL  g_bInUserLock = TRUE;

HWND  m_hWndDevice = NULL;		//�豸��Ӧ�Ĵ��ھ�������������3Dģ��

////////////////////////////////////////////////////////////////
//D3D��������ɴ�����
////////////////////////////////////////////////////////////////

IGA_Filter_IDirect3D::IGA_Filter_IDirect3D()
{
    m_dwRefCount = 0;
}

IGA_Filter_IDirect3D::~IGA_Filter_IDirect3D()
{
}

//------------------------------------------------------------
//ж�ش���ģ�顢ж��3dģ�顢�ϴ���ע��  �� �����Ϣ
//------------------------------------------------------------
void  ReleaseAllResource()
{
	//
	//ж�ش���ģ��
	//
	IGAWIN::CloseFlash()       ;
	IGAWIN::CloseVideo()       ;
	IGAWIN::CloseActiveFlash() ;
	IGAWIN::UnInitWindow()     ;
	
	if(g_nDebug>1)
		LOG <<"D3Dǿ��ж�ش���ģ��. " << "\r\n";
	
	//
	//ж��3dģ��
	//
	if ( NULL != g_pIGA3DDestoryt)
	{
		(*(FUN_IGA3DDestory )g_pIGA3DDestoryt)();
		UnInit3DCore();	
		if(g_nDebug>1)
			LOG <<"D3Dǿ��ж��3Dģ��. " << "\r\n";
	}	
	
	//
	//�ϴ���ע��  �� �����Ϣ 	
	//
	
	if(CheckIP())
	{
		if(g_nDebug>1)
		{
			LOG << "�豸�ͷ�ǰ�ϴ���ע��  �� �����Ϣ" << "\r\n";
			LOG << "ip ��Ч" << "\r\n";
		}
		
		KAdCoreApp* p = CreateADApp() ;
		p->UploadFocus()     ;
		p->UploadPlayInfo()  ;
	}
	else
	{
		if(g_nDebug>1)
			LOG << "�ϴ���ע: δ����" << "\r\n";
	}

}

//------------------------------------------------------------
//�����IGA_Filter_IDirect3DSurface::OnUnlockRect_Before��δ���е�������滻
//------------------------------------------------------------
BOOL CompleteMultiTextureReplace(IDirect3DDevice* pDevice)
{
	KAdCoreApp* pApp = CreateADApp();
	for ( KADManger::AD_MAP_IT  it = pApp->m_adManager.m_ads.begin(); it != pApp->m_adManager.m_ads.end(); it++)
	{
		if  ( it->second->type != AD_TEXTURE ) 
			continue;
		
		KTextureAD*  pTextureAD = (KTextureAD*)it->second;  
      	if(!pTextureAD)				
			continue;

		KTextureAD::DEV_MAP_IT  pDev =  pTextureAD->devs.find(pDevice);
		if ( pDev == pTextureAD->devs.end() ) 	
			continue;
		
		TextureADContent&   ad = pDev->second;
		if  ( ad.m_pMexture == NULL || ad.m_pADTexture == NULL || ad.bAllReplace)
			continue;

		
		DWORD  dwLevelCount = ((IDirect3DTexture*)(ad.m_pMexture))->GetLevelCount();
		
		for ( DWORD dwLevel = 0 ;dwLevel < dwLevelCount ;dwLevel++)
		{						
			if ( IGA_D3D_HELP::Texture_DoReplace((IDirect3DTexture*)(ad.m_pADTexture),(IDirect3DTexture*)(ad.m_pMexture),dwLevel) )
			{
				START_LOG;
				LOG << "��ȫ�滻�ɹ�  level = " << dwLevel   << " ID= " << pTextureAD->m_dwAdID;
				END_LOG;
			}
			else
			{
				ad.m_pMexture = NULL;
				
				START_LOG;
				LOG << "��ȫ�滻ʧ�� level=" << dwLevel  << " ID= " << pTextureAD->m_dwAdID;
				END_LOG;
				
				//�Ѹù��λ���õ�m_dwNeedReplaceTexture
				KADManger  & adManager = pApp->m_adManager;
				if (  adManager.m_dwNeedReplaceTexture.IsCreate(pDevice) )
				{ 
					adManager.m_dwNeedReplaceTexture.d_array [ pDevice ].insert(pTextureAD->m_dwAdID );					
					ad.bAllReplace  = TRUE;
					break;
				}
				
			}
			ad.bAllReplace  = TRUE;
		}					
	}
	return TRUE;
}
VOID    IGA_Filter_IDirect3D::OnRelease_After      (ULONG  ulRefCount)
{
	try
	{
		if(g_nDebug>1)
		{
			START_LOG;
			LOG << "IDirect3D::OnRelease:   count= " << ulRefCount<< "\r\n";
		}
	
		if ( 0 == ulRefCount )
		{	
			if(g_nDebug>1)
				END_LOG;
			
			return;
		}

		if ( g_dwD3DDeviceCount != 0 )
		{
			////ж�ش���ģ�顢ж��3dģ�顢�ϴ���ע��  �� �����Ϣ
			ReleaseAllResource();

			g_dwD3DDeviceCount = 0;
		}

		if(g_nDebug>1)
			END_LOG;

	}catch(...)
	{
		START_LOG;
		LOG << "OnRelease_After Exception"  ;
		END_LOG;

		if(g_nDebug>0)
			OutputDebugString("OnRelease_After Exception");
	}

}
VOID    IGA_Filter_IDirect3D::OnCreateDevice_After (IDirect3DDevice*  pDevice,HWND hFocusWindow,IDirect3DDevice* pDeviceOrg)
{
	try
	{
	BOOL  bRet  = FALSE;

	if(g_nDebug>1)
	{
		START_LOG;
		LOG <<"IDirect3D::OnCreateDevice  [DEVICE]=" << (DWORD)pDevice   << "\r\n";
	}

	//
	//
	//
	IGA_D3D_HELP::SetDeviceRefCount(pDevice,0);		
	
	//
	//���ش���ģ��
	//
	bRet = IGAWIN::InitWindow(CallBackPlayTimeFocus);
	if(g_nDebug>1)
		LOG <<"���ش���ģ�� ��ret="   << (bRet ? "TRUE"  : "FALSE") << "\r\n";	

	//
	//���ر���XML�����й��λ��������Ӧͳ�����ݽṹ
	//
	KAdCoreApp* pApp = CreateADApp();
    pApp->LoadAD2Device(pDevice);   

	if(g_nDebug>1)
		LOG << "���ر��ع���ļ�" << "\r\n";
	
	//
	//����3dģ��	
	//
	bRet = Init3DCore();
	if(g_nDebug>1)
		LOG <<"����3dģ�� ��ret="   << (bRet ? "TRUE"  : "FALSE") << "\r\n";

 	if ( NULL != g_pFunIGA3DInit)
 	{
		m_hWndDevice = hFocusWindow;
 		g_dw3DRanderPos = (*(FUN_IGA3DINIT)g_pFunIGA3DInit)(hFocusWindow,pDeviceOrg,pDevice,CallBack3DFocus,CallBackDeviceRefCount);
 	}	
	
	g_dwD3DDeviceCount++;

	if(g_nDebug>1)
		END_LOG;
	}catch(...)
	{
		START_LOG;
		LOG << "OnCreateDevice_After Exception"  ;
		END_LOG;
		
		if(g_nDebug>0)
			OutputDebugString("OnCreateDevice_After Exception");
	}
}

////////////////////////////////////////////////////////////////
//�豸��������ɴ�����
////////////////////////////////////////////////////////////////

IGA_Filter_IDirect3DDevice::IGA_Filter_IDirect3DDevice()
{

}
IGA_Filter_IDirect3DDevice::~IGA_Filter_IDirect3DDevice()
{

}

VOID IGA_Filter_IDirect3DDevice::OnRelease_After                 (IDirect3DDevice* pDevice,ULONG&  ulRefCount )
{	
	try
	{
 	IGA_D3D_HELP::DeviceReleaseLock(pDevice);

	int nRefCount = IGA_D3D_HELP::GetDeviceRefCount(pDevice);

	if(g_nDebug>1)
	{
		START_LOG;
		char szTmp[300];
		wsprintf(szTmp,"IDirect3DDevice::OnRelease   [DC]%d [RC]%d [DEVICE]%08X\r\n",
			ulRefCount,nRefCount,pDevice);
		LOG << szTmp ;
	}

	if ( nRefCount == ulRefCount )
	{	
		////ж�ش���ģ�顢ж��3dģ�顢�ϴ���ע��  �� �����Ϣ
		ReleaseAllResource();

		//ɾ�����豸��������Դ
		if(g_nDebug>1)
			LOG << "ж�ع���ļ�" << "\r\n";

		//
		//����m_adManager.Clear();
		//
		CreateADApp()->DeleteAdFromDevice(pDevice);

		g_dwD3DDeviceCount--;
		ulRefCount = 0;				
	}

	if(g_nDebug>1)
		END_LOG;

 	IGA_D3D_HELP::DeviceReleaseUnLock(pDevice);	
	
	}catch(...)
	{
		START_LOG;
		LOG << "OnRelease_After�쳣"  ;
		END_LOG;

		//
		//�쳣��������豸��������
		//
		g_dwD3DDeviceCount=0;
		ulRefCount = 0;	
		OutputDebugString("OnRelease_After Exception");
	}
}

//��תǰ��
VOID IGA_Filter_IDirect3DDevice::OnPresent_Before                (IDirect3DDevice* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if ( NULL !=g_pIGA3DRender && g_dw3DRanderPos == Per_Present)
	{
		(*(FUN_IGA3DRender)g_pIGA3DRender)();
	}
}
VOID IGA_Filter_IDirect3DDevice::OnPresent_After                 (IDirect3DDevice* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if ( NULL != g_pSetFristDraw)
	{	
		(*(FUN_ISetFristDraw )g_pSetFristDraw)();
	}
}

//�����豸ǰ��
VOID IGA_Filter_IDirect3DDevice::OnReset_Before                  (IDirect3DDevice* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if( g_pIGA3DDestoryt!= NULL )
	{
		(*(FUN_IGA3DDestory )g_pIGA3DDestoryt)();
		g_dw3DRanderPos = -1;
	}
}
VOID IGA_Filter_IDirect3DDevice::OnReset_After                   (IDirect3DDevice* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice* pOrgDevice)
{	
	if( NULL != g_pFunIGA3DInit )
	{
		g_dw3DRanderPos = (*(FUN_IGA3DINIT)g_pFunIGA3DInit)(m_hWndDevice,pOrgDevice,pDevice,CallBack3DFocus,CallBackDeviceRefCount);
	}
}

//����ת�þ���ǰ
VOID IGA_Filter_IDirect3DDevice::OnSetTransform_Before           (IDirect3DDevice* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{	
	if ( NULL != g_pIGA3DGetMatrixData)
	{		
		(*(FUN_IGA3DGetMatrixData)g_pIGA3DGetMatrixData)( State, pMatrix );
	}

	if(  NULL !=g_pIGA3DRender && Per_SetTransform == g_dw3DRanderPos)
	{
		if ( D3DTS_WORLD == State )
		{
			(*(FUN_IGA3DRender)g_pIGA3DRender)();
		}
	}
	
}

//����ת�þ����
VOID IGA_Filter_IDirect3DDevice::OnSetTransform_After           (IDirect3DDevice* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{	
	if( NULL !=g_pIGA3DRender && Per_SetTransform == g_dw3DRanderPos )
	{
		
		if ( D3DTS_WORLD == State)
		{
			(*(FUN_IGA3DRender)g_pIGA3DRender)();
		}
	}
}

VOID IGA_Filter_IDirect3DDevice::OnSetTexture_Before             (IDirect3DDevice* pDevice, DWORD Stage, IDirect3DBaseTexture* pTexture)
{
	if  ( NULL == pTexture)  
	{
		return;
	}
	
	try
	{
	KAdCoreApp* pApp = CreateADApp();

	//�Ƿ������Ҫ��ȫ�滻������
    if  (  pApp->m_adManager.m_dwReplaceADTextureCount )
	{
		//����ڱ������ǰ��δ���е�������滻
		CompleteMultiTextureReplace(pDevice);
		pApp->m_adManager.m_dwReplaceADTextureCount --;
	}
   
	//
	// ���豸���Ƿ�����Ҫ���ж�̬�滻������
	//
	KADManger  & adManager = pApp->m_adManager;
	if ( ! adManager.m_dwNeedReplaceTexture.IsCreate(pDevice) )  
		return;
	
	//��Ҫ���ж�̬�����滻�����й��λID���ʹ��豸��ص�
	KADManger::AD_WAIT_MATCH_ID&  waitID = adManager.m_dwNeedReplaceTexture.d_array[pDevice];
	for ( KADManger::AD_WAIT_MATCH_ID_IT it0 = waitID.begin() ; it0  != waitID.end(); it0++)
	{
		DWORD   dwADID = *it0;

		/*
		���ID����Ϊ����߸�ֵ
		*/
		if  ( dwADID ==0)
		{
			START_LOG;
			LOG << __FILE__ << " " << __LINE__ << " ID:" << dwADID  ;
			END_LOG;
			//waitID.erase(it0);
			continue;
		}

		//
		//����ID������������Ϣ
		//
		KADManger::AD_MAP_IT  it = CreateADApp()->m_adManager.m_ads.find(dwADID);
		if  ( it == CreateADApp()->m_adManager.m_ads.end() )
		{
			START_LOG;
			LOG << __FILE__ << " " << __LINE__ << " ID:" << dwADID  ;
			END_LOG;
			//waitID.erase(it0);
			continue;
		}

		//
		//ȷ�ϴ������Ƿ���ڵ�ǰ�豸
		//
		KTextureAD*  pTextureAD = (KTextureAD*)it->second;        		
		KTextureAD::DEV_MAP_IT  pDev =  pTextureAD->devs.find(pDevice);
		if ( pDev == pTextureAD->devs.end() ) 
		{
			START_LOG;
			LOG << __FILE__ << " " << __LINE__ << " ID:" << dwADID  ;
			END_LOG;
			continue;
		}

		TextureADContent&   ad = pDev->second;			 
		
		IDirect3DTexture*  pMatchTex = (IDirect3DTexture*)ad.m_pMatchTexture ;
		IDirect3DTexture*  pADTex    = (IDirect3DTexture*)ad.m_pADTexture ;

		//
		//ȷ��ԴĿ������ָ����Ч
		//
		if (  NULL == pMatchTex ||  NULL == pADTex )
		{
			START_LOG;
			LOG << __FILE__ << " " << __LINE__ << " ID:" << dwADID  ;
			END_LOG;
			//waitID.erase(it0);
			continue;
		}
		
		//
		//�Ƚ�
		//
		g_bInUserLock = FALSE;
		if ( IGA_D3D_HELP::Texture_DoCompare(pMatchTex,(IDirect3DTexture*)pTexture,0))
		{	

			START_LOG;
            LOG << "REPLACE  id=" << pTextureAD->m_dwAdID << pTextureAD->m_MatchFileName;
            END_LOG;

			for ( int i =0; i < pTexture->GetLevelCount();i++ )
				IGA_D3D_HELP::Texture_DoReplace(pADTex,(IDirect3DTexture*)pTexture,i);					


			//
			//���ò�����ʼʱ��
			//
			pTextureAD->m_pFocus->dwStartPalyTime = ::GetTickCount();
			
			//�����ع���
			pTextureAD->m_pFocus->dwShowCount++;
			
			ad.m_pMexture = pTexture;
			waitID.erase(it0);
			g_bInUserLock = TRUE;
			break;
		}
		g_bInUserLock = TRUE;					
	}

	}catch(...)
	{
 		START_LOG;
 		LOG << "OnSetTexture_Before Exception"  ;
 		END_LOG;
		OutputDebugString("OnSetTexture_Before");
	}

}

////////////////////////////////////////////////////////////////
//����COM��������ɴ�����
////////////////////////////////////////////////////////////////

IGA_Filter_IDirect3DTexture::IGA_Filter_IDirect3DTexture(){}
IGA_Filter_IDirect3DTexture::~IGA_Filter_IDirect3DTexture(){}

VOID    IGA_Filter_IDirect3DTexture::OnRelease_After      (IDirect3DDevice*pDevice,IDirect3DTexture*  pTexture,ULONG  ulRefCount       )
{	
	//
	//�������ã�������
	//
	if ( ulRefCount != 0 ) 
		return ;

	try
	{
		//
		//������ڼ�������������
		//
		if  (  CreateADApp()->m_adManager.IsLoadingTexture() ) 
			return;

		//���Ҽ��صĹ���Ƿ�����˴�����
		KADManger::AD_MAP&  adMap = CreateADApp()->m_adManager.m_ads;    
		for (KADManger::AD_MAP_IT  it = adMap.begin() ; it != adMap.end() ;it++ )
		{
			//
			//ֻ������������
			//
			if  ( it->second->type != AD_TEXTURE) 
				continue;

			KTextureAD*  pTextureAD = (KTextureAD*)it->second;        
			KTextureAD::DEV_MAP_IT  pDev =  pTextureAD->devs.find(pDevice);

			//
			//û���ҵ����豸������
			//
			if ( pDev == pTextureAD->devs.end() )
				continue;

			//
			//�Ƚ�����ָ��
			//
			if  ( pTexture != pDev->second.m_pMexture)
				continue;

			//
			//��־����
			//
			if(g_nDebug>1)
			{
 				START_LOG;
                 LOG << _T("OnRelease_After   [ADID] = ") << pTextureAD->m_dwAdID    << _T("\r\n");
 				END_LOG;
			}

			//
			//�������ʱ��
			//
			DWORD dwTime = ::GetTickCount();
			if(dwTime > it->second->m_pFocus->dwStartPalyTime)
				it->second->m_pFocus->dwPlayTime = dwTime - (it->second->m_pFocus->dwStartPalyTime); 
			else
				it->second->m_pFocus->dwPlayTime = 0xFFFFFFFF - (it->second->m_pFocus->dwStartPalyTime) + dwTime+1; 
			
			pDev->second.m_pMexture = NULL;

			//
			//�˹�����²��룬�ȴ������滻
			//
			KAdCoreApp* pApp       = CreateADApp();
			KADManger  & adManager = pApp->m_adManager;
			adManager.m_dwNeedReplaceTexture.d_array[pDevice].insert(pTextureAD->m_dwAdID) ; 
		
	}

	}catch(...)
	{
		START_LOG;
		LOG << "OnRelease_After Exception"  ;
		END_LOG;

		if(g_nDebug>0)
			OutputDebugString("OnRelease_After");
	}
}

////////////////////////////////////////////////////////////////
//����COM��������ɴ�����
////////////////////////////////////////////////////////////////

IGA_Filter_IDirect3DSurface::IGA_Filter_IDirect3DSurface(){}
IGA_Filter_IDirect3DSurface::~IGA_Filter_IDirect3DSurface(){}
VOID    IGA_Filter_IDirect3DSurface::OnLockRect_After (IDirect3DSurface* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) 
{	
	try{

	if  (  !g_bInUserLock)  return;
    if  (  CreateADApp()->m_adManager.IsLoadingTexture() ) return;
	if  ( Flags & D3DLOCK_READONLY )   return;
	if  ( pRect != NULL)               return;	
	
	//
	//�ڱ������ǰ��Ҫ�������Ĳ���
	//
	SURFACE_PARAM   param={pLockedRect->Pitch, pLockedRect->pBits ,pRect,Flags};
	m_lockSurface.insert( CUR_SURFACE_MAP::value_type(pSurface,param));

	}catch(...)
	{
		START_LOG;
		LOG << "OnLockRect_After�쳣"  ;
		END_LOG;
		OutputDebugString("OnLockRect_After�쳣");
	}
}

VOID    IGA_Filter_IDirect3DSurface::OnUnlockRect_Before(IDirect3DDevice* pDevice,IDirect3DSurface* pSurface) 
{
	if  (  !g_bInUserLock)  return;
	static   BOOL   bReplacing = FALSE;
	if  ( bReplacing )   return;
	try
	{

	//�Ƿ������滻��

	m_pTexture = NULL ;

	//�����Ƿ���Ч�ı���
	CUR_SURFACE_MAP_IT  it1 = m_lockSurface.find(pSurface);
	if  ( it1 == m_lockSurface.end() )	return;

    SURFACE_PARAM  param = (*it1).second;
	bReplacing = TRUE;	
	
	IDirect3DSurface*      pAdSurface    = NULL;
	IDirect3DSurface*      pMatchSurface = NULL;
	D3DLOCKED_RECT         LockedRect   ;
	D3DSURFACE_DESC        SurfaceDesc  ;
	D3DSURFACE_DESC        MatchSurfaceDesc ;

	//LOG << " ��ʼ����Ƚ�";
	KAdCoreApp*         pApp  =    CreateADApp();	
	KADManger::AD_MAP&  adMap = pApp->m_adManager.m_ads;  	


	for (KADManger::AD_MAP_IT  it = adMap.begin() ; it != adMap.end() ;it++ )
	{		
		if  ( it->second->type != AD_TEXTURE) continue;

		//���ҹ��ͼ���Ƿ��д��豸��
		KTextureAD*  pTextureAD = (KTextureAD*)it->second;        		
		KTextureAD::DEV_MAP_IT  pDev =  pTextureAD->devs.find(pDevice);
        if ( pDev == pTextureAD->devs.end() )  continue;
		

		TextureADContent&   ad = pDev->second;
		//�жϴ˹���Ƿ��Ѿ����滻
	    if ( ad.m_pMexture != NULL )  continue;

		//�ж��Ƿ������Ч������		
		if (  NULL == ad.m_pADTexture ||  NULL == ad.m_pMatchTexture )  continue;
		IDirect3DTexture*  pMatchTex = (IDirect3DTexture*)ad.m_pMatchTexture ;
		IDirect3DTexture*  pADTex    = (IDirect3DTexture*)ad.m_pADTexture ;


		//ȡ��Ҫƥ��Ĺ���������
		
		DWORD  dwLevelCount = pMatchTex->GetLevelCount();
		dwLevelCount =1 ;

		for( DWORD  dwLevel = 0; dwLevel < dwLevelCount ; dwLevel++)
		{
			//ȡ��ƥ����������
			pMatchTex->GetSurfaceLevel(dwLevel,&pMatchSurface);	

			LockRectRaw(pMatchSurface,&LockedRect,NULL,D3DLOCK_READONLY);		

			//�Ƚ�Pitch
			if ( LockedRect.Pitch   !=  param.Pitch ) 
			{
				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();	
				continue;
			}
			
			//
			//�Ƚϸ�ʽ
			//
			pMatchTex->GetLevelDesc(dwLevel,&MatchSurfaceDesc);
			HRESULT hr = pSurface->GetDesc(&SurfaceDesc);
			
			if ( SurfaceDesc.Format != MatchSurfaceDesc.Format )
			{
				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();	
				continue;
			}

			//
			//����
			//
            if ( SurfaceDesc.Height > MatchSurfaceDesc.Height 
				|| SurfaceDesc.Width > MatchSurfaceDesc.Width 
				||  SurfaceDesc.Width == 0 
				|| SurfaceDesc.Height == 0)
			{
				UnlockRectRaw(pMatchSurface);
		        pMatchSurface->Release();	
				break;
			}

			if ( SurfaceDesc.Height != MatchSurfaceDesc.Height || SurfaceDesc.Width != MatchSurfaceDesc.Width )
			{
				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();	
				continue;
			}

			//
			//�������ݱȽ�						
			//
            BOOL  bRet = IGA_D3D_HELP::Surface_DoCompare(param.pBits, LockedRect.pBits, param.Pitch, &SurfaceDesc,FALSE);
			if (! bRet )
			{
				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();
				continue;
			}
			

			pADTex->GetSurfaceLevel(dwLevel,&pAdSurface);

			LockRectRaw(pAdSurface,&LockedRect,NULL,D3DLOCK_READONLY);																					
			
			bRet = IGA_D3D_HELP::Surface_DoReplace((CHAR*)(param.pBits),(CHAR*)(LockedRect.pBits),param.Pitch,&SurfaceDesc);							
			
			UnlockRectRaw(pAdSurface);
			pAdSurface->Release();

			
			if  ( !bRet )
			{
				START_LOG;
				LOG << "��ʼ���滻ʧ��"  << pTextureAD->m_dwAdID;
				END_LOG;

				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();	
				break;
			}

								
			//���浱ǰ����ָ��
			ad.m_pSurface = pSurface;	

			//
			//��ñ����Ӧ������
			//
			void *pContainer = NULL;
			IDirect3DTexture *pTexture = NULL;
			HRESULT hr1 = pSurface->GetContainer(__IID_IDirect3DTexture, &pContainer);
			if (SUCCEEDED(hr1) && pContainer)
			{
				pTexture = (IDirect3DTexture *)pContainer;
				pTexture->Release();
			}
			ad.m_pMexture = pTexture;
			ad.bAllReplace = FALSE;

			//
			//���ò�����ʼʱ��
			//
			pTextureAD->m_pFocus->dwStartPalyTime = ::GetTickCount();

			//�����ع���
			pTextureAD->m_pFocus->dwShowCount++;


			UnlockRectRaw(pMatchSurface);
			pMatchSurface->Release();

			//��ȫ�滻������
			pApp->m_adManager.m_dwReplaceADTextureCount++;
        
			//ȥ����̬�滻
			KADManger::AD_WAIT_MATCH_ID&  waitID = pApp->m_adManager.m_dwNeedReplaceTexture.d_array[pDevice];
			waitID.erase(pTextureAD->m_dwAdID);

			START_LOG;
			LOG << "��ʼ���滻�ɹ�"  << pTextureAD->m_dwAdID;
			END_LOG;
			break;
		}
	}		
	
	bReplacing = FALSE;
	m_lockSurface.erase(it1);

	}
	catch(...)
	{
		bReplacing = FALSE;
		START_LOG;
		LOG << "IGA_Filter_IDirect3DSurface::OnUnlockRect_Before�쳣"  ;
		END_LOG;
		OutputDebugString("OnUnlockRect_Before");
	}
}

