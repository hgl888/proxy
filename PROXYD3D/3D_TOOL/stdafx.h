// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0410 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0400	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS


#pragma warning( disable : 4996 )

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdhtml.h>

#include "z_FileIni.h"
#include "z_Memory.h"
#define _IGA_DATA _T("F43436CA-F337-410a-8BDB-F470040A81C2")
struct _SIGASTATEDATA
{
	float x;
	float y;
	float z;

	bool bIsReload;

	UINT nPickObjID;
	
	BOOL bIsShowPickMesh;

	_SIGASTATEDATA()
	{
		x = 0;
		y = 0;
		z = 0;
		bIsReload = false;
		nPickObjID = -1;
		bIsShowPickMesh = false;
	}

};

typedef CShareMemory<_SIGASTATEDATA,1> IGA_DATA_MEM;



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
	IGA_PAOPAOKADINGCHE,	//���ܿ�����
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
	IGA_XINYANG,			//����--����ս��	
	IGA_ZHANHUO,			//ս��	
	IGA_JIETOULANQIU,		//��ͷ����	
	IGA_TIANTANG2,			//����2
	IGA_SUN,				//�漣����
	IGA_TIANZUN,			//����OnLine
	IGA_WLOL,				//����Ⱥ���� 
	IGA_GV,					//�󺽺�ʱ��Online
	IGA_RELIPAIQIU,			//��������	
	IGA_CHAOJIWUZHE,			//��������

	IGA_XIANJIAN=41,		//�ɽ�
	IGA_YINGTAOCHENG = 42, // ӣ�ҳ�   
	IGA_FENGYICHUANSHUO = 43,	//��ӡ��˵
	IGA_SHUSHAN = 44,			//��ɽ

	IGA_WUJIEQU_MLXY=155,	//�����(��������)
	IGA_WUJIEQU_XJP,		//�����(�¼���)
	IGA_WUJIEQU_FLB = 158,	//�����(���ɱ�)

	IGA_TEST = 1000,		//������Ϸʹ��ID
};

