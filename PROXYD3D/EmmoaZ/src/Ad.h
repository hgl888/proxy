//************************************************************************************************************
// class      :  
// purpose    :  ��涨��ṹ��
// Date       :  2008-8-4  9:45:43
// author     :  3o3afe1
//************************************************************************************************************
#if !defined _AD_H
#define _AD_H

#pragma warning(disable:4786)
#include <map>
#include <list>
#include <Set>

#pragma once

#pragma  pack(push ,1)
//��ע����Ϣ
struct RESPONSE_FOCUS
{
	enum
	{
		MAX_FOCUS	= 4,
	};
	DWORD	dwAdID;						//	���ID��
	DWORD	dwDownloadCount;			//	���ش���
	DWORD	dwLoadCount;				//	�����Դ�����صĴ���
	DWORD	dwClickCount;				//	��汻����Ĵ���
	struct {
		union
		{
			DWORD	dwNavigateCount;	//	�������򱻲��ŵĴ���
			DWORD	dwPlayCount;	
		};
		DWORD	dwFullPlayCount;		//	��汻�������ŵĴ���
		DWORD	dwPlayTime;				//	��汻���ŵ�ʱ����
	}	pi[MAX_FOCUS];	
};

struct RESPONSE_FOCUS2
{
	
	DWORD	dwAdID;						//	���ID��
	DWORD	dwDownloadCount;			//	���ش���
	DWORD	dwLoadCount;				//	�����Դ�����صĴ���

	DWORD	dwClickCount;				//	��汻����Ĵ���   �������ع����	

	DWORD	dwPlayCount;		        //	�������ع����
	DWORD	dwFullPlayCount;			//	��������������
	DWORD	dwPlayTime;		            //  �������ع�ʱ��

	DWORD	dwReserve1;		            //  �����ع����
	DWORD	dwReserve2;		            //  ������������
	DWORD	dwReserve3;		            //  �����ع�ʱ��

	DWORD	dwReserve4;		            //  �Ƚ������ع����
	DWORD	dwReserve5;		            //  �Ƚ�������������
	DWORD	dwReserve6;		            //  �Ƚ������ع�ʱ��

	DWORD	dwReserve7;		            //  ģ���ع����
	DWORD	dwReserve8;		            //  ģ����������
	DWORD	dwReserve9;		            //  ģ���ع�ʱ��

};

//Ϊ�˱�����ǰ��һ��

#pragma  warning(disable:4200)
struct IGA_FOCUS_RESPONSE
{
	DWORD	dwLength;		  
	BYTE	byType;			  
	BYTE	byFlags;		  // = 0
	BYTE	byCount;		  
	BYTE	byFrom;			  
	
	BYTE	byRequestType;		//	����ͷ����
	BYTE	byFlags2;			//	�Ӱ���ͷ����
	WORD	wRequestHeader;		//	����ͷ�����ȣ������Ӱ�ͷ����
	
	DWORD	dwGameID;                     //��Ϸid
	union
	{
		struct  {
			DWORD	dwReserved[2];
		};
		struct  {
			DWORD	dwAreaID;
			DWORD	dwAddress;
			
		};
	};
	DWORD	dwAdCount;           //�������
	CHAR	szMacID[ 18 ];       //mac��ַ
	
	RESPONSE_FOCUS   FocusData[0];
};


#pragma pack(pop)


/* ��������¼ */
struct  AD_XML_RECORD
{ 
public:
	DWORD      dwID;                //���id��
	DWORD      dwClose;
	
	
	DWORD      dwAdVersion;         //���汾��
	DWORD      dwOrigFileVersion;   //Ŀ���ļ��汾��
	
	DWORD      dwAdType;            //_tagADType
	DWORD      dwReserve1;          //�����Ƿ�ɾ��
	
	TCHAR      szOrigFile [ _MAX_PATH  ] ;
	TCHAR      szAdFile   [ _MAX_PATH  ] ;
	
    //ftp����
	TCHAR      szIP       [ 20  ] ;
	TCHAR      szftpDir   [ 256 ] ;

	inline     BOOL IsColsed ()  const { return (dwClose != 0 ) ;}
	
};


/************************************************************************/
/* ����ע������                                                       */
/************************************************************************/
typedef struct tagFocus
{	
	DWORD	dwAdID;						//	���ID��

	DWORD	dwDownloadCount;			//	���ش���
	DWORD	dwLoadCount;				//	�����Դ�����صĴ���
	DWORD	dwClickCount;				//	��汻����Ĵ���  3d
	
	DWORD   dwShowCount;                //  �������ʾ����  �����������
	
	DWORD   dwStartPalyTime ;
	DWORD	dwFullPlayCount;		//	��汻�������ŵĴ���
	DWORD	dwPlayTime;				//	��汻���ŵ�ʱ����

	

}FOCUS, *LPFOCUS;

/********************************************************************************************************************/
/*                 ���                                                                                             */
/********************************************************************************************************************/
typedef enum
{
    AD_TEXTURE,
	AD_FLASH,
	AD_VIDEO,
	AD_3D,
	AD_ACTIVEFLASH,
	AD_SEARCH
} _tagADType;

enum ENUM_IGA_GAME_LIST
{
		IGA_NO_GAME = 0,
		IGA_FENGYUN,			//����
		IGA_GONGFUSHIJIE,		//��������
		IGA_GUANGZHIGUODU,		//��֮����
		IGA_SHENQI,				//����
		IGA_TEZHONGBUDUI,		//���ֲ���
		IGA_HANGHAISHIJI,		//��������
		IGA_JIJIASHIJI,			//��������
		IGA_WUJIEQU	,			//�����
		IGA_HANGHAISHIJI_EX	,	//�������͸��°�
		IGA_JINGTIANDONGDI,		//���춯��
		IGA_LONGHUMEN,			//������
		IGA_HAIZEIWANG	,		//������
		IGA_TIANSHANGRENJIAN,	//�����˼�
		IGA_JUREN,				//����
		IGA_KANGZHAN,			//��ս
		IGA_LUOQI,				//����
		IGA_PAOPAOKADINGCHE,		//���ܿ�����
		IGA_REXUEJIANGHU,		//��Ѫ����
		
		
		IGA_SILUCHUANSHUO,		//˿·��˵
		IGA_TIANJI,				//���
		IGA_TIANLONGBABU,		//�����˲�  
		IGA_WANMEISHIJIE,		//��������	
		IGA_ZHUXIAN	,			//����	
		IGA_WEIWUDUZUN,			//Ψ�����	
		IGA_ZHUOYUEZHIJIAN,		//׿Խ֮��	
		IGA_CHUNQIU_Q_ZHUAN, 	//����Q��	
		IGA_FENGHUOZHILV,		//���֮��	
		IGA_MENGJIANG,			//�ͽ�	
		IGA_ZHENGFU	,			//����
		IGA_MOYU,				//����
		IGA_XINYANG,				//����--����ս��	
		IGA_ZHANHUO,				//ս��	
		IGA_JIETOULANQIU,		//��ͷ����	
		IGA_TIANTANG2,			//����2
		IGA_SUN,					//�漣����
		IGA_TIANZUN,				//����OnLine
		IGA_WLOL,				//����Ⱥ���� 
		IGA_GV,					//�󺽺�ʱ��Online
		IGA_RELIPAIQIU,			//��������	
		
		IGA_WUJIEQU_MLXY	=155,	//�����(��������)
		IGA_WUJIEQU_XJP,			//�����(�¼���)
		IGA_WUJIEQU_FLB		=158,	//�����(���ɱ�)
};

struct KAd 
{
 	DWORD	       m_dwAdID               ;	 //���id	
    _tagADType     type                   ;   //�������

    DWORD          m_dwAdVersion          ;   //��ǰ���İ汾��
	DWORD          m_dwMatchVersion       ;   //��ǰ���İ汾��

	DWORD          m_bLoad                ;   //�˹���Ƿ��Ѿ�������
	tagFocus*      m_pFocus               ;   //����ע��ָ��   
};

/*������*/
struct TextureADContent
{
		VOID*     m_pADTexture    ;               //�������
		VOID*     m_pMatchTexture ;               //ƥ������
		
		VOID*     m_pSurface      ;         
		VOID*     m_pMexture      ;               //��������ص������ַ

		BOOL      bAllReplace     ;

};
struct KTextureAD : public KAd
{

	TCHAR     m_adFileName      [ _MAX_PATH ] ;   //����ļ�����
	TCHAR     m_MatchFileName   [ _MAX_PATH ] ;   //ƥ���ļ�����

	typedef   std::map<void*,TextureADContent>     DEV_MAP;
	typedef   DEV_MAP::iterator                    DEV_MAP_IT;
	
	
    DEV_MAP   devs;
 

};

struct KFlashAD   : public KAd
{
    TCHAR     m_FlashName       [ _MAX_PATH ] ;   //flash�ļ�����
	
};
struct KVideoAD   : public KAd
{
    TCHAR     m_VideoName       [ _MAX_PATH ] ;  //��Ƶ�ļ�����	
};

/*3dģ�͹��*/
struct  MeshADContent
{

};
struct K3DAD      : public KAd
{
	TCHAR     m_3DName          [ _MAX_PATH ] ;   //3D�ļ�����	

	typedef   std::map<void*,MeshADContent>     DEV_MAP;
	typedef   DEV_MAP::iterator                 DEV_MAP_IT;
	
	DEV_MAP   devs;

};

struct KActiveFlashAD      : public KAd
{
    TCHAR     m_ActiveFlashName [ _MAX_PATH ] ;   //�flash�ļ�����	

	};

struct KSearchAD      : public KAd
{
    TCHAR     m_SearchName [ _MAX_PATH ] ;   //�ļ�����	

};




/*****************************************************************************************************************/
/******************�����ļ��еĹ��********************************************************************************/




struct  AD_XML_Info
{
public:
    AD_XML_Info()     {    ::InitializeCriticalSection(&m_cr); }
	~AD_XML_Info()    {    ::DeleteCriticalSection(&m_cr)    ; }

	
	inline VOID  add (AD_XML_RECORD&   ad)
							{    m_ads.push_back(ad);	}
	inline VOID  clear()	{   m_ads.clear();	}

	inline BOOL  TryLock () {	return ::TryEnterCriticalSection(&m_cr); }
	inline VOID  Lock  ()   { ::EnterCriticalSection(&m_cr);        }	
	inline VOID  Unlock()   { ::LeaveCriticalSection(&m_cr);        }
	

	
	typedef std::list<AD_XML_RECORD>    AD_RECORD;
	typedef AD_RECORD::iterator         AD_RECORD_IT;
	
	DWORD       dwCRC;
	AD_RECORD   m_ads;

private:	
    CRITICAL_SECTION    m_cr;
	
};

#include <deque>
template < class T>
struct   D3D_DEVICE_ARRAY
{
	typedef std::map<VOID*,T>   D3D_DEVICE_ARRARY_ELEMENT;
	D3D_DEVICE_ARRARY_ELEMENT   d_array;


    D3D_DEVICE_ARRAY(){}
	~D3D_DEVICE_ARRAY(){}	
	
	//�豸�Ƿ��Ѵ���
	BOOL IsCreate(VOID* pDevice ) 
	{
		D3D_DEVICE_ARRARY_ELEMENT::iterator it = d_array.find(pDevice);
		return ( it != d_array.end());
	}
};


class KADManger
{
public:
	/* ���id�͹��ָ��map*/
	typedef     std::map< DWORD,KAd* >       AD_MAP;
    typedef     AD_MAP::iterator             AD_MAP_IT; 


	//�ȴ�ƥ���������id        
    //typedef     std::deque<DWORD>            AD_WAIT_MATCH_ID;
	//typedef     std::deque<DWORD>::iterator  AD_WAIT_MATCH_ID_IT;
    typedef     std::set<DWORD>            AD_WAIT_MATCH_ID;
	typedef     std::set<DWORD>::iterator  AD_WAIT_MATCH_ID_IT;


public:	
	KADManger();
	~KADManger();			

public: 
	inline DWORD  GetXMLCRC()   { return m_xml.dwCRC; }

public:
    inline BOOL   IsLoadingTexture()   { return m_bLoadTexture; }

public:
	//���ر����浽�б���
	DWORD  Load(VOID*  pDevice);

	/*����3dģ��*/
	BOOL   Update3D_Mesh(AD_XML_RECORD& ad,VOID*  pDevice );

	/*����flash*/
	BOOL   UpdateFlash(AD_XML_RECORD& ad,VOID*  pDevice );

	/*����video*/
	BOOL   UpdateVideo(AD_XML_RECORD& ad,VOID*  pDevice );

	/*����acitve flash*/
	BOOL   UpdateActiveFlash(AD_XML_RECORD& ad,VOID*  pDevice );

	//�Ƿ���Ҫ����
    BOOL   IsNeedUpdate(AD_XML_RECORD& rs,BOOL bForceCmp = TRUE,int* lpChange=NULL); 
 
	VOID   Clear(VOID*);
	VOID   ClearAll();


    //DWORD  GetNextTextureID(VOID*  pDevice,BOOL  bRemove = TRUE)  ;
	//VOID   RemoveTextureID(DWORD  dwID);    


	//������Ч�Ĺ�ע�ȵ�������
	int   GetFocusDataToBuf(char* pData,DWORD&  dwLen);

	BOOL  GetInforFromAD(RESPONSE_FOCUS2* pOutFocus,tagFocus*  pFocus,_tagADType Type);

    AD_XML_Info   m_xml; 						//XML�ļ�
	AD_MAP        m_ads;						//����б�
	
    DWORD         m_dwReplaceADTextureCount;	//�ȴ��滻�Ķ༶����

    D3D_DEVICE_ARRAY<AD_WAIT_MATCH_ID>        m_dwNeedReplaceTexture ;

private:
	

	inline BOOL  TryLock () {	return ::TryEnterCriticalSection(&m_cr); }
	inline VOID  Lock  ()   { ::EnterCriticalSection(&m_cr);        }	
	inline VOID  Unlock()   { ::LeaveCriticalSection(&m_cr);        }

private:
	
	typedef std::set<VOID*>         DEV_COUNT_MAP;
	typedef DEV_COUNT_MAP::iterator  DEV_COUNT_MAP_IT;
    DEV_COUNT_MAP		m_dev_count;	//������ص��豸����map	    
	
	BOOL				m_bLoadTexture;	//�Ƿ����ڼ�������
	
	CRITICAL_SECTION    m_cr;			//��
};

/*****************************************************************************************************************8
 *  ģ�͵���Ļص�
 * dwID      :  ģ��id��
 * lpReserve : ����
****************************************************************************************************************/
extern VOID WINAPI CallBack3DFocus(DWORD  dwID,LPVOID  lpReserve );
/*****************************************************************************************************************8
 *  �������ü����Ļص�
 * pDevice   :  �豸ָ��
 * lnCount   :  ���ĵ����ü���
 * lpReserve :  ����
****************************************************************************************************************/
extern VOID WINAPI CallBackDeviceRefCount(VOID*  pDevice,long lnCount,LPVOID lpReserve);

/*****************************************************************************************************************8
 *  flash  ��video��activeflash����ʱ���Ļص�
 * dwID      :  ���id��
 * dwType    :  1:��ʼ���ţ�2����������
 * dwCurTime :  ��ǰʱ��
 * lpReserve : ����
****************************************************************************************************************/
extern VOID WINAPI CallBackPlayTimeFocus(DWORD  dwID,DWORD dwType,DWORD  dwCurTime,LPVOID  lpReserve );

#endif // !defined(AFX_AD_H__4377162B_D0C8_4F5B_B26B_A6EF2FE10401__INCLUDED_)
