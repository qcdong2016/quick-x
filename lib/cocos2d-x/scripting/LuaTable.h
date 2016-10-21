#pragma once


struct lua_State;

namespace cocos2d
{

class LuaTable
{
public:
	LuaTable();
	LuaTable(lua_State* L);

	void setInt(int index, int value);
	void setInt(const char* key, int value);

	void setFloat(int index, float value);
	void setFloat(const char* key, float value);

	void setDouble(int index, double value);
	void setDouble(const char* key, double value);

	void setBool(int index, bool value);
	void setBool(const char* key, bool value);

	void setString(int index, const char* value);
	void setString(const char* key, const char* value);

	void setUserType(int index, const char* type, void* ptr);
	void setUserType(const char* key, const char* type, void* ptr);

private:
	lua_State* _luaState;
};
}