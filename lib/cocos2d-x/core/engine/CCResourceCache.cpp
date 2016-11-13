#include "CCResourceCache.h"
#include <algorithm>

NS_CC_BEGIN
static const SharedPtr<Resource> noResource;

void Resource::beginLoad(SharedPtr<MemBuffer> buf)
{

}

ResourceCache::ResourceCache()
{
}

SharedPtr<Resource> ResourceCache::getResource(ID resType, const std::string& path)
{
	const SharedPtr<Resource> cached = findResource(resType, path);

	if (cached)
		return cached;

	SharedPtr<MemBuffer> buf = FileSystem::readAll(path);

	if (buf->isNull())
		return noResource;

	SharedPtr<Resource> res(ObjectFactoryManager::newObject<Resource>(resType));

	res->beginLoad(buf);

	return res;
}

const SharedPtr<Resource>& ResourceCache::findResource(ID type, const std::string& path)
{
	class finder
	{
	public:
		const char* path;
		finder(const char* s) : path(s) {}

		bool operator () (SharedPtr<Resource>& ele) {
			if (ele->getPath().compare(path) == 0)
				return true;
		}
	};

	auto iter = std::find_if(_resources.begin(), _resources.end(), finder(path.c_str()));

	if (iter != _resources.end())
		return *iter;

	return noResource;
}

void ResourceCache::removeUnused()
{
	// todo
}

NS_CC_END
