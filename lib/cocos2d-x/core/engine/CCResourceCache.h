
#pragma once

#include "SubSystem.h"
#include <vector>
#include "IO/FileSystem.h"

NS_CC_BEGIN

class CC_DLL Resource : public CCObject
{
	CCOBJECT(Resource, CCObject);

public:
	virtual ~Resource() {}

	const std::string& getPath() { return _path; }
	void setPath(const std::string& p) { _path = p; }

	virtual void beginLoad(MemBuffer* buf, void* userdata) {};

protected:
	friend class ResourceCache;

	std::string _path;
};

class CC_DLL ResourceCache : public SubSystem
{
	CCOBJECT(ResourceCache, SubSystem)

public:
	ResourceCache();

	void addResource(ID resType, Resource* res);
	template<typename T>void addResource(Resource* res)
	{
		addResource(T::getTypeStatic(), res);
	}

	Resource* getResource(ID resType, const std::string& path, void* userdata = 0);
	template <class T> 
	T* getResource(const std::string& path, void* userdata = 0)
	{
		return (T*)getResource(T::getTypeStatic(), path, userdata);
	}

	void removeUnused();

private:
	const SharedPtr<Resource>& findResource(ID type, const std::string& path);

	std::vector<SharedPtr<Resource> > _resources;
};

NS_CC_END
