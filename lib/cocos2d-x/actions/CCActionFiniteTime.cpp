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

#include "CCActionFiniteTime.h"
#include "CCActionInterval.h"
#include "nodes/CCNode.h"
#include "support/CCPointExtension.h"
#include "engine/CCDirector.h"

NS_CC_BEGIN

//
// FiniteTimeAction
//

CCFiniteTimeAction *CCFiniteTimeAction::reverse()
{
    CCLOG("cocos2d: FiniteTimeAction#reverse: Implement me");
    return NULL;
}

//
// Speed
//
CCSpeed::~CCSpeed()
{
    CC_SAFE_RELEASE(m_pInnerAction);
}

CCSpeed* CCSpeed::create(CCActionInterval* pAction, float fSpeed)
{
    CCSpeed *pRet = new CCSpeed();
    if (pRet && pRet->initWithAction(pAction, fSpeed))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCSpeed::initWithAction(CCActionInterval *pAction, float fSpeed)
{
    CCAssert(pAction != NULL, "");
    pAction->retain();
    m_pInnerAction = pAction;
    m_fSpeed = fSpeed;    
    return true;
}

void CCSpeed::paste(CCObject* o)
{
    Super::paste(o);
	dynamic_cast<SelfType*>(o)->initWithAction( (CCActionInterval*)(m_pInnerAction->copy()) , m_fSpeed );
}

void CCSpeed::startWithTarget(CCNode* pTarget)
{
    CCAction::startWithTarget(pTarget);
    m_pInnerAction->startWithTarget(pTarget);
}

void CCSpeed::stop()
{
    m_pInnerAction->stop();
    CCAction::stop();
}

void CCSpeed::step(float dt)
{
    m_pInnerAction->step(dt * m_fSpeed);
}

bool CCSpeed::isDone()
{
    return m_pInnerAction->isDone();
}

CCActionInterval *CCSpeed::reverse()
{
     return (CCActionInterval*)(CCSpeed::create(m_pInnerAction->reverse(), m_fSpeed));
}

void CCSpeed::setInnerAction(CCActionInterval *pAction)
{
    if (m_pInnerAction != pAction)
    {
        CC_SAFE_RELEASE(m_pInnerAction);
        m_pInnerAction = pAction;
        CC_SAFE_RETAIN(m_pInnerAction);
    }
}

//
// Follow
//
CCFollow::~CCFollow()
{
    CC_SAFE_RELEASE(m_pobFollowedNode);
}

CCFollow* CCFollow::create(CCNode *pFollowedNode, const CCRect& rect/* = CCRectZero*/)
{
    CCFollow *pRet = new CCFollow();
    if (pRet && pRet->initWithTarget(pFollowedNode, rect))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCFollow::initWithTarget(CCNode *pFollowedNode, const CCRect& rect/* = CCRectZero*/)
{
    CCAssert(pFollowedNode != NULL, "");
 
    pFollowedNode->retain();
    m_pobFollowedNode = pFollowedNode;
    if (rect.equals(CCRectZero))
    {
        m_bBoundarySet = false;
    }
    else
    {
        m_bBoundarySet = true;
    }
    
    m_bBoundaryFullyCovered = false;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_obFullScreenSize = CCPointMake(winSize.width, winSize.height);
    m_obHalfScreenSize = ccpMult(m_obFullScreenSize, 0.5f);

    if (m_bBoundarySet)
    {
        m_fLeftBoundary = -((rect.origin.x+rect.size.width) - m_obFullScreenSize.x);
        m_fRightBoundary = -rect.origin.x ;
        m_fTopBoundary = -rect.origin.y;
        m_fBottomBoundary = -((rect.origin.y+rect.size.height) - m_obFullScreenSize.y);

        if(m_fRightBoundary < m_fLeftBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            m_fRightBoundary = m_fLeftBoundary = (m_fLeftBoundary + m_fRightBoundary) / 2;
        }
        if(m_fTopBoundary < m_fBottomBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            m_fTopBoundary = m_fBottomBoundary = (m_fTopBoundary + m_fBottomBoundary) / 2;
        }

        if( (m_fTopBoundary == m_fBottomBoundary) && (m_fLeftBoundary == m_fRightBoundary) )
        {
            m_bBoundaryFullyCovered = true;
        }
    }
    
    return true;
}

void CCFollow::step(float dt)
{
    CC_UNUSED_PARAM(dt);

    if(m_bBoundarySet)
    {
        // whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
        if(m_bBoundaryFullyCovered)
            return;

        Vec2 tempPos = ccpSub( m_obHalfScreenSize, m_pobFollowedNode->getPosition());

        m_pTarget->setPosition(ccp(clampf(tempPos.x, m_fLeftBoundary, m_fRightBoundary), 
                                   clampf(tempPos.y, m_fBottomBoundary, m_fTopBoundary)));
    }
    else
    {
        m_pTarget->setPosition(ccpSub(m_obHalfScreenSize, m_pobFollowedNode->getPosition()));
    }
}

bool CCFollow::isDone()
{
    return ( !m_pobFollowedNode->isRunning() );
}

void CCFollow::stop()
{
    m_pTarget = NULL;
    CCAction::stop();
}

NS_CC_END


