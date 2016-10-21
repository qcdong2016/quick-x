#include "LuaEvent.h"
#include "LuaTable.h"

NS_CC_BEGIN

void EventHandlerLua::invoke(VariantMap& evt)
{
	_function->BeginCall();

	_function->PushInt(this->getEventType());
	// fix me
	LuaTable t;
	for (auto& it : evt) {
		const char* key = it.first.toString().c_str();
		switch (it.second.GetType())
		{
		case VAR_INT: t.setInt(key, it.second.GetInt()); break;
		case VAR_STRING: t.setString(key, it.second.GetString().c_str()); break;
		case VAR_FLOAT: t.setFloat(key, it.second.GetFloat()); break;
		case VAR_DOUBLE: t.setDouble(key, it.second.GetDouble()); break;
		case VAR_BOOL: t.setBool(key, it.second.GetBool()); break;
		case VAR_PTR:
		default:
			break;
		}
	}

	_function->PushLuaTable(&t);

	_function->EndCall();
}

/// Return a unique copy of the event handler.
EventHandler* EventHandlerLua::clone() const
{
	return new EventHandlerLua(_function);
}




NS_CC_END