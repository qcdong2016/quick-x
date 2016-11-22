#include "CCModule.h"
#include "base/Ptr.h"
#include "CCPlatformMacros.h"
#include "CCDevice.h"
#include <vector>
#include <string>
#include "textures/CCTexture2D.h"
#include "sprite_nodes/CCSpriteFrame.h"
#include "resources/CCPlistResource.h"
#include "label_nodes/CCLabelBMFont.h"

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
	ObjectFactoryManager::addFactory<CCTexture2D>();
	ObjectFactoryManager::addFactory<CCSpriteFrame>();
    ObjectFactoryManager::addFactory<PlistResource>();
    ObjectFactoryManager::addFactory<CCBMFontConfiguration>();
}

void CoreModule::detach()
{

}


NS_CC_END

