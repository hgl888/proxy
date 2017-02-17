extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
	#pragma comment(lib,"lua.lib")
}

class CLuaScript
{
public:
	CLuaScript();
	~CLuaScript();

public:
	int LoadLuaFile(char* fileName );

	void RunScript();

	inline operator lua_State*() 
	{
		return L;
	}

//����ĺ����б�����֪ͨ�ű�
public:
	void RegFunction( char* funName, lua_CFunction pfun );






private:
	lua_State *L;

};

CLuaScript* GetScript();