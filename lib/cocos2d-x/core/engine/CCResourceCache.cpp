#include "CCResourceCache.h"
#include <algorithm>
#include "resources/CCSpriteFrame.h"
#include "resources/CCPlistResource.h"

NS_CC_BEGIN
static const SharedPtr<Resource> noResource;

ResourceCache::ResourceCache()
{
}

void ResourceCache::addResource(ID resType, Resource* res)
{
	_resources.insert(_resources.begin(), SharedPtr<Resource>(res));
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
		_resources.insert(_resources.begin(), res);
		return res;
	}

	return nullptr;
}

Resource* ResourceCache::findResource(ID type, const std::string& path)
{
	auto iter = std::find_if(_resources.begin(), _resources.end(), 
		[&](SharedPtr<Resource>& ele)
	{
		return (ele->getType() == type && ele->getPath() == path);
	});

	if (iter != _resources.end())
		return *iter;

	return noResource;
}

void ResourceCache::removeUnused()
{
	// todo
}

NS_CC_END
