#include "CCResourceCache.h"
#include <algorithm>

NS_CC_BEGIN
static const SharedPtr<Resource> noResource;

ResourceCache::ResourceCache()
{
}

Resource* ResourceCache::getResource(ID resType, const std::string& path, void* userdata /* = 0 */)
{
	const SharedPtr<Resource> cached = findResource(resType, path);

	if (cached)
		return cached;

	SharedPtr<MemBuffer> buf = FileSystem::readAll(path);

	SharedPtr<Resource> res(ObjectFactoryManager::newObject<Resource>(resType));

	res->_path = path;

	if (!buf->isNull()) {
		res->beginLoad(buf, userdata);
		_resources.insert(_resources.begin(), res);
	}

	return res;
}

const SharedPtr<Resource>& ResourceCache::findResource(ID type, const std::string& path)
{
	auto iter = std::find_if(_resources.begin(), _resources.end(), 
		[&](SharedPtr<Resource>& ele)
	{
		if (ele->getType() == type && ele->getPath() == path)
			return true;
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
