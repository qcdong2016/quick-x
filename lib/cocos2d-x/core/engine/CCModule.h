
#pragma once

#include "CCObject.h"
#include <string>

NS_CC_BEGIN


class CC_DLL Module : public CCObject
{
public:
	virtual ~Module() {};
};

class CC_DLL ModuleManager
{
public:
	template<typename T>
	static bool addModule() {
		return addModule(new T);
	}
	static bool addModule(Module* m);

	static void shutdown();
};


class CC_DLL CoreModule : public Module
{
public:
	CoreModule();
};

NS_CC_END