
#pragma once

#include "CCSubSystem.h"
#include <set>

NS_CC_BEGIN

class MemBuffer;

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
	void addResource(Resource* res)
	{
		addResource(res->getType(), res);
	}

	Resource* findResource(ID type, const std::string& path);
	template<typename T> T* findResource(const std::string& path)
	{
		return (T*)findResource(T::getTypeStatic(), path);
	}

	Resource* getResource(ID resType, const std::string& path, void* userdata = 0);
	template <class T> 
	T* getResource(const std::string& path, void* userdata = 0)
	{
		return (T*)getResource(T::getTypeStatic(), path, userdata);
	}

	Resource* getResourceBG(ID resType, const std::string& path, void* userdata = 0);
	template <class T>
	T* getResourceBG(const std::string& path, void* userdata = 0)
	{
		return (T*)getResourceBG(T::getTypeStatic(), path, userdata);
	}

	void removeUnusedByType(ID resType);
	template <class T>
	void removeUnusedByType() { removeUnusedByType(T::getTypeStatic()); }

	void removeUnused();

private:
	typedef std::set<SharedPtr<Resource> > ResourceGroup;
	typedef std::map<ID, ResourceGroup> ResourceMap;

	ResourceMap _resources;
};

NS_CC_END
