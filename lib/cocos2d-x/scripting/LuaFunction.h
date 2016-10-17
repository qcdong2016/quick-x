

#pragma once
#include "cocos/RefCounted.h"


struct lua_State;
typedef int (*lua_CFunction) (lua_State *L);

namespace cocos2d
{
	class LuaTable;

/// C++ representation of Lua function object.
class LuaFunction : public RefCounted
{
public:
    /// Construct from a Lua function object at the specified stack index.
    LuaFunction(lua_State* L, int index);
    /// Construct from a C function.
    LuaFunction(lua_State* L, lua_CFunction func);
    /// Destruct.
    ~LuaFunction();

    /// Check that function is valid.
    bool IsValid() const;
    /// Begin function call. When a script object is given then pass it as self argument (first parameter) to the function call.
    bool BeginCall();
    /// End call and actually execute the function. The return values, if any, are still left in the stack when this call returns.
    bool EndCall(int numReturns = 0);
    /// Push int to stack.
    void PushInt(int value);
    /// Push bool to stack.
    void PushBool(bool value);
    /// Push float to stack.
    void PushFloat(float value);
    /// Push double to stack.
    void PushDouble(double value);
    /// Push string to stack.
    void PushString(const char* value);
    /// Push user type to stack.
    void PushUserType(void* userType, const char* typeName);

    /// Push Lua table to stack. When the specified table is not found then a nil is pushed instead.
    void PushLuaTable(const LuaTable* t);

    /// Return function ref.
    int GetFunctionRef() const { return _functionRef; }

	explicit operator bool() const
	{
		return IsValid();
	}

private:
    /// Lua state.
    lua_State* _luaState;
    /// Number of arguments being pushed so far. For internal use only.
    int _numArguments;
    /// Lua reference to function object.
    int _functionRef;
};

}
