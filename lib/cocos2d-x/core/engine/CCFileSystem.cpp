#include "CCFileSystem.h"
#include "CCPlatformMacros.h"
#include "ccMacros.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

extern "C"
{
#include "crypto/xxtea.h"
}
#ifdef WIN32
    #include <windows.h>
    #include <tchar.h>
    #include <stdio.h>
    #include <direct.h>
    #define gp_stat _stat
    #define gp_stat_struct struct stat
#else
    #define __EXT_POSIX2
    #include <libgen.h>
    #include <dirent.h>
    #define gp_stat stat
    #define gp_stat_struct struct stat
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "android/asset_manager_jni.h"
static AAssetManager* s_assetManager = nullptr;
#endif

#include "support/zip_support/ZipUtils.h"
#include "CCDevice.h"

NS_CC_BEGIN


bool DecoderXXTea::is(unsigned char* data, size_t size)
{
	return (memcmp(data, _sign.c_str(), _sign.length()) == 0);
}


unsigned char* DecoderXXTea::decode(unsigned char* data, size_t& size_inout)
{
	// decrypt XXTEA
	xxtea_long len = 0;
	unsigned char* tbuff = xxtea_decrypt(data + _sign.length(), (xxtea_long)size_inout - (xxtea_long)_sign.length(),
		(unsigned char*)_key.c_str(), (xxtea_long)_key.length(), &len);

	unsigned char* buffer = new unsigned char[len];
	size_inout = len;

	memcpy(buffer, tbuff, len);
	free(tbuff);

	return buffer;
}


MemBuffer::~MemBuffer()
{
	CC_SAFE_DELETE_ARRAY(_data);
}

static std::vector<std::string> s_searchPath;
static std::string s_searchRoot = ".";

void FileSystem::setAssetsManager(void* assetsManager)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	s_assetManager = (AAssetManager*)assetsManager;
#endif
}

void FileSystem::setResourceRoot(const std::string& root)
{
	s_searchRoot = root;
}

void FileSystem::addResourcePath(const std::string& path)
{
	if (!isAbsolutePath(path))
		s_searchPath.push_back(join(s_searchRoot, path));
	else
		s_searchPath.push_back(path);
}

std::string FileSystem::getWritablePath()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return ".";// the exe path;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return getFileDirectoryJNI();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return CCDevice::getWritablePath();
#endif
}

bool FileSystem::listFiles(const std::string& dirPath, std::vector<std::string>& files)
{
	std::string path;// (CCFileUtils::sharedFileUtils()->getSearchRootPath());

    if (!dirPath.empty())
    {
        path.append(dirPath);
    }
#ifdef WIN32

    path.append("/*");
    // Convert char to wchar
    std::basic_string<TCHAR> wPath;
    wPath.assign(path.begin(), path.end());

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(wPath.c_str(), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) 
    {
        return false;
    }
    do
    {
        // Add to the list if this is not a directory
        if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        {
            // Convert wchar to char
            std::basic_string<TCHAR> wfilename(FindFileData.cFileName);
            std::string filename;
            filename.assign(wfilename.begin(), wfilename.end());
            files.push_back(filename);
        }
    } while (FindNextFile(hFind, &FindFileData) != 0);

    FindClose(hFind);
    return true;

#else

    bool result = false;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (!isAbsolutePath(dirPath)) {
    	// List the files that are in the android APK at this path
	    AAssetDir* assetDir = AAssetManager_openDir(s_assetManager, dirPath.c_str());
	    if (assetDir != NULL)
	    {
	        AAssetDir_rewind(assetDir);
	        const char* file = NULL;
	        while ((file = AAssetDir_getNextFileName(assetDir)) != NULL)
	        {
	            std::string filename(file);
	            // Check if this file was already added to the list because it was copied to the SD card.
	            if (find(files.begin(), files.end(), filename) == files.end())
	            {
	                files.push_back(filename);
	            }
	        }
	        AAssetDir_close(assetDir);
	        result = true;
	    }
	    return result;
    }
    
#endif

    path.append("/.");

    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if (dir != NULL)
    {
        while ((dp = readdir(dir)) != NULL)
        {
            std::string filepath(path);
            filepath.append("/");
            filepath.append(dp->d_name);

            struct stat buf;
            if (!stat(filepath.c_str(), &buf))
            {
                // Add to the list if this is not a directory
                if (!S_ISDIR(buf.st_mode))
                {
                    files.push_back(dp->d_name);
                }
            }
        }
        closedir(dir);
        result = true;
    }

    return result;
#endif
}


bool FileSystem::isFileExist(const std::string& filePath)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (!isAbsolutePath(filePath)) {
        AAsset* asset = AAssetManager_open(s_assetManager, filePath.c_str(), AASSET_MODE_UNKNOWN);
        if (asset) {
            int length = AAsset_getLength(asset);
            AAsset_close(asset);
            return length > 0;
        }
        return false;
    }
#endif
    
    gp_stat_struct s;
    return stat(filePath.c_str(), &s) == 0;
    
}

std::string FileSystem::fullPathOfFile(const std::string& filename)
{
    std::string test;
    
    if (!isAbsolutePath(filename))
        test = join(s_searchRoot, filename);
    else
        test = filename;
    
    if (isFileExist(test))
        return test;
    
    for (std::string& ps : s_searchPath) {
        test = join(ps, filename);
        
        if (isFileExist(test)) {
            return test;
        }
    }
    
    return filename;
}

static unsigned char* readFile(const std::string& fullPath, size_t* pSize)
{
	unsigned char * pBuffer = NULL;
	CCAssert(!fullPath.empty() && pSize != NULL, "Invalid parameters.");
	*pSize = 0;
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	if (!FileSystem::isAbsolutePath(fullPath)) {
		AAsset* asset = AAssetManager_open(s_assetManager, fullPath.c_str(), AASSET_MODE_UNKNOWN);
	    if (asset) {
		    *pSize = AAsset_getLength(asset);
			pBuffer = new unsigned char[*pSize];

		    int readsize = AAsset_read(asset, pBuffer, *pSize);
		    AAsset_close(asset);
	    }
	}
	else
#endif
	{
		// read the file from hardware
		FILE *fp = fopen(fullPath.c_str(), "rb");
		if (fp) {
			fseek(fp, 0, SEEK_END);
			*pSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			pBuffer = new unsigned char[*pSize];
			*pSize = fread(pBuffer, sizeof(unsigned char), *pSize, fp);
			fclose(fp);
		}
	}

	if (!pBuffer)
		CCLog("Get data from file(%s) failed.", fullPath.c_str());

	return pBuffer;
}

SharedPtr<MemBuffer> FileSystem::readAll(const std::string& filePath)
{
	size_t size;
	unsigned char* data = readFile(fullPathOfFile(filePath), &size);

	if (data && _dataDecoder.Get() && _dataDecoder->is(data, size))
	{
		unsigned char* buf = _dataDecoder->decode(data, size);
		delete[] data;

		data = buf;
	}

	return SharedPtr<MemBuffer>(new MemBuffer(data, size));
}

std::string FileSystem::readAllString(const std::string& filePath)
{
	SharedPtr<MemBuffer> bf = readAll(filePath);

	if (bf->isNull())
		return "";

	return std::string((char*)bf->getData(), bf->getSize());
}

bool FileSystem::isAbsolutePath(const std::string& filePath)
{
    if (filePath.empty() || filePath[0] == '\0')
        return false;
#ifdef WIN32
    if (filePath[1] != '\0')
    {
        char first = filePath[0];
        return (filePath[1] == ':' && ((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z')));
    }
    return false;
#else
    return filePath[0] == '/';
#endif
}

std::string FileSystem::getName(const std::string& name)
{
	int i = name.find_last_of("/\\");
	if (i != -1)
		return name.substr(i + 1);
	else
		return name;
}

std::string FileSystem::getBaseName(const std::string& name)
{
	std::string fullname = getName(name);
	int i = fullname.find_last_of('.');

	if (i != -1)
		return fullname.substr(0, i);
	else
		return fullname;
}

std::string FileSystem::getDirectoryName(const std::string& path)
{
	return getName(getDirectory(path));
}

std::string FileSystem::getDirectory(const std::string& path)
{
	int i = path.find_last_of("/\\");
	if (i != -1)
		return path.substr(0, i);

	return "";
}

std::string FileSystem::join(const std::string& a, const std::string& b)
{
    if (a.size() == 0 || a == ".")
        return b;
    
	int i = a.find_last_of("/\\");
	if (i != a.length() - 1)
		return a + '/' + b;
	else
		return a + b;
}

std::string FileSystem::getExtension(const std::string& path)
{
	std::string::size_type pos = path.rfind('.');
	if (pos != std::string::npos)
		return path.substr(pos + 1);
	else
		return std::string();
}

std::string FileSystem::replaceExtension(const std::string& path, const std::string& newExt)
{
	std::string::size_type pos = path.rfind('.');
	if (pos != std::string::npos)
		return path.substr(0, pos) + "." + newExt;
	else
		return path + "." + newExt;
}

SharedPtr<Decoder> FileSystem::_dataDecoder;

NS_CC_END

