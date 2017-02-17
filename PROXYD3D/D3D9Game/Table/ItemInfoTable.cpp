 
#include "ItemInfoTable.h"
#include "../GameDefine.h"
#include "../itemManager.h"

using namespace UI_TEXTURE_ID;


ITEM_INFO g_ItemInfoTable[MAX_ITEM] = 
{
	//{10001,,,},
	//{10002,,,},
	//...
	{0,L"С��HPҩˮ",UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_0,ITEM_MESH_ID_RED,ITEM_HPMP,40,L"�ɻָ�����HP",3},
	{1,L"С��MPҩˮ",UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_1,ITEM_MESH_ID_BLUE,ITEM_HPMP,35,L"�ɻָ�����MP",4},
	{2,L"�ֲ�",		 UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_2,ITEM_MESH_ID_BAG,ITEM_MATERAIL,40,L"����������",0},
	{3,L"����",		 UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_3,ITEM_MESH_ID_BAG,ITEM_MATERAIL,20,L"���·��õ���",0},
	{4,L"��Ƥ",		 UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_4,ITEM_MESH_ID_BAG,ITEM_MATERAIL,40,L"������Ƥ��",0},
	{5,L"���õ��·�",UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_5,ITEM_MESH_ID_BAG,ITEM_EQUIP,50,L"���õ��·�,����+3",0},
	{6,L"���õ�Ƥ��",UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_6,ITEM_MESH_ID_BAG,ITEM_EQUIP,50,L"���õ�Ƥ��,����+2",0},
};


ITEM_INFO * GetItemInfo( DWORD dwId )
{
	for( int i = 0; i < MAX_ITEM; ++i )
	{
		if( g_ItemInfoTable[i].dwId == dwId )
			return &g_ItemInfoTable[i];
	}
	return NULL;
}

ITEM_HPMP_INFO g_HPMPInfoTable[MAX_ITEM] = 
{
	{10001,50,0},
	{10002,0,50},
};

ITEM_HPMP_INFO * GetHPMPInfo( DWORD dwId )
{
	for( int i = 0; i < MAX_ITEM; ++i )
	{
		if( g_HPMPInfoTable[i].dwId == dwId )
			return &g_HPMPInfoTable[i];
	}
	return NULL;
}