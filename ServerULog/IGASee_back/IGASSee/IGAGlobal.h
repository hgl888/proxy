#pragma once

//class IGAGlobal
//{
//public:
//	IGAGlobal(void);
//	~IGAGlobal(void);
//};

#	if	defined(ZLIB) || defined(USING_ZLIB)
#	pragma message("*** Auto linking ZLIB 0.9...")
#	include "./../lib/include/zlib.h"
#		if		defined (DEBUG) || defined(_DEBUG) || defined(DBG)
#			pragma comment(lib, "./../lib/Debug/zlib.lib")
#		else
#			pragma comment(lib, "./../lib/release/zlib.lib")
#		endif
#	endif


#define PLAYINFO_EXT	_T(".PlayInfo")
#define FOCUSINFO_EXT   _T(".FocusInfo")
#define INI_EXT			_T(".ini")
#define LOG_EXT         _T(".LOG")

#define	IDS_SECTION_SDK				_T("SDK")
#define	IDS_SECTION_HARDWAREINFO	_T("HARDWAREINFO")

#define MAX_MAC			18
#define MAX_PATH_KEY    5


#define ABRK	_T('/')
#define BRK		_T('\\')

// ʹ�ý����ֽ�
#pragma pack(push, 1)

typedef	struct tagClientPlayInfo
{
	DWORD	dwGameID;
	DWORD	dwPlayTime;
	DWORD	dwCloseTime;
	DWORD	dwChnID;
	DWORD	dwAreadID;
	TCHAR	szMacID[20];
	TCHAR	szAddress[20];
}CLIENTPLAYINFO;

typedef struct tagPacketHeader 
{
	DWORD	dwLength;		// General packet length including this packet header
#ifdef SERVER_TIME
	DWORD	dwSrvTime;		//	the tick of the services
#endif
	BYTE	byType;			// Packet type, PACKETTYPE_XXXX
	BYTE	byFlags;		// Packet flags. such as crypt or compressed
	BYTE	byCount;		// Sub packet count
	BYTE	byFrom;			// Reserved;
}PACKETHEADER, *LPPACKETHEADER;


typedef struct tagRequestHeader 
{
	BYTE	byRequestType;		//	����ͷ����
	BYTE	byFlags;			//	�Ӱ���ͷ����
	WORD	wRequestHeader;		//	����ͷ�����ȣ������Ӱ�ͷ����
}REQUESTHEADER, *LPREQUESTHEADER;

template<typename HEADINFO>
struct FOCUSREQUEST_T : public HEADINFO
{
	DWORD	dwGameID;
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
	DWORD	dwAdCount;
	CHAR	szMacID[MAX_MAC];
};


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

#pragma pack(pop)


typedef struct tagListData
{
	WPARAM wParam;
	LPARAM lParam;
}LISTDATA, *LPLISTDATA;


static	const CHAR *g_szGP[]	=
{
	_T("NONE"),
		_T("DirectX 9.0"),
		_T("DirectX 8.0"),
		_T("DirectDraw"),
		_T("OpenGL"),
		_T("GDI"),
		_T("Unknown"),
};


// ��������
CString AUTOAPPENDBRK(CString &str);

CString GetConfigFileName();
BOOL CreateConfigFile();

LPCTSTR GetToken(LPCTSTR psz, int n);
