#include "CCResourceCache.h"
#include <algorithm>
#include "resources/CCSpriteFrame.h"
#include "resources/CCPlistResource.h"
#include "resources/CCAnimation.h"
#include "CCFileSystem.h"

NS_CC_BEGIN

ResourceCache::ResourceCache()
{
}

void ResourceCache::addResource(ID resType, Resource* res)
{
	_resources[resType].insert(SharedPtr<Resource>(res));
}

Resource* ResourceCache::getResource(ID resType, const std::string& path, void* userdata /* = 0 */)
{
	std::string fullpath = FileSystem::fullPathOfFile(path);
	Resource* cached = findResource(resType, fullpath);

	if (cached)
		return cached;
	
	// For auto loading plist. 
	// TODO: bg loading.
	if (!FileSystem::isFileExist(fullpath) && resType == CCSpriteFrame::getTypeStatic()) {
		std::string plist = (FileSystem::getDirectory(fullpath) + ".plist");

		if (!FileSystem::isFileExist(FileSystem::fullPathOfFile(plist)))
			return nullptr;

		PlistResource* p = ResourceCache::getResource<PlistResource>(plist);
        
        return p->getFrame(FileSystem::getName(fullpath));
	}


	SharedPtr<MemBuffer> buf = FileSystem::readAll(fullpath);
	if (!buf->isNull()) {

		SharedPtr<Resource> res(ObjectFactoryManager::newObject<Resource>(resType));

		res->_path = fullpath;

		res->beginLoad(buf, userdata);
		addResource(res);
		return res;
	}

	return nullptr;
}

Resource* ResourceCache::findResource(ID resType, const std::string& path)
{
	auto iter = _resources.find(resType);

	if (iter == _resources.end())
		return nullptr;

	auto& group = iter->second;
	auto resIter = std::find_if(group.begin(), group.end(),
		[&](const SharedPtr<Resource>& res)
	{
		return (res->getPath() == path);
	});

	if (resIter != group.end())
		return *resIter;

	return nullptr;
}

Resource* ResourceCache::getResourceBG(ID resType, const std::string& path, void* userdata /*= 0*/)
{
	// todo
	return nullptr;
}

void ResourceCache::removeUnusedByType(ID resType)
{
	auto groupIter = _resources.find(resType);

	if (groupIter == _resources.end())
		return;

	auto& group = groupIter->second;

	for (auto iter = group.begin(); iter != group.end();)
	{
		if (iter->Refs() == 1)
			iter = group.erase(iter);
		else
			iter++;
	}
}

void ResourceCache::removeUnused()
{
	removeUnusedByType<PlistResource>();
	removeUnusedByType<CCAnimation>();
	removeUnusedByType<CCSpriteFrame>();
	removeUnusedByType<CCTexture2D>();
}

NS_CC_END


