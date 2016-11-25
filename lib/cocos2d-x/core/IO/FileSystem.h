#pragma once

#include <string>
#include <vector>
#include "CCPlatformMacros.h"
#include "base/RefCounted.h"
#include "base/Ptr.h"

NS_CC_BEGIN

class CC_DLL Decoder : public RefCounted
{
public:
	virtual ~Decoder() {}

	virtual unsigned char* decode(unsigned char* data, size_t& size_inout) = 0;
	virtual bool is(unsigned char* data, size_t size) = 0;
};

class CC_DLL DecoderXXTea : public Decoder
{
public:
	DecoderXXTea(const std::string& sign, const std::string& key) : _sign(sign), _key(key) {}
	virtual unsigned char* decode(unsigned char* data, size_t& size_inout);
	virtual bool is(unsigned char* data, size_t size);

private:
	std::string _sign;
	std::string _key;
};

class CC_DLL MemBuffer : public RefCounted
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

class CC_DLL FileSystem
{
public:
	template<typename T> 
	static void setDecoder() { _dataDecoder = SharedPtr<Decoder>(new T); }
	static Decoder* getDecoder() { return _dataDecoder; }
	static SharedPtr<MemBuffer> readAll(const std::string& filePath);
	static std::string readAllString(const std::string& filePath);

	static void setResourceRoot(const std::string& root);
	static void addResourcePath(const std::string& path);
	static std::string fullPathOfFile(const std::string& filename);

	static std::string getWritablePath();

	static bool listFiles(const std::string& dirPath, std::vector<std::string>& files);
	static bool isFileExist(const std::string& filePath);
	static bool isAbsolutePath(const std::string& filePath);

	static std::string getName(const std::string& name);
	static std::string getBaseName(const std::string& name);
	static std::string getDirectoryName(const std::string& path);
	static std::string getDirectory(const std::string& path);
	static std::string join(const std::string& a, const std::string& b);
	/// getExtension without '.'
	static std::string getExtension(const std::string& path);
	/// replaceExtension('a.jpg', 'png') -> a.png
	static std::string replaceExtension(const std::string& path, const std::string& newExt);

	/// internal
	static void setAssetsManager(void* assetsManager);
private:
	static SharedPtr<Decoder> _dataDecoder;
};

NS_CC_END

