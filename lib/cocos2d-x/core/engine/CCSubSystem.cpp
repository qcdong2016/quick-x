#include "CCSubSystem.h"
#include "CCDirector.h"

NS_CC_BEGIN

SubSystem* SubSystem::get(ID sysType)
{
	return CCDirector::sharedDirector()->getSubSystem(sysType);
}

SubSystem* SubSystem::add(ID sysType)
{
	return CCDirector::sharedDirector()->addSubSystem(sysType);
}

NS_CC_END
