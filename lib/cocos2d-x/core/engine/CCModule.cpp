#include "CCModule.h"
#include "base/Ptr.h"
#include "CCPlatformMacros.h"
#include "CCDevice.h"
#include <vector>
#include <string>

NS_CC_BEGIN

static std::vector<SharedPtr<Module> > s_modules;

bool ModuleManager::addModule(Module* m)
{
	s_modules.push_back(SharedPtr<Module>(m));
	m->attach();
	return true;
}

NS_CC_END

