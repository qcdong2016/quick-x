#include "SubSystem.h"
#include "CCDirector.h"

NS_CC_BEGIN

SubSystem* SubSystem::get(ID sysType)
{
	return CCDirector::sharedDirector()->getSubSystem(sysType);
}

NS_CC_END