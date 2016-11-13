
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
	static bool addModule() {
		return addModule(new T);
	}
	static bool addModule(Module* m);
};

NS_CC_END