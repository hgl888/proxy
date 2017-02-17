/************************************************************************
*	Copyright(c) 2008-2009 ���ѲƸ����������������������޹�˾
*	All rights reserved
*	FileName : IGA3D_UI.cpp
*	����������IGA3D, modify by hgl
*	��ǰ�汾��2, 0, 8, 1226
*	����/�޸��ߣ�hgl
*	������ڣ�2008-12-26
*	ȡ���汾: 2, 0, 8, 1222
*	�޸��ˣ�
*	������ڣ�                                                      
/************************************************************************/

#include "stdafx.h"
#include "DefineStruct.h"
#include "IGA3D9.h"
#include "IGA3DModule.h"
#include "IGA3D_UI.h"

CIGA3DMessageManager	g_CIGA3DMessageManager;

//-----------------------------------------------------------------
//��Ϣ������
CIGA3DMessageManager::CIGA3DMessageManager()
{
	m_xCursor = 0;
	m_yCursor = 0;

}

CIGA3DMessageManager::~CIGA3DMessageManager()
{

}

HRESULT CIGA3DMessageManager::Init()
{

	return S_OK;
}

bool CIGA3DMessageManager::GetIGA3DMessage( STIGA3DMSG* msg )
{
	
	if( m_queueMsg.empty() )
	{
		return false;	//û����Ϣ
	}
	*msg = m_queueMsg.front();

	return true;

	//if( GetKeyState(VK_RBUTTON) < 0 )	//����Ҽ�����״̬
	//{
	//	msg->type = 0;
	//	msg->key  = IGA3D_MSG_RBUTTONDOWN;
	//	msg->x    = 0;
	//	msg->y    = 0;
	//}
	//else
	//{
	//	return false;
	//}

	//return true;
}

void CIGA3DMessageManager::SendIGA3DMessage( STIGA3DMSG msg )
{
	m_queueMsg.push(msg);
}

void CIGA3DMessageManager::RemoveMsg()
{
	m_queueMsg.pop();
}

void CIGA3DMessageManager::Destory()
{


}

void CIGA3DMessageManager::UpdataMessage()
{
	HWND hWnd = GetActiveWindow();
	POINT pointCursor;
	GetCursorPos(&pointCursor);
	ScreenToClient(hWnd, &pointCursor);
	
	m_xCursor = pointCursor.x;
	m_yCursor = pointCursor.y;


	static bool bstaticflg = false;
	static bool bstaticflg1 = false;
	if( GetKeyState(VK_LBUTTON) < 0 )	//����Ҽ�����״̬
	{
		bstaticflg1 = true;
		if( !bstaticflg )
		{
			bstaticflg = true;
			
			//����һ����갴��״̬
			STIGA3DMSG msg;
			msg.iKey = IGA3D_MSG_LBUTTONDOWN;
			msg.ixPosition   = m_xCursor;
			msg.iyPosition   = m_yCursor;
			SendIGA3DMessage( msg );
		}
	}
	else	//����Ҽ�̧��״̬
	{
		bstaticflg = false;
		if( bstaticflg1 )
		{
			bstaticflg1 = false;
			
			//����һ�����̧��״̬
			STIGA3DMSG msg;
			msg.iKey = IGA3D_MSG_LBUTTONUP;
			msg.ixPosition   = m_xCursor;
			msg.iyPosition   = m_yCursor;
			SendIGA3DMessage( msg );

		}
	}
}
