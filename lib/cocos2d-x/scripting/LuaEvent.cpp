#include "LuaEvent.h"
#include "LuaTable.h"
#include "engine/CCEngineEvents.h"

NS_CC_BEGIN

void EventHandlerLua::invoke(EventData& data)
{
	_function->BeginCall();

	if (this->getEventType() == UpdateEvent::_ID)
	{
		_function->PushFloat(data[UpdateEvent::timeStep].GetFloat());
	}
	else 
	{
		//_function->PushInt(this->getEventType());
		LuaTable t;

		for (int i = 0; i < data.getParamCount(); i++) {
			const char* key = data.getParamName(i);
			Variant& v = data[i];
			switch (v.GetType())
			{
			case VAR_INT: t.setInt(key, v.GetInt()); break;
			case VAR_STRING: t.setString(key, v.GetString().c_str()); break;
			case VAR_FLOAT: t.setFloat(key, v.GetFloat()); break;
			case VAR_DOUBLE: t.setDouble(key, v.GetDouble()); break;
			case VAR_BOOL: t.setBool(key, v.GetBool()); break;
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
