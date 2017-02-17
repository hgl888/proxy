/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : IGA3D_UI.h
*	����������IGA3D, modify by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#pragma  once 

//#include "d3d9.h"
//#include "d3dx9.h"

#include <queue>
using namespace std;

#include <functional>
#include "DefineConst.h"


class CIGA3DMessageManager
{
public:
	CIGA3DMessageManager();
	~CIGA3DMessageManager();

public:
	HRESULT Init();
	void    Destory();
	void	UpdataMessage();
	bool    GetIGA3DMessage( STIGA3DMSG* msg );	//����Ϣ������õ�һ����Ϣ
	void	SendIGA3DMessage( STIGA3DMSG msg );	//����Ϣ���͵���Ϣ������
	void	RemoveMsg();//�Ƴ�������Ϣ
	void	SetMsgManagerState( DWORD state );	//���ù�����״̬

private:

	queue<STIGA3DMSG>	m_queueMsg;

public:
	int		m_xCursor;
	int		m_yCursor;

};

extern CIGA3DMessageManager g_CIGA3DMessageManager;





