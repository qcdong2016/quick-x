#include "LuaEvent.h"
#include "LuaTable.h"
#include "engine/CCEngineEvents.h"

NS_CC_BEGIN

void EventHandlerLua::invoke(EventDataMap& data)
{
	_function->BeginCall();

	if (this->getEventType() == UpdateEvent::Param::Name)
	{
		_function->PushFloat(data[UpdateEvent::Param::timeStep].GetFloat());
	}
	else 
	{
		//_function->PushInt(this->getEventType());
		LuaTable t;
		for (auto& it : data) {
			const char* key = it.first;
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
	}


	_function->EndCall();
}

/// Return a unique copy of the event handler.
EventHandler* EventHandlerLua::clone() const
{
	return new EventHandlerLua(_function);
}




NS_CC_END