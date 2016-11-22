#include "CCPlistResource.h"
#include "cocoa/CCDictionary.h"
#include "textures/CCTexture2D.h"
#include "sprite_nodes/CCSpriteFrame.h"

NS_CC_BEGIN


typedef std::vector<std::string> strArray;

// string toolkit
static inline void split(std::string src, const char* token, strArray& vect)
{
	size_t nend = 0;
	size_t nbegin = 0;
	while (nend != std::string::npos)
	{
		nend = src.find(token, nbegin);
		if (nend == -1)
			vect.push_back(src.substr(nbegin, src.length() - nbegin));
		else
			vect.push_back(src.substr(nbegin, nend - nbegin));
		nbegin = nend + strlen(token);
	}
}

// first, judge whether the form of the string like this: {x,y}
// if the form is right,the string will be split into the parameter strs;
// or the parameter strs will be empty.
// if the form is right return true,else return false.
static bool splitWithForm(const char* pStr, strArray& strs)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!pStr);

		// string is empty
		std::string content = pStr;
		CC_BREAK_IF(content.length() == 0);

		size_t nPosLeft = content.find('{');
		size_t nPosRight = content.find('}');

		// don't have '{' and '}'
		CC_BREAK_IF(nPosLeft == std::string::npos || nPosRight == std::string::npos);
		// '}' is before '{'
		CC_BREAK_IF(nPosLeft > nPosRight);

		std::string pointStr = content.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);
		// nothing between '{' and '}'
		CC_BREAK_IF(pointStr.length() == 0);

		size_t nPos1 = pointStr.find('{');
		size_t nPos2 = pointStr.find('}');
		// contain '{' or '}' 
		CC_BREAK_IF(nPos1 != std::string::npos || nPos2 != std::string::npos);

		split(pointStr, ",", strs);
		if (strs.size() != 2 || strs[0].length() == 0 || strs[1].length() == 0)
		{
			strs.clear();
			break;
		}

		bRet = true;
	} while (0);

	return bRet;
}

// implement the functions

static CCRect CCRectFromString(const char* pszContent)
{
	CCRect result = CCRectZero;

	do
	{
		CC_BREAK_IF(!pszContent);
		std::string content = pszContent;

		// find the first '{' and the third '}'
		size_t nPosLeft = content.find('{');
		size_t nPosRight = content.find('}');
		for (int i = 1; i < 3; ++i)
		{
			if (nPosRight == std::string::npos)
			{
				break;
			}
			nPosRight = content.find('}', nPosRight + 1);
		}
		CC_BREAK_IF(nPosLeft == std::string::npos || nPosRight == std::string::npos);

		content = content.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);
		size_t nPointEnd = content.find('}');
		CC_BREAK_IF(nPointEnd == std::string::npos);
		nPointEnd = content.find(',', nPointEnd);
		CC_BREAK_IF(nPointEnd == std::string::npos);

		// get the point string and size string
		std::string pointStr = content.substr(0, nPointEnd);
		std::string sizeStr = content.substr(nPointEnd + 1, content.length() - nPointEnd);

		// split the string with ','
		strArray pointInfo;
		CC_BREAK_IF(!splitWithForm(pointStr.c_str(), pointInfo));
		strArray sizeInfo;
		CC_BREAK_IF(!splitWithForm(sizeStr.c_str(), sizeInfo));

		float x = (float)atof(pointInfo[0].c_str());
		float y = (float)atof(pointInfo[1].c_str());
		float width = (float)atof(sizeInfo[0].c_str());
		float height = (float)atof(sizeInfo[1].c_str());

		result = CCRectMake(x, y, width, height);
	} while (0);

	return result;
}

static CCPoint CCPointFromString(const char* pszContent)
{
	CCPoint ret = CCPointZero;

	do
	{
		strArray strs;
		CC_BREAK_IF(!splitWithForm(pszContent, strs));

		float x = (float)atof(strs[0].c_str());
		float y = (float)atof(strs[1].c_str());

		ret = CCPointMake(x, y);
	} while (0);

	return ret;
}

static CCSize CCSizeFromString(const char* pszContent)
{
	CCSize ret = CCSizeZero;

	do
	{
		strArray strs;
		CC_BREAK_IF(!splitWithForm(pszContent, strs));

		float width = (float)atof(strs[0].c_str());
		float height = (float)atof(strs[1].c_str());

		ret = CCSizeMake(width, height);
	} while (0);

	return ret;
}

CCSpriteFrame* PlistResource::getFrame(const std::string& name)
{
    auto iter = _frames.find(name);
    if (iter == _frames.end())
        return nullptr;
    
    return iter->second;
}

void PlistResource::beginLoad(MemBuffer* buf, void* userdata)
{
	SharedPtr<CCDictionary> dict( CCDictionary::createWithContentsOfDataThreadSafe((const char*)buf->getData(), buf->getSize()));

	std::string dir = FileSystem::getDirectory(getPath());
	std::string basename = FileSystem::getBaseName(getPath());
	std::string texturePath("");

	CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
	if (metadataDict)
	{
		// try to read  texture file name from meta data
		texturePath = metadataDict->valueForKey("textureFileName")->getCString();
	}

	if (!texturePath.empty())
	{
		// build texture path relative to plist file
		texturePath = FileSystem::join(dir, texturePath);
	}
	else
	{
		// build texture path by replacing file extension
		texturePath = FileSystem::join(dir, basename + ".png");
		CCLOG("cocos2d: CCSpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
	}

	CCTexture2D *pTexture = SubSystem::get<ResourceCache>()->getResource<CCTexture2D>(texturePath);
	
	if (!pTexture)
	{
		CCLOG("cocos2d: CCSpriteFrameCache: Couldn't load texture");
		return;
	}
	_texture = pTexture;

	/*
	Supported Zwoptex Formats:

	ZWTCoordinatesFormatOptionXMLLegacy = 0, // Flash Version
	ZWTCoordinatesFormatOptionXML1_0 = 1, // Desktop Version 0.0 - 0.4b
	ZWTCoordinatesFormatOptionXML1_1 = 2, // Desktop Version 1.0.0 - 1.0.1
	ZWTCoordinatesFormatOptionXML1_2 = 3, // Desktop Version 1.0.2+
	*/

	CCDictionary *framesDict = (CCDictionary*)dict->objectForKey("frames");
	int format = 0;

	// get the format
	if (metadataDict != NULL)
	{
		format = metadataDict->valueForKey("format")->intValue();
	}

	ResourceCache* cache = SubSystem::get<ResourceCache>();
	// check the format
	CCAssert(format >= 0 && format <= 3, "format is not supported for CCSpriteFrameCache addSpriteFramesWithDictionary:textureFilename:");

	dir = FileSystem::join(dir, basename);

	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(framesDict, pElement)
	{
		CCDictionary* frameDict = (CCDictionary*)pElement->getObject();
		std::string spriteFrameName = pElement->getStrKey();
		std::string fullname = FileSystem::join(dir, spriteFrameName);
		CCSpriteFrame* spriteFrame = cache->findResource<CCSpriteFrame>(spriteFrameName);
		if (spriteFrame)
			continue;

		spriteFrame = new CCSpriteFrame();
		// fullname of spriteFrame
		spriteFrame->setPath(fullname);

		if (format == 0)
		{
			float x = frameDict->valueForKey("x")->floatValue();
			float y = frameDict->valueForKey("y")->floatValue();
			float w = frameDict->valueForKey("width")->floatValue();
			float h = frameDict->valueForKey("height")->floatValue();
			float ox = frameDict->valueForKey("offsetX")->floatValue();
			float oy = frameDict->valueForKey("offsetY")->floatValue();
			int ow = frameDict->valueForKey("originalWidth")->intValue();
			int oh = frameDict->valueForKey("originalHeight")->intValue();
			// check ow/oh
			if (!ow || !oh)
			{
				CCLOGWARN("cocos2d: WARNING: originalWidth/Height not found on the CCSpriteFrame. AnchorPoint won't work as expected. Regenrate the .plist");
			}
			// abs ow/oh
			ow = abs(ow);
			oh = abs(oh);
			// create frame
			spriteFrame->initWithTexture(_texture,
				CCRectMake(x, y, w, h),
				false,
				CCPointMake(ox, oy),
				CCSizeMake((float)ow, (float)oh)
			);
		}
		else if (format == 1 || format == 2)
		{
			CCRect frame = CCRectFromString(frameDict->valueForKey("frame")->getCString());
			bool rotated = false;

			// rotation
			if (format == 2)
			{
				rotated = frameDict->valueForKey("rotated")->boolValue();
			}

			CCPoint offset = CCPointFromString(frameDict->valueForKey("offset")->getCString());
			CCSize sourceSize = CCSizeFromString(frameDict->valueForKey("sourceSize")->getCString());

			// create frame
			spriteFrame->initWithTexture(_texture,
				frame,
				rotated,
				offset,
				sourceSize
			);
		}
		else if (format == 3)
		{
			// get values
			CCSize spriteSize = CCSizeFromString(frameDict->valueForKey("spriteSize")->getCString());
			CCPoint spriteOffset = CCPointFromString(frameDict->valueForKey("spriteOffset")->getCString());
			CCSize spriteSourceSize = CCSizeFromString(frameDict->valueForKey("spriteSourceSize")->getCString());
			CCRect textureRect = CCRectFromString(frameDict->valueForKey("textureRect")->getCString());
			bool textureRotated = frameDict->valueForKey("textureRotated")->boolValue();

			// get aliases
			CCArray* aliases = (CCArray*)(frameDict->objectForKey("aliases"));

			CCObject* pObj = NULL;
			CCARRAY_FOREACH(aliases, pObj)
			{
				std::string oneAlias = ((CCString*)pObj)->getCString();

				if (cache->getResource<CCSpriteFrame>(fullname))
					CCLOGWARN("cocos2d: WARNING: an alias with name %s already exists", oneAlias.c_str());
			}
			// create frame
			spriteFrame->initWithTexture(_texture,
				CCRectMake(textureRect.origin.x, textureRect.origin.y, spriteSize.width, spriteSize.height),
				textureRotated,
				spriteOffset,
				spriteSourceSize);
		}

		// add sprite frame
		cache->addResource<CCSpriteFrame>(spriteFrame);
		_frames[spriteFrameName] = (SharedPtr<CCSpriteFrame>(spriteFrame));
	}
}



NS_CC_END
