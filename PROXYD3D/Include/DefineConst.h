/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : defineCon.h
*	����ʹ�ó������壬�����ַ���������ö�ٳ���, add by hgl
*	�޸�SHOWOUTPUTDEBUG�궨�壬���ڵ��԰汾����ʾ��Ϸִ�е�λ��
*	��ǰ�汾��2, 0, 9, 0304
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1226
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#pragma  once

#ifndef _DEFINECONST_H_
#define _DEFINECONST_H_


#include "stdio.h"

#define ADINFO_EMMOA_3D (TEXT("InterAdtive\\EMMOA\\3D\\"))
#define	ADINFO_EMMOA_RES_FLASH	"InterAdtive\\EMMOA\\res\\flash\\" 

#define _IGA_DATA (TEXT("F43436CA-F337-410a-8BDB-F470040A81C2"))

const float EPSINON = 0.000001f;  //�������Ƚ�ֵ 
#define FLOAT_EQAUL_ZERO(fdate)  (((fdate) >= - EPSINON) && ((fdate) <= EPSINON))

#define D3DRM_XTEMPLATE_BYTES   3278
#define RELEASE_COM(ixPoint)	{ if(NULL != (ixPoint)) { (ixPoint)->Release(); (ixPoint) = NULL; } }

#ifndef SAFE_DELETE
#define SAFE_DELETE( T ) {if( T ) { delete T; T = NULL; }}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( T ) {if( T ) { delete[] T; T = NULL; }}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( T ) if( T ) { T->Release(); T = NULL; }
#endif


#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = x; if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = x; }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
#endif
#endif

//////////////////////////////////////////////////////////////////////////

// Put up an assertion failure message box.
inline void InlineShowOutPutDebug(LPCSTR file, int line,  PCSTR expr) 
{
	char szOutputString[ sizeof(char) *MAX_PATH];
	memset(szOutputString, 0, sizeof(char)  * MAX_PATH );
	SYSTEMTIME st;
	GetLocalTime( &st );
	char szDate[100];
	sprintf( szDate, "%04d-%02d-%02d %02d:%02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	sprintf(szOutputString, "%s,  File %s, line %d ,Date:%s ",expr, file, line, szDate );
	OutputDebugStringA(szOutputString);
}


//��debug����ʾ�����Ϣ��,ʹ��ǰҪ�ȶ������ַ���������
# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	define	SHOWOUTPUTDEBUG(szOutputString) InlineShowOutPutDebug(__FILE__, __LINE__, #szOutputString)
# else
#	define	SHOWOUTPUTDEBUG(szOutputString)  InlineShowOutPutDebug(__FILE__, __LINE__, #szOutputString)
# endif
/*
�����ǹ��߻��ǿͻ���
0:��ʾ�ǿͻ��ˣ�������������Ѱ�ҡ����¼��ص��ֶ����ܣ�ͬʱҲ��������κ���Ϣ��
1:��ʾ�ǹ���ģʽ�����Խ�������Ѱ�ҡ�����3Dģ�ͣ����������ص���Ϣ��
*/
#define IS_3D_TOOL	0


# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	define	PROTECT(s)		{	static	TCHAR *__FUNC_NAME__	= TEXT(#s);
#	define	UNPROTECT()		}
# else
#	define	PROTECT(s)		{			\
							try			\
							{			\
								static	TCHAR *__FUNC_NAME__= TEXT(#s);	
								
#	define	UNPROTECT()		}			\
							catch(...)	\
							{			\
							}			\
						}
# endif


enum 
{
	b_ESC, b_F1, b_F2, b_F3, b_F4, b_F5, b_F6, b_F7, b_F8, b_F9, b_F10, b_F11, b_F12, //���ܼ�
	b_L, b_W, b_H, b_S, b_R, b_T, b_M, b_A, b_N,//��ĸ��
	b_UP, b_DOWN, b_LEFT, b_RIGHT, //�����
	b_LBUTTON, b_RBUTTON, //������
	b_CTRL, b_SHIFT, //CTRL��SHIFT��
	b_ADD, b_SUB, //��+������-����
	b_DELETE, //DELETE��
	b_MAX, b_MIN, //��,������.����<������>����
};

enum GAME_STATE
{
	STATE_START,	//��ʼ
	STATE_IN_SCENE, //�����˳���
	STATE_AFTER_DLG,	//���ѡ����һ��������ڣ�����IGA�Ի���,��ʾ��ҿ���IGA�˵��
	STATE_CHOICE_IGA,	//���ѡ����IGA�
	STATE_IGA_START,	//IGA���ʼ
	STATE_IGA_COMPLETE, //���IGA�
	STATE_IGA_END 		//�˳�IGA������ܵ��������ɺ��˳�����;�˳���һ��ʼ���˳�
};

enum IGA_STATE
{
	IGA_START,	//IGA����ʼ
	IGA_END,	//IGA�������
	IGA_1,	//��ɵ�һ������
	IGA_2,	//
	IGA_3,
	IGA_OK,	//���ȫ������
	IGA_DESIGN //���ģʽ
};

enum RenderPos
{
	Per_SetTransform,	//���þ���ǰ
	Post_SetTransform,	//���þ����
	Per_Present,		//��תǰ
	Post_Present		//��ת��
};

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

	IGA_XIANJIAN=41,		//�ɽ�
	IGA_YINGTAOCHENG = 42, // ӣ�ҳ�   
	IGA_FENGYICHUANSHUO = 43,	//��ӡ��˵
	IGA_SHUSHAN = 44,			//��ɽ
	IGA_ZHENSANGUO = 45,		//������
	IGA_MOLINGONLINE= 46,		//ħ��Online
	IGA_QICHENG = 47,			//����

	IGA_WUJIEQU_MLXY=155,	//�����(��������)
	IGA_WUJIEQU_XJP,		//�����(�¼���)
	IGA_WUJIEQU_FLB = 158,	//�����(���ɱ�)

	IGA_TEST = 1000,		//������Ϸʹ��ID
};


#endif

