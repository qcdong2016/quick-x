#pragma once

#include "engine/CCObject.h"

NS_CC_BEGIN

class CC_DLL SubSystem : public CCObject
{
	CCOBJECT(SubSystem, CCObject)
public:
	virtual ~SubSystem() {}

	static SubSystem* get(ID sysType);
	template<typename T> static T* get()
	{
		return (T*)get(T::getTypeStatic());
	}

	static SubSystem* add(ID sysType);
	template<typename T> static T* add()
	{
		return (T*)add(T::getTypeStatic());
	}
};

NS_CC_END