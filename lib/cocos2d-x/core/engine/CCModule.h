
#pragma once
#include "CCPlatformMacros.h"
#include "base/RefCounted.h"
#include <string>

NS_CC_BEGIN


class Module : public RefCounted
{
public:
	virtual void attach() = 0;
	virtual void detach() = 0;
};

class ModuleManager
{
public:
	template<typename T>
	static bool addModule(const std::string& name) {
		return addModule(name, new T);
	}
	static bool addModule(const std::string& name, Module* m);

	static void attachAll();
};

#define CC_MODULE_DEFINE(type) \
	static bool _##type = cocos2d::ModuleManager::addModule<type>(#type);
NS_CC_END