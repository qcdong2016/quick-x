#include "CCModule.h"
#include "base/Ptr.h"
#include <string>
#include <map>
#include "CCPlatformMacros.h"
#include "CCDevice.h"

NS_CC_BEGIN

static std::map<std::string, SharedPtr<Module> > namedModules;

bool ModuleManager::addModule(const std::string& name, Module* m)
{
	namedModules[name] = SharedPtr<Module>(m);
	return true;
}

void ModuleManager::attachAll()
{
	std::map<std::string, SharedPtr<Module> >::iterator iter = namedModules.begin();

	for (; iter != namedModules.end(); iter++)
	{
		CCLOG("attach module %s", iter->first.c_str());
		iter->second->attach();
	}
}

NS_CC_END
