
#pragma once

#include "CCPlatformMacros.h"
#include "base/RefCounted.h"
#include <string>

NS_CC_BEGIN


class CC_DLL Module : public RefCounted
{
public:
	virtual void attach() = 0;
	virtual void detach() = 0;
};

class CC_DLL ModuleManager
{
public:
	template<typename T>
	static bool addModule() {
		return addModule(new T);
	}
	static bool addModule(Module* m);
};


class CC_DLL CoreModule : public Module
{
public:
	virtual void attach();
	virtual void detach();
};

NS_CC_END