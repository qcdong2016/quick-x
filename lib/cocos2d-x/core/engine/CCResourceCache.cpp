#include "CCResourceCache.h"
#include <algorithm>
#include "sprite_nodes/CCSpriteFrame.h"
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
	Resource* cached = findResource(resType, path);

	if (cached)
		return cached;

	// For auto loading plist. 
	// TODO: bg loading.
	if (!FileSystem::isFileExist(path) && resType == CCSpriteFrame::getTypeStatic()) {
		std::string plist = (FileSystem::getDirectory(path) + ".plist");

		if (!FileSystem::isFileExist(FileSystem::fullPathOfFile(plist)))
			return nullptr;

		ResourceCache::getResource(PlistResource::getTypeStatic(), plist);
		return getResource(resType, path);
	}


	SharedPtr<MemBuffer> buf = FileSystem::readAll(path);
	if (!buf->isNull()) {

		SharedPtr<Resource> res(ObjectFactoryManager::newObject<Resource>(resType));

		res->_path = path;

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
