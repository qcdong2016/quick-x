#include "FileSystem.h"
#include "CCPlatformMacros.h"
#include "CCFileUtils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

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

#ifdef __ANDROID__
#include <android/asset_manager.h>
// extern AAssetManager* __assetManager;
#endif

NS_CC_BEGIN


MemBuffer::~MemBuffer()
{
	CC_SAFE_DELETE_ARRAY(_data);
}


void FileSystem::addResourcePath(const std::string& path)
{
	CCFileUtils::sharedFileUtils()->addSearchPath(path.c_str());
}

bool FileSystem::listFiles(const std::string& dirPath, std::vector<std::string>& files)
{
	std::string path(CCFileUtils::sharedFileUtils()->getSearchRootPath());

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

    path.append("/.");
    bool result = false;

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

#ifdef __ANDROID__
    // List the files that are in the android APK at this path
    // AAssetDir* assetDir = AAssetManager_openDir(__assetManager, dirPath);
    // if (assetDir != NULL)
    // {
    //     AAssetDir_rewind(assetDir);
    //     const char* file = NULL;
    //     while ((file = AAssetDir_getNextFileName(assetDir)) != NULL)
    //     {
    //         std::string filename(file);
    //         // Check if this file was already added to the list because it was copied to the SD card.
    //         if (find(files.begin(), files.end(), filename) == files.end())
    //         {
    //             files.push_back(filename);
    //         }
    //     }
    //     AAssetDir_close(assetDir);
    //     result = true;
    // }
#endif

    return result;
#endif
}

bool FileSystem::fileExists(const std::string& filePath)
{
    std::string fullPath;

#ifdef __ANDROID__
    // fullPath = __assetPath;
    // fullPath += resolvePath(filePath);

    // if (androidFileExists(fullPath.c_str()))
    // {
    //     return true;
    // }
#endif

	fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath.c_str());

    gp_stat_struct s;
    return stat(fullPath.c_str(), &s) == 0;

}

SharedPtr<MemBuffer> FileSystem::readAll(const std::string& filePath)
{
	unsigned long size;
	unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(filePath.c_str(), "rb", &size);
	return SharedPtr<MemBuffer>(new MemBuffer(data, size));
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

std::string FileSystem::getDirectoryName(const std::string& path)
{
    if (path.empty())
    {
        return "";
    }
#ifdef WIN32
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    _splitpath(path.c_str(), drive, dir, NULL, NULL);
    std::string dirname;
    size_t driveLength = strlen(drive);
    if (driveLength > 0)
    {
        dirname.reserve(driveLength + strlen(dir));
        dirname.append(drive);
        dirname.append(dir);
    }
    else
    {
        dirname.assign(dir);
    }
    std::replace(dirname.begin(), dirname.end(), '\\', '/');
    return dirname;
#else
    // dirname() modifies the input string so create a temp string
    std::string dirname;
    char* tempPath = new char[path.size() + 1];
    strcpy(tempPath, path.c_str());
    char* dir = ::dirname(tempPath);
    if (dir && strlen(dir) > 0)
    {
        dirname.assign(dir);
        // dirname() strips off the trailing '/' so add it back to be consistent with Windows
        dirname.append("/");
    }
    CC_SAFE_DELETE(tempPath);
    return dirname;
#endif
}

std::string FileSystem::getExtension(const std::string& path)
{
	std::string::size_type pos = path.rfind('.');
	if (pos != std::string::npos)
		return path.substr(pos + 1);
	else
		return std::string();
}


NS_CC_END
