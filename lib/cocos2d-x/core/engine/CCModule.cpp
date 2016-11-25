#include "CCModule.h"
#include "base/Ptr.h"
#include "CCPlatformMacros.h"
#include "CCDevice.h"
#include <vector>
#include <string>
#include "textures/CCTexture2D.h"
#include "resources/CCSpriteFrame.h"
#include "resources/CCPlistResource.h"
#include "nodes/CCLabelBMFont.h"
#include "CCDebugHud.h"
#include "CCScheduler.h"
#include "actions/CCActionManager.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "CCAccelerometer.h"

NS_CC_BEGIN

static std::vector<SharedPtr<Module> > s_modules;

bool ModuleManager::addModule(Module* m)
{
	s_modules.push_back(SharedPtr<Module>(m));
	m->attach();
	return true;
}


void CoreModule::attach()
{
	// subsystems 
	ObjectFactoryManager::addFactory<DebugHud>();
	ObjectFactoryManager::addFactory<ResourceCache>();
	ObjectFactoryManager::addFactory<CCScheduler>();
	ObjectFactoryManager::addFactory<CCActionManager>();
	ObjectFactoryManager::addFactory<CCTouchDispatcher>();
	ObjectFactoryManager::addFactory<CCAccelerometer>();

	// resources
	ObjectFactoryManager::addFactory<CCTexture2D>();
	ObjectFactoryManager::addFactory<CCSpriteFrame>();
    ObjectFactoryManager::addFactory<PlistResource>();
    ObjectFactoryManager::addFactory<CCBMFontConfiguration>();
}

void CoreModule::detach()
{

}


NS_CC_END

