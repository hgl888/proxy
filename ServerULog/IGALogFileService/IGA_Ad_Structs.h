#ifndef __IGA_AD_STRUCTS_H__
#define	__IGA_AD_STRUCTS_H__


#include "consts.h"

#pragma pack(push, 1)

/************************************************************************/
/* ý����Ϣ                                                             */
/************************************************************************/
typedef struct tagMediaID
{
	union
	{
		DWORD	dwMediaID;
		struct 
		{
			WORD	w[2];
		};
		struct 
		{
			BYTE	b[4];
		};
		struct 
		{
			WORD	wReserved;
			BYTE	byScene;
			BYTE	bySample;
		};
	};
}MEDIAID, *LPMEDIAID;

/************************************************************************/
/* ����                                                               */
/************************************************************************/

typedef struct tagAdId
{
	DWORD	dwID;									// PK, INT, NOT NULL
	DWORD	dwVersion;
	CHAR	szAdPosition[MAX_ADPOSITION];			// NVARCHAR
	CHAR	szAdMedium[MAX_ADMEDIUM];
	CHAR	szAdIntroduction[MAX_ADINTRODUCTION];
	double	dbPrice;								// Decimal(18,2)
	INT		nCurrencyType;							//	
	INT		nAdPayMode;								
	INT		bDeleted;
}ADID,*LPADID;


/************************************************************************/
/* ͨ�����紫����ͻ��˵Ĺ��λ��Ϣ                                     */
/************************************************************************/
typedef struct tagAdInfoInComm
{
	DWORD	dwID;							//	���ID��
	BYTE	byEnabled;						//	����Ƿ�رջ��Ǵ�
	BYTE	byType;							//	ý������	MEDIUMTYPE_DDS
	union
	{
		struct  {
			BYTE	byOrigFileNameLength;			//	���������ļ���������
			BYTE	byAdFileNameLength;				//	���������ļ����Ƴ���
			DWORD	dwOriginalLevel;				//	Դ�ļ��Ĳ��
			DWORD	dwAdLevel;						//	����ļ��Ĳ��
			DWORD	dwOriginalFileVersion;			//	Դ�ļ��İ汾
			DWORD	dwAdFileVersion;				//	����ļ��İ汾�š�
		};
		struct  {
			BYTE	byFileNameLength0;
			BYTE	byFileNameLength1;
			DWORD	dwLevel0;
			DWORD	dwLevel1;
			DWORD	dwVersion0;
			DWORD	dwVersion1;
		};
	};
	//	�����������ļ����ƣ�
	//CHAR	szOriginalFileName[byOrigFileNameLength];
	//CHAR	szAdFileName[byAdFileNameLength];
	
}ADINFOINCOMM, *LPADINFOINCOMM;

/************************************************************************/
/* �ͻ��˱����ڱ��صĹ��λ��Ϣ�ṹ                                     */
/************************************************************************/
typedef struct tagClientAdInfo
{
	DWORD	dwID;							//	���ID��
	DWORD	dwType;							//	������ͻ�ý�����͵�
	union
	{
		struct  {
			DWORD	dwOriginalLevel;				//	Դ�ļ��Ĳ��
			DWORD	dwAdLevel;						//	����ļ��Ĳ��
			DWORD	dwOriginalFileVersion;			//	Դ�ļ��İ汾
			DWORD	dwAdFileVersion;				//	���汾��
			
			CHAR	szOriginalFileName[_MAX_PATH];	//	Դ�ļ�����
			CHAR	szAdFileName[_MAX_PATH];		//	�����Դ�ļ�����
		};
		
		struct  {
			DWORD	dwLevel0;						//	Դ�ļ��Ĳ��
			DWORD	dwLevel1;						//	����ļ��Ĳ��
			DWORD	dwVersion0;						//	Դ�ļ��İ汾
			DWORD	dwVersion1;						//	���汾��
			CHAR	szFileName0[_MAX_PATH];			//	Դ�ļ�����
			CHAR	szFileName1[_MAX_PATH];			//	�����Դ�ļ�����
		};
	};

	DWORD	dwReserved[4];

}CLIENTADINFO, *LPCLIENTADINFO;


/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef struct tagGameID
{
	DWORD	dwGameID;
	CHAR	szGameName[MAX_GAMENAME];
	DWORD	dwVersion;
	CHAR	szVersion[MAX_GAMEVER];
	CHAR	szPublisher[MAX_PUBLISHER];
	CHAR	szDeveloper[MAX_DEVELOPER];
	DWORD	dwGraphicsPlatform;				//GP_DX9;	GP_GX8

}GAMEID, *LPGAMEID;

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef struct tagHardwareID
{
	CHAR	szMac[MAX_MAC];
	CHAR	szCpu[MAX_CPU];
	CHAR	szDisplay[MAX_DISPLAY];
	DWORD	dwOperatingSystem;
	DWORD	dwMemorySize;
	DWORD	dwGraphicsPlatform;				//GP_DX9;	GP_GX8
}HARDWAREID, *LPHARDWAREID;



/************************************************************************/
/* ����ע������                                                       */
/************************************************************************/
typedef struct tagFocus
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
	
}FOCUS, *LPFOCUS;

/************************************************************************/
/* �ͻ������Ӽ�¼��Ϣ                                                   */
/************************************************************************/
typedef struct tagClientPlayInfo
{
	DWORD	dwPlayerID;
	DWORD	dwGameID;
	DWORD	dwLocationID;
	DWORD	dwHighVersion;
	DWORD	dwLowVersion;
	DWORD	dwTime;
	WORD	wConnectTimes;
	WORD	wConnectedTimes;
	DWORD	dwPlayCount;
	struct  {
		DWORD	dwPlayTick;
		DWORD	dwPlayTime;
	}	pi[0];
}CLIENTPLAYINFO, *LPCLIENTPLAYINFO;

typedef	struct tagPlayInfo
{
	DWORD	dwPlayTime;
	DWORD	dwCloseTime;
}PLAYINFO, *LPPLAYINFO;

#pragma pack(pop)



#endif	///__IGA_AD_STRUCTS_H__