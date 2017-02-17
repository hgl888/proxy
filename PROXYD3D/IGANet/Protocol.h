// Protocol.h: interface for the Protocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOL_H__ABA4BE9F_3FB2_42DD_A492_4ADCC4AD3828__INCLUDED_)
#define AFX_PROTOCOL_H__ABA4BE9F_3FB2_42DD_A492_4ADCC4AD3828__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/************************************************************************/
/* ͨ�ű�����                                                           */
/************************************************************************/
typedef enum ePACKETTYPE
{
	PACKETTYPE_NONE		= 0x0,	//
	PACKETTYPE_CONNECT,			// Connect packet
	PACKETTYPE_RECONNECT,		// Reconnect packet
	
	PACKETTYPE_STREAM,			// Stream packet

	PACKETTYPE_MESSAGE,			//	��Ϣ��
	PACKETTYPE_BULLETIN,		//	������Ϣ��
	PACKETTYPE_TALKS,			//	������Ϣ
	PACKETTYPE_MAX		= 0xFF,
}PACKETTYPE;

/************************************************************************/
/* �Ӱ�����                                                             */
/************************************************************************/
typedef enum eSUBPACKETTYPE
{
	SUBPACKETTYPE_FILE,		
	SUBPACKETTYPE_DATA,
}SUBPACKETTYPE;

/************************************************************************/
/* ͨ�Ű�ѹ�����������                                                 */
/************************************************************************/
typedef enum ePACKETFLAGS
{
	PACKETFLAGS_NONE,				// �Ȳ�ѹ����Ҳ������
	PACKETFLAGS_COMPRESSEDONLY,		// ����ѹ��
	PACKETFLAGS_ENCRYPTEDONLY,		// ��������
	PACKETFLAGS_COMPRESSENCRYPTED,	// ��ѹ�������
	PACKETFLAGS_ENCRYPTCOMPRESSED,	// �ȼ��ܺ�ѹ��
	PACKETFLAGS_MD5,				// MD5�㷨����
}PACKETFLAGS;

/************************************************************************/
/* �����Ӧ������                                                       */
/************************************************************************/
typedef enum eREQUESTTYPE
{
	REQUESTTYPE_HARDWAREINFO,		//	Ӳ����Ϣ 
	REQUESTTYPE_GAMEINFO,			//	��Ϸý����Ϣ
	REQUESTTYPE_ADINFO,				//	���λ��Ϣ
	REQUESTTYPE_SDKUPDATEINFO,		//	SDK������Ϣ
	REQUESTTYPE_FILEINFO,			//	�ļ�������Ϣ
	REQUESTTYPE_FOCUS,				//	����ע������
	REQUESTTYPE_BULLETIN,			//	������Ϣ
	REQUESTTYPE_MESSAGE,			//	ͨ����Ϣ
	REQUESTTYPE_CHAT,				//	������Ϣ
	REQUESTTYPE_AD,					//	
	REQUESTTYPE_PLAYINFO,			//
}REQUESTTYPE;

//////////////////////////////////////////////////////////////////////////
//	ͨ�Ű���Դ
typedef enum ePacketFrom
{
	PACKETFROM_IIS,	//	������IIS����
	PACKETFROM_PLAYER,	//	��������ҵ�����
	PACKETFROM_ROUTER,	//	������Router������
	PACKETFROM_FRONTER,	//	������Fronter������
	PACKETFROM_PROXY,	//	�������ڲ����ɴ���������������Ӧ��
}PACKETFROM;


#pragma pack(push, 1)

#define SERVER_TIME


/// General packet header item
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

/************************************************************************/
/* �ӱ�ͷ                                                               */
/************************************************************************/
typedef struct tagSubPacketHeader 
{
	BYTE	bySubPacketType;		//	�Ӱ�����
	union
	{
		BYTE	byReserved;
		BYTE	byResponseState;
	};
	WORD	wHeaderLength;			//	�Ӱ���ͷ����
}SUBPACKETHEADER, *LPSUBPACKETHEADER;

/************************************************************************/
/*	����ͷ��Ӧ���ͷ                                                    */
/************************************************************************/
typedef struct tagRequestHeader 
{
	BYTE	byRequestType;		//	����ͷ����
	BYTE	byFlags;			//	�Ӱ���ͷ����
	WORD	wRequestHeader;		//	����ͷ�����ȣ������Ӱ�ͷ����
}REQUESTHEADER, *LPREQUESTHEADER;

#endif // !defined(AFX_PROTOCOL_H__ABA4BE9F_3FB2_42DD_A492_4ADCC4AD3828__INCLUDED_)
