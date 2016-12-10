
#pragma once
#include "engine/CCSubSystem.h"
#include "engine/CCModule.h"

NS_CC_BEGIN

class NoUIModule : public Module
{
public:
	NoUIModule();
	~NoUIModule();

	void draw(EventData& data);

private:
	void* _nvg;
};

NS_CC_END