/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
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

#include "CCAction.h"
#include "CCActionInterval.h"
#include "nodes/CCNode.h"
#include "support/CCPointExtension.h"
#include "engine/CCDirector.h"

NS_CC_BEGIN
//
// Action Base Class
//

CCAction::CCAction()
:m_pOriginalTarget(NULL)
,m_pTarget(NULL)
,m_nTag(kCCActionTagInvalid)
{
}

CCAction::~CCAction()
{
    CCLOGINFO("cocos2d: deallocing");
}

CCAction* CCAction::create()
{
    CCAction * pRet = new CCAction();
    pRet->autorelease();
    return pRet;
}

const char* CCAction::description()
{
    return CCString::createWithFormat("<CCAction | Tag = %d>", m_nTag)->getCString();
}

void CCAction::paste(CCObject* o)
{
    Super::paste(o);
    dynamic_cast<SelfType*>(o)->m_nTag = m_nTag;
}

void CCAction::startWithTarget(CCNode *aTarget)
{
    m_pOriginalTarget = m_pTarget = aTarget;
}

void CCAction::stop()
{
    m_pTarget = NULL;
}

bool CCAction::isDone()
{
    return true;
}

void CCAction::step(float dt)
{
    CC_UNUSED_PARAM(dt);
    CCLOG("[Action step]. override me");
}

void CCAction::update(float time)
{
    CC_UNUSED_PARAM(time);
    CCLOG("[Action update]. override me");
}

NS_CC_END


