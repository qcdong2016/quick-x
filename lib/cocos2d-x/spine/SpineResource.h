
#pragma once
#include "engine/CCResourceCache.h"
#include "spine/SkeletonData.h"
#include "spine/Atlas.h"

NS_CC_BEGIN

class CC_DLL SpineAtlasResource : public Resource
{
	CCOBJECT(SpineAtlasResource, Resource);
public:
	SpineAtlasResource();
	~SpineAtlasResource();
	
	virtual void beginLoad(MemBuffer* buf, void* userdata);

	spAtlas* _atlas;
};

class CC_DLL SpineSkeletonDataResource : public Resource
{
	CCOBJECT(SpineSkeletonDataResource, Resource);
public:
	SpineSkeletonDataResource();
	~SpineSkeletonDataResource();
	virtual void beginLoad(MemBuffer* buf, void* userdata);
	spSkeletonData* _skeletonData;
};

NS_CC_END