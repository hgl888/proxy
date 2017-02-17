#if !defined(_ADCOREAPP_H)
#define _ADCOREAPP_H

#pragma once

#include "CoreApp.h"
#include "AppHelp.h"
#include "z_Hardware.h"
#include "IGAComm.h"
#include "Compression.h"
#include "Ad_3DFilter.h"
#include "Ad.h"
#include "Encrypt.h"


class KAdCoreApp : public KCoreApp
{
	friend IGA_Filter_IDirect3D;
public:
	KAdCoreApp();	
	virtual ~KAdCoreApp();
	
public:
	/*��ʼ�ӹ�*/
	VOID* HookD3D   ( VOID*  pD3D);

	/*��ini�����ļ�*/
	BOOL  LoadConfig();

	/*����������ļ�*/
	BOOL  LoadADXml(AD_XML_Info& adInfo);

	/*���¹�������ļ�У��ֵ*/
	void UpdateXmlCRC(AD_XML_Info& adInfo);

	/* ���ع�浽�豸�� */
	DWORD LoadAD2Device( VOID* pDevice);
	VOID  DeleteAdFromDevice( VOID* pDevice);

protected:
	virtual INT     OnInitThread();
	virtual INT     OnExit    ();	
	virtual INT     OnUpdateByXML      ();	
	virtual INT     OnTimeOut ();

private:
	VOID   Init();

    /* ���أ���ѹzip ָ���Ĺ��λ*/
    BOOL   DownUpdateAD(AD_XML_RECORD&   ad);

	/*  ȡsdk�汾�� */
	VOID   LoadSDKVersion(TCHAR*  szVersion)   ;

    /* ��ѯsdk */
	BOOL   QuerySDK()            ;
	
	/* ��ѯ���� */
	BOOL   QueryDriver()         ;
    
	/* ��ѯ��� */
	BOOL   QueryPlugin()         ;

	/* ��ѯ���� */
	BOOL   QueryConfig()         ;

	/* ��ѯ������� */
	BOOL   QueryAdInfo()         ;

	/* ���ص������ */
	BOOL   DownloadOneAdZip    (LPCTSTR szIP,LPCTSTR szDir,LPCTSTR  szLocalName );

	/* �ϴ�Ӳ����Ϣ */
    BOOL   UploadHardwareInfo()  ;

	/*
	�����µ�XML�ļ����ݣ���֪ͨ�����ʾģ����� 
	*/
    BOOL   ApplyNewXmlEffect()  ;

	/*
	��������ļ�
	*/
	void ClearFilesByXML();

	//��xml�ļ�����ȡ�����ļ���
	void GetFilesFromXML(	CStringArray &fileD3DArray,
							CStringArray &fileDDSArray,
							CStringArray &fileFlashArray,
							CStringArray &fileVedioArray);
	//���ָ��·�����ļ�
	void ClearFilesByPath(CString& strPath,CStringArray &fileArray);

	//�����Ϸ�汾��
	void GetGameVersion(int nGameId,char* lpBuf,int nBufferLen);

public:
    /* �ϴ������Ϣ */
	BOOL   UploadPlayInfo()      ; 

	/* �ϴ���ע��  */
	BOOL   UploadFocus()         ;

private:
	HINSTANCE       m_hInstance;           //��dll���

	//hook��3d����
	DWORD           m_dwHookType;
	TCHAR           m_szHookType[ 20 ];

	//��ʼʱ��  f�������˷��صĿ�ʼʱ��
	DWORD           m_dwStartServerTime;
	DWORD           m_dwStarLocalTime;
    DWORD           m_dwRunTime;         //�ͻ��˵�����ʱ��


	TCHAR           m_szFTPIP[ 20 ];
	IGAComm         m_net;
    IGAPath         m_paths;
    GameInfo        m_game;
	ZHardwareInfo   m_hardware;
	D3D_FILTER      m_3Dfilter;         //3d������

	IP_AREA         m_IPAREA; 

	//��������xml�ļ�����
	AD_XML_Info     m_ServerxmlAD ;	
public:
	KADManger       m_adManager;

	//��Ҫ���µ�����
	AD_XML_Info     m_NeedUpdateTexture;	

	//��ѹ����
	KUnzip          m_unZip; 
	//����
	KBase64Coding   m_encrypt;

};

/*��������*/
KAdCoreApp*  CreateADApp();

#endif // !defined(AFX_ADCOREAPP_H__5773C6E5_84BD_4076_9ED4_9C88BC788BEF__INCLUDED_)
