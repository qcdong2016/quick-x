#include "engine/CCObject.h"
#include "base/Ptr.h"
#include "LuaFunction.h"

NS_CC_BEGIN

/// Template implementation of the event handler invoke helper (stores a function pointer of specific class.)
class EventHandlerLua : public EventHandler
{
public:
	EventHandlerLua(int funcIdx) :
		_function(new LuaFunction(funcIdx))
	{
		assert(_function);
	}
	EventHandlerLua(SharedPtr<LuaFunction> func) :
		_function(func)
	{
	}

	virtual void invoke(IEventData& evt);
	virtual EventHandler* clone() const;
private:
	SharedPtr<LuaFunction> _function;
};

NS_CC_END