#pragma once

#include <string>
#include <vector>
#include "CCPlatformMacros.h"
#include "cocos/RefCounted.h"
#include "cocos/Ptr.h"

NS_CC_BEGIN

class MemBuffer : public RefCounted
{
public:

	MemBuffer() : _data(nullptr), _size(0) {}

	MemBuffer(unsigned char* data, unsigned long size) 
		: _data(data)
		, _size(size)
	{
	}

	virtual ~MemBuffer();

	unsigned long getSize() { return _size; }
	unsigned char* getData() { return _data;  }
	bool isNull() { return _data == nullptr; }

protected:

	unsigned char* _data;
	unsigned long _size;
};

class FileSystem
{
public:
	static void addResourcePath(const std::string& path);

	static SharedPtr<MemBuffer> readAll(const std::string& filePath);

	static bool listFiles(const std::string& dirPath, std::vector<std::string>& files);
	static bool fileExists(const std::string& filePath);
	static bool isAbsolutePath(const std::string& filePath);
	static std::string getDirectoryName(const std::string& path);
	static std::string getExtension(const std::string& path);
};

NS_CC_END

