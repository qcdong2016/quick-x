#include "LuaFunction.h"
#include "CCPlatformMacros.h"
#include "CCCommon.h"
#include <assert.h>
#include "tolua++.h"

namespace cocos2d
{

LuaFunction::LuaFunction(lua_State* L, int index) :
    _luaState(L),
    _numArguments(-1)
{
    assert(L);
    lua_pushvalue(L, index);
    _functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
}

LuaFunction::LuaFunction(lua_State* L, lua_CFunction func) :
    _luaState(L),
    _numArguments(-1)
{
    assert(L);
    lua_pushcfunction(L, func);
    _functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
}

LuaFunction::~LuaFunction()
{
    luaL_unref(_luaState, LUA_REGISTRYINDEX, _functionRef);
    _functionRef = LUA_NOREF;
}

bool LuaFunction::IsValid() const
{
    return _functionRef != LUA_REFNIL && _functionRef != LUA_NOREF;
}

bool LuaFunction::BeginCall()
{
    if (!IsValid())
        return false;

    lua_rawgeti(_luaState, LUA_REGISTRYINDEX, _functionRef);
    _numArguments = 0;

    return true;
}

bool LuaFunction::EndCall(int numReturns)
{
    assert(_numArguments >= 0);
    int numArguments = _numArguments;
    _numArguments = -1;

    if (lua_pcall(_luaState, numArguments, numReturns, 0) != 0)
    {
        const char* message = lua_tostring(_luaState, -1);
        CCLOG("Execute Lua function failed: %s", message);
        lua_pop(_luaState, 1);
        return false;
    }

    return true;
}

void LuaFunction::PushInt(int value)
{
    assert(_numArguments >= 0);
    ++_numArguments;
    lua_pushinteger(_luaState, value);
}

void LuaFunction::PushBool(bool value)
{
    assert(_numArguments >= 0);
    ++_numArguments;
    lua_pushboolean(_luaState, value);
}

void LuaFunction::PushFloat(float value)
{
    assert(_numArguments >= 0);
    ++_numArguments;
    lua_pushnumber(_luaState, value);
}

void LuaFunction::PushDouble(double value)
{
    assert(_numArguments >= 0);
    ++_numArguments;
    lua_pushnumber(_luaState, value);
}

void LuaFunction::PushString(const char* value)
{
    assert(_numArguments >= 0);
    ++_numArguments;
    lua_pushstring(_luaState, value);
}

void LuaFunction::PushUserType(void* userType, const char* typeName)
{
    assert(_numArguments >= 0);
    ++_numArguments;
    tolua_pushusertype(_luaState, userType, typeName);
}

void LuaFunction::PushLuaTable(const LuaTable* t)
{
    assert(_numArguments >= 0);
    ++_numArguments;
	// doing nothing.
}

}
