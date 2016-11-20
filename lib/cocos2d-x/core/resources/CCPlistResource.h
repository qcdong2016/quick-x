
#pragma once

#include "CCPlatformMacros.h"
#include "engine/CCResourceCache.h"

NS_CC_BEGIN

class CCDictionary;
class CCTexture2D;

class PlistResource : public Resource
{
	CCOBJECT(PlistResource, Resource);

public:

	virtual void beginLoad(MemBuffer* buf, void* userdata);
	
	void addSpriteFramesWithDictionary(CCDictionary* dict, CCTexture2D* tex);
};

NS_CC_END