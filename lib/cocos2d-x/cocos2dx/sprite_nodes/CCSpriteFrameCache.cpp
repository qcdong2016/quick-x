/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2009      Jason Booth
 Copyright (c) 2009      Robert J Payne
 Copyright (c) 2011      Zynga Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "ccMacros.h"
#include "textures/CCTextureCache.h"
#include "CCSpriteFrameCache.h"
#include "CCSpriteFrame.h"
#include "CCSprite.h"
#include "support/TransformUtils.h"
#include "platform/CCFileUtils.h"
#include "cocoa/CCString.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCDictionary.h"
#include <vector>

using namespace std;

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

CCRect CCRectFromString(const char* pszContent)
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

CCPoint CCPointFromString(const char* pszContent)
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

CCSize CCSizeFromString(const char* pszContent)
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


static CCSpriteFrameCache *pSharedSpriteFrameCache = NULL;

CCSpriteFrameCache* CCSpriteFrameCache::sharedSpriteFrameCache(void)
{
    if (! pSharedSpriteFrameCache)
    {
        pSharedSpriteFrameCache = new CCSpriteFrameCache();
        pSharedSpriteFrameCache->init();
    }

    return pSharedSpriteFrameCache;
}

void CCSpriteFrameCache::purgeSharedSpriteFrameCache(void)
{
    CC_SAFE_RELEASE_NULL(pSharedSpriteFrameCache);
}

bool CCSpriteFrameCache::init(void)
{
    m_pSpriteFrames= new CCDictionary();
    m_pSpriteFramesAliases = new CCDictionary();
    m_pLoadedFileNames = new std::set<std::string>();
    return true;
}

CCSpriteFrameCache::~CCSpriteFrameCache(void)
{
    CC_SAFE_RELEASE(m_pSpriteFrames);
    CC_SAFE_RELEASE(m_pSpriteFramesAliases);
    CC_SAFE_DELETE(m_pLoadedFileNames);
}

void CCSpriteFrameCache::addSpriteFramesWithDictionary(CCDictionary* dictionary, CCTexture2D *pobTexture)
{
    /*
     Supported Zwoptex Formats:

     ZWTCoordinatesFormatOptionXMLLegacy = 0, // Flash Version
     ZWTCoordinatesFormatOptionXML1_0 = 1, // Desktop Version 0.0 - 0.4b
     ZWTCoordinatesFormatOptionXML1_1 = 2, // Desktop Version 1.0.0 - 1.0.1
     ZWTCoordinatesFormatOptionXML1_2 = 3, // Desktop Version 1.0.2+
     */

    CCDictionary *metadataDict = (CCDictionary*)dictionary->objectForKey("metadata");
    CCDictionary *framesDict = (CCDictionary*)dictionary->objectForKey("frames");
    int format = 0;

    // get the format
    if(metadataDict != NULL)
    {
        format = metadataDict->valueForKey("format")->intValue();
    }

    // check the format
    CCAssert(format >=0 && format <= 3, "format is not supported for CCSpriteFrameCache addSpriteFramesWithDictionary:textureFilename:");

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(framesDict, pElement)
    {
        CCDictionary* frameDict = (CCDictionary*)pElement->getObject();
        std::string spriteFrameName = pElement->getStrKey();
        CCSpriteFrame* spriteFrame = (CCSpriteFrame*)m_pSpriteFrames->objectForKey(spriteFrameName);
        if (spriteFrame)
        {
            continue;
        }

        if(format == 0)
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
            if(!ow || !oh)
            {
                CCLOGWARN("cocos2d: WARNING: originalWidth/Height not found on the CCSpriteFrame. AnchorPoint won't work as expected. Regenrate the .plist");
            }
            // abs ow/oh
            ow = abs(ow);
            oh = abs(oh);
            // create frame
            spriteFrame = new CCSpriteFrame();
            spriteFrame->initWithTexture(pobTexture,
                                         CCRectMake(x, y, w, h),
                                         false,
                                         CCPointMake(ox, oy),
                                         CCSizeMake((float)ow, (float)oh)
                                         );
        }
        else if(format == 1 || format == 2)
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
            spriteFrame = new CCSpriteFrame();
            spriteFrame->initWithTexture(pobTexture,
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
            CCArray* aliases = (CCArray*) (frameDict->objectForKey("aliases"));
            CCString * frameKey = new CCString(spriteFrameName);

            CCObject* pObj = NULL;
            CCARRAY_FOREACH(aliases, pObj)
            {
                std::string oneAlias = ((CCString*)pObj)->getCString();
                if (m_pSpriteFramesAliases->objectForKey(oneAlias.c_str()))
                {
                    CCLOGWARN("cocos2d: WARNING: an alias with name %s already exists", oneAlias.c_str());
                }

                m_pSpriteFramesAliases->setObject(frameKey, oneAlias.c_str());
            }
            frameKey->release();
            // create frame
            spriteFrame = new CCSpriteFrame();
            spriteFrame->initWithTexture(pobTexture,
                                         CCRectMake(textureRect.origin.x, textureRect.origin.y, spriteSize.width, spriteSize.height),
                                         textureRotated,
                                         spriteOffset,
                                         spriteSourceSize);
        }

        // add sprite frame
        m_pSpriteFrames->setObject(spriteFrame, spriteFrameName);
        spriteFrame->release();
    }
}

void CCSpriteFrameCache::addSpriteFramesWithFile(const char *pszPlist, CCTexture2D *pobTexture)
{
    if (m_pLoadedFileNames->find(pszPlist) != m_pLoadedFileNames->end())
    {
        return;//We already added it
    }
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszPlist);
    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());

    addSpriteFramesWithDictionary(dict, pobTexture);
    m_pLoadedFileNames->insert(pszPlist);

    dict->release();
}

void CCSpriteFrameCache::addSpriteFramesWithFile(const char* plist, const char* textureFileName)
{
    CCAssert(textureFileName, "texture name should not be null");
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(textureFileName);

    if (texture)
    {
        addSpriteFramesWithFile(plist, texture);
    }
    else
    {
        CCLOG("cocos2d: CCSpriteFrameCache: couldn't load texture file. File not found %s", textureFileName);
    }
}

void CCSpriteFrameCache::addSpriteFramesWithFile(const char *pszPlist)
{
    CCAssert(pszPlist, "plist filename should not be NULL");

    if (m_pLoadedFileNames->find(pszPlist) == m_pLoadedFileNames->end())
    {
        std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszPlist);
        CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());

        string texturePath("");

        CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
        if (metadataDict)
        {
            // try to read  texture file name from meta data
            texturePath = metadataDict->valueForKey("textureFileName")->getCString();
        }

        if (! texturePath.empty())
        {
            // build texture path relative to plist file
            texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(texturePath.c_str(), pszPlist);
        }
        else
        {
            // build texture path by replacing file extension
            texturePath = pszPlist;

            // remove .xxx
            size_t startPos = texturePath.find_last_of(".");
            texturePath = texturePath.erase(startPos);

            // append .png
            texturePath = texturePath.append(".png");

            CCLOG("cocos2d: CCSpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
        }

        CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str());

        if (pTexture)
        {
            addSpriteFramesWithDictionary(dict, pTexture);
            m_pLoadedFileNames->insert(pszPlist);
        }
        else
        {
            CCLOG("cocos2d: CCSpriteFrameCache: Couldn't load texture");
        }

        dict->release();
    }

}

void CCSpriteFrameCache::addSpriteFrame(CCSpriteFrame *pobFrame, const char *pszFrameName)
{
    m_pSpriteFrames->setObject(pobFrame, pszFrameName);
}

void CCSpriteFrameCache::removeSpriteFrames(void)
{
    m_pSpriteFrames->removeAllObjects();
    m_pSpriteFramesAliases->removeAllObjects();
    m_pLoadedFileNames->clear();
}

void CCSpriteFrameCache::removeUnusedSpriteFrames(void)
{
    bool bRemoved = false;
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pSpriteFrames, pElement)
    {
        CCSpriteFrame* spriteFrame = (CCSpriteFrame*)pElement->getObject();
        if( spriteFrame->retainCount() == 1 )
        {
            CCLOG("cocos2d: CCSpriteFrameCache: removing unused frame: %s", pElement->getStrKey());
            m_pSpriteFrames->removeObjectForElememt(pElement);
            bRemoved = true;
        }
    }

    // XXX. Since we don't know the .plist file that originated the frame, we must remove all .plist from the cache
    if( bRemoved )
    {
        m_pLoadedFileNames->clear();
    }
}


void CCSpriteFrameCache::removeSpriteFrameByName(const char *pszName)
{
    // explicit nil handling
    if( ! pszName )
    {
        return;
    }

    // Is this an alias ?
    CCString* key = (CCString*)m_pSpriteFramesAliases->objectForKey(pszName);

    if (key)
    {
        m_pSpriteFrames->removeObjectForKey(key->getCString());
        m_pSpriteFramesAliases->removeObjectForKey(key->getCString());
    }
    else
    {
        m_pSpriteFrames->removeObjectForKey(pszName);
    }

    // XXX. Since we don't know the .plist file that originated the frame, we must remove all .plist from the cache
    m_pLoadedFileNames->clear();
}

void CCSpriteFrameCache::removeSpriteFramesFromFile(const char* plist)
{
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(plist);
    CCDictionary* dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());

    removeSpriteFramesFromDictionary((CCDictionary*)dict);

    // remove it from the cache
    set<string>::iterator ret = m_pLoadedFileNames->find(plist);
    if (ret != m_pLoadedFileNames->end())
    {
        m_pLoadedFileNames->erase(ret);
    }

    dict->release();
}

void CCSpriteFrameCache::removeSpriteFramesFromDictionary(CCDictionary* dictionary)
{
    CCDictionary* framesDict = (CCDictionary*)dictionary->objectForKey("frames");
    CCArray* keysToRemove = CCArray::create();

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(framesDict, pElement)
    {
        if (m_pSpriteFrames->objectForKey(pElement->getStrKey()))
        {
            keysToRemove->addObject(CCString::create(pElement->getStrKey()));
        }
    }

    m_pSpriteFrames->removeObjectsForKeys(keysToRemove);
}

void CCSpriteFrameCache::removeSpriteFramesFromTexture(CCTexture2D* texture)
{
    CCArray* keysToRemove = CCArray::create();

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(m_pSpriteFrames, pElement)
    {
        string key = pElement->getStrKey();
        CCSpriteFrame* frame = (CCSpriteFrame*)m_pSpriteFrames->objectForKey(key.c_str());
        if (frame && (frame->getTexture() == texture))
        {
            keysToRemove->addObject(CCString::create(pElement->getStrKey()));
        }
    }

    m_pSpriteFrames->removeObjectsForKeys(keysToRemove);
}

CCSpriteFrame* CCSpriteFrameCache::spriteFrameByName(const char *pszName)
{
    CCAssert(pszName, "CCSpriteFrameCache::spriteFrameByName() - invalid pszName");
    CCSpriteFrame* frame = (CCSpriteFrame*)m_pSpriteFrames->objectForKey(pszName);
    if (!frame)
    {
        // try alias dictionary
        CCString *key = (CCString*)m_pSpriteFramesAliases->objectForKey(pszName);  
        if (key)
        {
            frame = (CCSpriteFrame*)m_pSpriteFrames->objectForKey(key->getCString());
            if (! frame)
            {
                CCLOG("cocos2d: CCSpriteFrameCache: Frame '%s' not found", pszName);
            }
        }
    }
    return frame;
}

NS_CC_END
