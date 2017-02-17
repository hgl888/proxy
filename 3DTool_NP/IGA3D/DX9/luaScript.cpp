#include "stdafx.h"

CLuaScript* GetScript()
{
	static CLuaScript lua;
	return &lua;
}


//---------------------------------------------
CLuaScript::CLuaScript()
{
	L = lua_open();
	lua_cpcall(L, luaopen_base, 0); 
	lua_cpcall(L, luaopen_io, 0);   
	lua_cpcall(L, luaopen_math, 0); 
	lua_cpcall(L, luaopen_string, 0);
}

CLuaScript::~CLuaScript()
{
	lua_close(L);
}

int CLuaScript::LoadLuaFile( char* fileName )
{
	if( L != NULL )
	{
		lua_close(L);
		L = lua_open();
		lua_cpcall(L, luaopen_base, 0); 
		lua_cpcall(L, luaopen_io, 0);   
		lua_cpcall(L, luaopen_math, 0); 
		lua_cpcall(L, luaopen_string, 0);
	}

	int s = luaL_loadfile( L, fileName );
    if ( s==0 )
	{
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
		if( s == 0 )
		{
			lua_getglobal(L,"DrawPos");
			if( lua_isnumber(L,1) )
			{
				int DrawPos = (int)lua_tonumber(L,1);
				return DrawPos;
			}
		}
		else if ( s == LUA_ERRERR )
		{
			OutputDebugString("CLuaScript: ���д�������ʱ�����Ĵ���");
		}
		else if( s == LUA_ERRMEM )
		{
			OutputDebugString("CLuaScript: �ڴ�������");		
		}
		else if( s == LUA_ERRRUN )
		{
			OutputDebugString("CLuaScript: ����ʱ����");
		}
	}
	OutputDebugString("CLuaScript: �ű��ļ�����ʧ�ܣ�");
	return Per_Present;
}

void CLuaScript::RegFunction( char* funName, lua_CFunction pfun )
{
	lua_register( L, funName, pfun );
}

void CLuaScript::RunScript()
{
	//��ȡlua����
	lua_getglobal(L,"Run");

	//��lua�������ݲ���

    //lua_pushnumber(L, 0 );


	//ִ��lua����
	lua_pcall( L,	//lua�����
			   0,   //��ջ�Ĳ�����Ŀ
			   0,   //����ֵ����Ŀ
			   0 ); //������Ϣ

	//char text[256];

	//for( int i = 1; ; i++ )
	//{
	//	//����NULLֵ�ͽ���
	//	if( lua_isnil(L,-i) )
	//	{
	//		break;
	//	}

	//	//�����ַ���
	//	if( lua_isstring(L, -i) )
	//	{
	//		sprintf(text, "%d : %s\n", i, lua_tostring(L, -i) );
	//		continue;
	//	}

	//	//��������
	//	if( lua_isnumber(L, -i) )
	//	{
	//		sprintf(text, "%d : %s\n", i, lua_tonumber(L, -i) );
	//		continue;
	//	}
	//	OutputDebugString(text);
	//}

	////���������ջ
	//lua_pop(L, 1);

}