#include "SpineResource.h"
#include "spine/SkeletonJson.h"
#include "ccMacros.h"
NS_CC_BEGIN



SpineAtlasResource::SpineAtlasResource()
	: _atlas(nullptr)
{
}

SpineAtlasResource::~SpineAtlasResource()
{
	if (_atlas)
		spAtlas_dispose(_atlas);
}

void SpineAtlasResource::beginLoad(SharedPtr<MemBuffer> buf, void* userdata)
{
	_atlas = spAtlas_create((const char*)buf->getData(), 
		buf->getSize(), FileSystem::getDirectory(_path).c_str(), nullptr);
}

SpineSkeletonDataResource::SpineSkeletonDataResource()
	: _skeletonData(nullptr)
{

}

SpineSkeletonDataResource::~SpineSkeletonDataResource()
{
	if (_skeletonData)
		spSkeletonData_dispose(_skeletonData);
}

void SpineSkeletonDataResource::beginLoad(SharedPtr<MemBuffer> buf, void* userdata)
{
	SpineAtlasResource* atlas = (SpineAtlasResource*)userdata;

	spSkeletonJson* json = spSkeletonJson_create(atlas->_atlas);
	json->scale = 1.0f;

	_skeletonData = spSkeletonJson_readSkeletonData(json, (const char*)buf->getData());

	CCAssert(_skeletonData, json->error ? json->error : "Error reading skeleton data.");
	spSkeletonJson_dispose(json);
}

NS_CC_END
