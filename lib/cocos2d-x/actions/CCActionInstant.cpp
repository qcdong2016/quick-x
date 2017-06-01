/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2011 Zynga Inc.
 
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

#include "CCActionInstant.h"
#include "nodes/CCNode.h"
#include "nodes/CCSprite.h"
#include "cocoa/CCScriptSupport.h"

NS_CC_BEGIN
//
// InstantAction
//
CCActionInstant::CCActionInstant() {
}

bool CCActionInstant::isDone() {
    return true;
}

void CCActionInstant::step(float dt) {
    CC_UNUSED_PARAM(dt);
    update(1);
}

void CCActionInstant::update(float time) {
    CC_UNUSED_PARAM(time);
    // nothing
}

CCFiniteTimeAction * CCActionInstant::reverse() {
    return (CCFiniteTimeAction*) (copy()->autorelease());
}

//
// Show
//

CCShow* CCShow::create() 
{
    CCShow* pRet = new CCShow();

    if (pRet) {
        pRet->autorelease();
    }

    return pRet;
}

void CCShow::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(true);
}

CCFiniteTimeAction* CCShow::reverse() {
    return (CCFiniteTimeAction*) (CCHide::create());
}

//
// Hide
//
CCHide * CCHide::create() 
{
    CCHide *pRet = new CCHide();

    if (pRet) {
        pRet->autorelease();
    }

    return pRet;
}

void CCHide::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(false);
}

CCFiniteTimeAction *CCHide::reverse() {
    return (CCFiniteTimeAction*) (CCShow::create());
}

//
// ToggleVisibility
//
CCToggleVisibility * CCToggleVisibility::create()
{
    CCToggleVisibility *pRet = new CCToggleVisibility();

    if (pRet)
    {
        pRet->autorelease();
    }

    return pRet;
}

void CCToggleVisibility::update(float time) 
{
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(!m_pTarget->isVisible());
}

//
// Remove Self
//
CCRemoveSelf * CCRemoveSelf::create(bool isNeedCleanUp /*= true*/) 
{
	CCRemoveSelf *pRet = new CCRemoveSelf();

	if (pRet && pRet->init(isNeedCleanUp)) {
		pRet->autorelease();
	}

	return pRet;
}

bool CCRemoveSelf::init(bool isNeedCleanUp) {
	m_bIsNeedCleanUp = isNeedCleanUp;
	return true;
}

void CCRemoveSelf::update(float time) {
	CC_UNUSED_PARAM(time);
	m_pTarget->removeFromParentAndCleanup(m_bIsNeedCleanUp);
}

CCFiniteTimeAction *CCRemoveSelf::reverse() {
	return (CCFiniteTimeAction*) (CCRemoveSelf::create(m_bIsNeedCleanUp));
}

void CCRemoveSelf::paste(CCObject* o) {
    Super::paste(o);
	dynamic_cast<SelfType*>(o)->init(m_bIsNeedCleanUp);
}

//
// FlipX
//

CCFlipX *CCFlipX::create(bool x)
{
    CCFlipX *pRet = new CCFlipX();

    if (pRet && pRet->initWithFlipX(x)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCFlipX::initWithFlipX(bool x) {
    m_bFlipX = x;
    return true;
}

void CCFlipX::update(float time) {
    CC_UNUSED_PARAM(time);
    ((CCSprite*) (m_pTarget))->setFlipX(m_bFlipX);
}

CCFiniteTimeAction* CCFlipX::reverse() {
    return CCFlipX::create(!m_bFlipX);
}

void CCFlipX::paste(CCObject* o) {
    Super::paste(o);
	dynamic_cast<SelfType*>(o)->initWithFlipX(m_bFlipX);
}

//
// FlipY
//

CCFlipY * CCFlipY::create(bool y)
{
    CCFlipY *pRet = new CCFlipY();

    if (pRet && pRet->initWithFlipY(y)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCFlipY::initWithFlipY(bool y) {
    m_bFlipY = y;
    return true;
}

void CCFlipY::update(float time) {
    CC_UNUSED_PARAM(time);
    ((CCSprite*) (m_pTarget))->setFlipY(m_bFlipY);
}

CCFiniteTimeAction* CCFlipY::reverse() {
    return CCFlipY::create(!m_bFlipY);
}

void CCFlipY::paste(CCObject* o) {
    Super::paste(o);
	dynamic_cast<SelfType*>(o)->initWithFlipY(m_bFlipY);
}

//
// Place
//

CCPlace* CCPlace::create(const Vec2& pos)
{
    CCPlace *pRet = new CCPlace();

    if (pRet && pRet->initWithPosition(pos)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCPlace::initWithPosition(const Vec2& pos) {
    m_tPosition = pos;
    return true;
}

void CCPlace::paste(CCObject* o) {
    Super::paste(o);
	dynamic_cast<SelfType*>(o)->initWithPosition(m_tPosition);
}

void CCPlace::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->setPosition(m_tPosition);
}

//
// CallFunc
//
CCCallFunc * CCCallFunc::create(EventHandler* handler) 
{
    CCCallFunc *pRet = new CCCallFunc();

    if (pRet) {
        pRet->_handler = handler;
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}


CCCallFunc::~CCCallFunc(void)
{
	CC_SAFE_DELETE(_handler);
}

void CCCallFunc::paste(CCObject* o) {
    Super::paste(o);
	CCCallFunc* cf = static_cast<SelfType*>(o);
	cf->_handler = _handler->clone();
}

void CCCallFunc::update(float time) {
    CC_UNUSED_PARAM(time);
    this->execute();
}

void CCCallFunc::execute() {
	_handler->invoke();
}


NS_CC_END
