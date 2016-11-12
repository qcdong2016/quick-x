#pragma once

#include "engine/CCObject.h"

NS_CC_BEGIN

class CC_DLL SubSystem : public CCObject
{
	CCOBJECT(SubSystem, CCObject)
public:
	virtual ~SubSystem() {}
};

NS_CC_END