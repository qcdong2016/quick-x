#include "LuaTable.h"
#include "tolua++.h"
#include "CCLuaEngine.h"

namespace cocos2d
{

LuaTable::LuaTable(lua_State* L)
{
	_luaState = L;
	lua_newtable(L);
}

LuaTable::LuaTable()
{
    _luaState = CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();
	lua_newtable(_luaState);
}

void LuaTable::setInt(int index, int value)
{
	lua_pushinteger(_luaState, index);
	lua_pushinteger(_luaState, value);
	lua_settable(_luaState, -3);
}
void LuaTable::setInt(const char* key, int value)
{
	lua_pushstring(_luaState, key);
	lua_pushinteger(_luaState, value);
	lua_settable(_luaState, -3);
}

void LuaTable::setFloat(int index, float value)
{
    lua_pushinteger(_luaState, index);
    lua_pushnumber(_luaState, value);
    lua_settable(_luaState, -3);
}
void LuaTable::setFloat(const char* key, float value)
{
    lua_pushstring(_luaState, key);
	lua_pushnumber(_luaState, value);
    lua_settable(_luaState, -3);
}

void LuaTable::setDouble(int index, double value)
{
    lua_pushinteger(_luaState, index);
	lua_pushnumber(_luaState, value);
    lua_settable(_luaState, -3);
}
void LuaTable::setDouble(const char* key, double value)
{
    lua_pushstring(_luaState, key);
	lua_pushnumber(_luaState, value);
    lua_settable(_luaState, -3);
}

void LuaTable::setBool(int index, bool value)
{
    lua_pushinteger(_luaState, index);
    lua_pushboolean(_luaState, value);
    lua_settable(_luaState, -3);
}
void LuaTable::setBool(const char* key, bool value)
{
    lua_pushstring(_luaState, key);
	lua_pushboolean(_luaState, value);
    lua_settable(_luaState, -3);
}

void LuaTable::setString(int index, const char* value)
{
    lua_pushinteger(_luaState, index);
	lua_pushstring(_luaState, value);
    lua_settable(_luaState, -3);
}
void LuaTable::setString(const char* key, const char* value)
{
    lua_pushstring(_luaState, key);
	lua_pushstring(_luaState, value);
    lua_settable(_luaState, -3);
}

void LuaTable::setUserType(int index, const char* type, void* ptr)
{
	lua_pushinteger(_luaState, index);
	tolua_pushusertype(_luaState, ptr, type);
	lua_settable(_luaState, -3);
}
void LuaTable::setUserType(const char* key, const char* type, void* ptr)
{
	lua_pushstring(_luaState, key);
	tolua_pushusertype(_luaState, ptr, type);
	lua_settable(_luaState, -3);
}
}
