/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2010      Ricardo Quesada
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
#include "CCAnimationCache.h"
#include "ccMacros.h"
#include "CCAnimation.h"
#include "CCSpriteFrame.h"
#include "cocoa/CCString.h"
#include "IO/FileSystem.h"

using namespace std;

NS_CC_BEGIN

SharedPtr<CCAnimationCache> s_pSharedAnimationCache;

CCAnimationCache* CCAnimationCache::sharedAnimationCache(void)
{
    if (! s_pSharedAnimationCache)
    {
        s_pSharedAnimationCache = new CCAnimationCache();
        s_pSharedAnimationCache->init();
    }

    return s_pSharedAnimationCache;
}

void CCAnimationCache::purgeSharedAnimationCache(void)
{
	s_pSharedAnimationCache.Reset();
}

bool CCAnimationCache::init()
{
    m_pAnimations = new CCDictionary();
    return true;
}

CCAnimationCache::CCAnimationCache()
{
}

CCAnimationCache::~CCAnimationCache()
{
}

void CCAnimationCache::addAnimation(CCAnimation *animation, const char * name)
{
    m_pAnimations->setObject(animation, name);
}

void CCAnimationCache::removeAnimationByName(const char* name)
{
    if (! name)
    {
        return;
    }

    m_pAnimations->removeObjectForKey(name);
}

CCAnimation* CCAnimationCache::animationByName(const char* name)
{
    return (CCAnimation*)m_pAnimations->objectForKey(name);
}



NS_CC_END