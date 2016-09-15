/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2009 Jason Booth

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

/*
 * Elastic, Back and Bounce actions based on code from:
 * http://github.com/NikhilK/silverlightfx/
 *
 * by http://github.com/NikhilK
 */

#include "CCActionEase.h"

NS_CC_BEGIN

#ifndef M_PI_X_2
#define M_PI_X_2 (float)M_PI * 2.0f
#endif

//
// EaseAction
//

CCActionEase* CCActionEase::create(CCActionInterval *pAction)
{
    CCActionEase *pRet = new CCActionEase();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

bool CCActionEase::initWithAction(CCActionInterval *pAction)
{
    CCAssert(pAction != NULL, "");

    if (CCActionInterval::initWithDuration(pAction->getDuration()))
    {
        m_pInner = pAction;
        pAction->retain();

        return true;
    }

    return false;
}

void CCActionEase::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

CCActionEase::~CCActionEase(void)
{
    CC_SAFE_RELEASE(m_pInner);
}

void CCActionEase::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_pInner->startWithTarget(m_pTarget);
}

void CCActionEase::stop(void)
{
    m_pInner->stop();
    CCActionInterval::stop();
}

void CCActionEase::update(float time)
{
    m_pInner->update(time);
}

CCActionInterval* CCActionEase::reverse(void)
{
    return CCActionEase::create(m_pInner->reverse());
}

CCActionInterval* CCActionEase::getInnerAction()
{
    return m_pInner;
}

//
// EaseRateAction
//

CCEaseRateAction* CCEaseRateAction::create(CCActionInterval *pAction, float fRate)
{
    CCEaseRateAction *pRet = new CCEaseRateAction();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fRate))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

bool CCEaseRateAction::initWithAction(CCActionInterval *pAction, float fRate)
{
    if (CCActionEase::initWithAction(pAction))
    {
        m_fRate = fRate;
        return true;
    }

    return false;
}

void CCEaseRateAction::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval*)(m_pInner->copy()), m_fRate);
}

CCEaseRateAction::~CCEaseRateAction(void)
{
}

CCActionInterval* CCEaseRateAction::reverse(void)
{
    return CCEaseRateAction::create(m_pInner->reverse(), 1 / m_fRate);
}

//
// EeseIn
//

CCEaseIn* CCEaseIn::create(CCActionInterval *pAction, float fRate)
{
    CCEaseIn *pRet = new CCEaseIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fRate))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

void CCEaseIn::paste(CCObject* o)
{
    Super::paste(o);
	O->initWithAction((CCActionInterval*)(m_pInner->copy()), m_fRate);
}

void CCEaseIn::update(float time)
{
    m_pInner->update(powf(time, m_fRate));
}

CCActionInterval* CCEaseIn::reverse(void)
{
    return CCEaseIn::create(m_pInner->reverse(), 1 / m_fRate);
}

//
// EaseOut
//
CCEaseOut* CCEaseOut::create(CCActionInterval *pAction, float fRate)
{
    CCEaseOut *pRet = new CCEaseOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fRate))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;   
}

void CCEaseOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval*)(m_pInner->copy()), m_fRate);
}

void CCEaseOut::update(float time)
{
    m_pInner->update(powf(time, 1 / m_fRate));
}

CCActionInterval* CCEaseOut::reverse()
{
    return CCEaseOut::create(m_pInner->reverse(), 1 / m_fRate);
}

//
// EaseInOut
//
CCEaseInOut* CCEaseInOut::create(CCActionInterval *pAction, float fRate)
{
    CCEaseInOut *pRet = new CCEaseInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fRate))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseInOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval*)(m_pInner->copy()), m_fRate);
}

void CCEaseInOut::update(float time)
{
    time *= 2;
    if (time < 1)
    {
        m_pInner->update(0.5f * powf(time, m_fRate));
    }
    else
    {
        m_pInner->update(1.0f - 0.5f * powf(2-time, m_fRate));
    }
}

// InOut and OutIn are symmetrical
CCActionInterval* CCEaseInOut::reverse(void)
{
    return CCEaseInOut::create(m_pInner->reverse(), m_fRate);
}

//
// EaseExponentialIn
//
CCEaseExponentialIn* CCEaseExponentialIn::create(CCActionInterval* pAction)
{
    CCEaseExponentialIn *pRet = new CCEaseExponentialIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;    
}

void CCEaseExponentialIn::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseExponentialIn::update(float time)
{
    m_pInner->update(time == 0 ? 0 : powf(2, 10 * (time/1 - 1)) - 1 * 0.001f);
}

CCActionInterval* CCEaseExponentialIn::reverse(void)
{
    return CCEaseExponentialOut::create(m_pInner->reverse());
}

//
// EaseExponentialOut
//
CCEaseExponentialOut* CCEaseExponentialOut::create(CCActionInterval* pAction)
{
    CCEaseExponentialOut *pRet = new CCEaseExponentialOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseExponentialOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseExponentialOut::update(float time)
{
    m_pInner->update(time == 1 ? 1 : (-powf(2, -10 * time / 1) + 1));
}

CCActionInterval* CCEaseExponentialOut::reverse(void)
{
    return CCEaseExponentialIn::create(m_pInner->reverse());
}

//
// EaseExponentialInOut
//

CCEaseExponentialInOut* CCEaseExponentialInOut::create(CCActionInterval *pAction)
{
    CCEaseExponentialInOut *pRet = new CCEaseExponentialInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseExponentialInOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseExponentialInOut::update(float time)
{
    time /= 0.5f;
    if (time < 1)
    {
        time = 0.5f * powf(2, 10 * (time - 1));
    }
    else
    {
        time = 0.5f * (-powf(2, -10 * (time - 1)) + 2);
    }

    m_pInner->update(time);
}

CCActionInterval* CCEaseExponentialInOut::reverse()
{
    return CCEaseExponentialInOut::create(m_pInner->reverse());
}

//
// EaseSineIn
//

CCEaseSineIn* CCEaseSineIn::create(CCActionInterval* pAction)
{
    CCEaseSineIn *pRet = new CCEaseSineIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseSineIn::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseSineIn::update(float time)
{
    m_pInner->update(-1 * cosf(time * (float)M_PI_2) + 1);
}

CCActionInterval* CCEaseSineIn::reverse(void)
{
    return CCEaseSineOut::create(m_pInner->reverse());
}

//
// EaseSineOut
//

CCEaseSineOut* CCEaseSineOut::create(CCActionInterval* pAction)
{
    CCEaseSineOut *pRet = new CCEaseSineOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseSineOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseSineOut::update(float time)
{
    m_pInner->update(sinf(time * (float)M_PI_2));
}

CCActionInterval* CCEaseSineOut::reverse(void)
{
    return CCEaseSineIn::create(m_pInner->reverse());
}

//
// EaseSineInOut
//

CCEaseSineInOut* CCEaseSineInOut::create(CCActionInterval* pAction)
{
    CCEaseSineInOut *pRet = new CCEaseSineInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseSineInOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseSineInOut::update(float time)
{
    m_pInner->update(-0.5f * (cosf((float)M_PI * time) - 1));
}

CCActionInterval* CCEaseSineInOut::reverse()
{
    return CCEaseSineInOut::create(m_pInner->reverse());
}

//
// EaseElastic
//

CCEaseElastic* CCEaseElastic::create(CCActionInterval *pAction)
{
    return CCEaseElastic::create(pAction, 0.3f);
}

CCEaseElastic* CCEaseElastic::create(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    CCEaseElastic *pRet = new CCEaseElastic();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fPeriod))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

bool CCEaseElastic::initWithAction(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    if (CCActionEase::initWithAction(pAction))
    {
        m_fPeriod = fPeriod;
        return true;
    }

    return false;
}

void CCEaseElastic::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()), m_fPeriod);
}

CCActionInterval* CCEaseElastic::reverse(void)
{
    CCAssert(0, "Override me");

    return NULL;
}

//
// EaseElasticIn
//

CCEaseElasticIn* CCEaseElasticIn::create(CCActionInterval *pAction)
{
    return CCEaseElasticIn::create(pAction, 0.3f);
}

CCEaseElasticIn* CCEaseElasticIn::create(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    CCEaseElasticIn *pRet = new CCEaseElasticIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fPeriod))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseElasticIn::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()), m_fPeriod);
}

void CCEaseElasticIn::update(float time)
{
    float newT = 0;
    if (time == 0 || time == 1)
    {
        newT = time;
    }
    else
    {
        float s = m_fPeriod / 4;
        time = time - 1;
        newT = -powf(2, 10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod);
    }

    m_pInner->update(newT);
}

CCActionInterval* CCEaseElasticIn::reverse(void)
{
    return CCEaseElasticOut::create(m_pInner->reverse(), m_fPeriod);
}

//
// EaseElasticOut
//

CCEaseElasticOut* CCEaseElasticOut::create(CCActionInterval *pAction)
{
    return CCEaseElasticOut::create(pAction, 0.3f);
}

CCEaseElasticOut* CCEaseElasticOut::create(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    CCEaseElasticOut *pRet = new CCEaseElasticOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fPeriod))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseElasticOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()), m_fPeriod);
}

void CCEaseElasticOut::update(float time)
{
    float newT = 0;
    if (time == 0 || time == 1)
    {
        newT = time;
    }
    else
    {
        float s = m_fPeriod / 4;
        newT = powf(2, -10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod) + 1;
    }

    m_pInner->update(newT);
}

CCActionInterval* CCEaseElasticOut::reverse(void)
{
    return CCEaseElasticIn::create(m_pInner->reverse(), m_fPeriod);
}

//
// EaseElasticInOut
//

CCEaseElasticInOut* CCEaseElasticInOut::create(CCActionInterval *pAction)
{
    return CCEaseElasticInOut::create(pAction, 0.3f);
}

CCEaseElasticInOut* CCEaseElasticInOut::create(CCActionInterval *pAction, float fPeriod/* = 0.3f*/)
{
    CCEaseElasticInOut *pRet = new CCEaseElasticInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction, fPeriod))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseElasticInOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()), m_fPeriod);
}

void CCEaseElasticInOut::update(float time)
{
    float newT = 0;
    if (time == 0 || time == 1)
    {
        newT = time;
    }
    else
    {
        time = time * 2;
        if (! m_fPeriod)
        {
            m_fPeriod = 0.3f * 1.5f;
        }

        float s = m_fPeriod / 4;

        time = time - 1;
        if (time < 0)
        {
            newT = -0.5f * powf(2, 10 * time) * sinf((time -s) * M_PI_X_2 / m_fPeriod);
        }
        else
        {
            newT = powf(2, -10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod) * 0.5f + 1;
        }
    }

    m_pInner->update(newT);
}

CCActionInterval* CCEaseElasticInOut::reverse(void)
{
    return CCEaseElasticInOut::create(m_pInner->reverse(), m_fPeriod);
}

//
// EaseBounce
//

CCEaseBounce* CCEaseBounce::create(CCActionInterval* pAction)
{
    CCEaseBounce *pRet = new CCEaseBounce();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseBounce::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

float CCEaseBounce::bounceTime(float time)
{
    if (time < 1 / 2.75)
    {
        return 7.5625f * time * time;
    } else 
    if (time < 2 / 2.75)
    {
        time -= 1.5f / 2.75f;
        return 7.5625f * time * time + 0.75f;
    } else
    if(time < 2.5 / 2.75)
    {
        time -= 2.25f / 2.75f;
        return 7.5625f * time * time + 0.9375f;
    }

    time -= 2.625f / 2.75f;
    return 7.5625f * time * time + 0.984375f;
}

CCActionInterval* CCEaseBounce::reverse()
{
    return CCEaseBounce::create(m_pInner->reverse());
}

//
// EaseBounceIn
//

CCEaseBounceIn* CCEaseBounceIn::create(CCActionInterval* pAction)
{
    CCEaseBounceIn *pRet = new CCEaseBounceIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseBounceIn::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseBounceIn::update(float time)
{
    float newT = 1 - bounceTime(1 - time);
    m_pInner->update(newT);
}

CCActionInterval* CCEaseBounceIn::reverse(void)
{
    return CCEaseBounceOut::create(m_pInner->reverse());
}

//
// EaseBounceOut
//

CCEaseBounceOut* CCEaseBounceOut::create(CCActionInterval* pAction)
{
    CCEaseBounceOut *pRet = new CCEaseBounceOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseBounceOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseBounceOut::update(float time)
{
    float newT = bounceTime(time);
    m_pInner->update(newT);
}

CCActionInterval* CCEaseBounceOut::reverse(void)
{
    return CCEaseBounceIn::create(m_pInner->reverse());
}

//
// EaseBounceInOut
//

CCEaseBounceInOut* CCEaseBounceInOut::create(CCActionInterval* pAction)
{
    CCEaseBounceInOut *pRet = new CCEaseBounceInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet; 
}

void CCEaseBounceInOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseBounceInOut::update(float time)
{
    float newT = 0;
    if (time < 0.5f)
    {
        time = time * 2;
        newT = (1 - bounceTime(1 - time)) * 0.5f;
    }
    else
    {
        newT = bounceTime(time * 2 - 1) * 0.5f + 0.5f;
    }

    m_pInner->update(newT);
}

CCActionInterval* CCEaseBounceInOut::reverse()
{
    return CCEaseBounceInOut::create(m_pInner->reverse());
}

//
// EaseBackIn
//

CCEaseBackIn* CCEaseBackIn::create(CCActionInterval *pAction)
{
    CCEaseBackIn *pRet = new CCEaseBackIn();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

void CCEaseBackIn::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseBackIn::update(float time)
{
    float overshoot = 1.70158f;
    m_pInner->update(time * time * ((overshoot + 1) * time - overshoot));
}

CCActionInterval* CCEaseBackIn::reverse(void)
{
    return CCEaseBackOut::create(m_pInner->reverse());
}

//
// EaseBackOut
//

CCEaseBackOut* CCEaseBackOut::create(CCActionInterval* pAction)
{
    CCEaseBackOut *pRet = new CCEaseBackOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

void CCEaseBackOut::paste(CCObject* o)
{
    Super::paste(o);
    O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseBackOut::update(float time)
{
    float overshoot = 1.70158f;

    time = time - 1;
    m_pInner->update(time * time * ((overshoot + 1) * time + overshoot) + 1);
}

CCActionInterval* CCEaseBackOut::reverse(void)
{
    return CCEaseBackIn::create(m_pInner->reverse());
}

//
// EaseBackInOut
//

CCEaseBackInOut* CCEaseBackInOut::create(CCActionInterval* pAction)
{
    CCEaseBackInOut *pRet = new CCEaseBackInOut();
    if (pRet)
    {
        if (pRet->initWithAction(pAction))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

void CCEaseBackInOut::paste(CCObject* o)
{
	Super::paste(o);
	O->initWithAction((CCActionInterval *)(m_pInner->copy()));
}

void CCEaseBackInOut::update(float time)
{
    float overshoot = 1.70158f * 1.525f;

    time = time * 2;
    if (time < 1)
    {
        m_pInner->update((time * time * ((overshoot + 1) * time - overshoot)) / 2);
    }
    else
    {
        time = time - 2;
        m_pInner->update((time * time * ((overshoot + 1) * time + overshoot)) / 2 + 1);
    }
}

CCActionInterval* CCEaseBackInOut::reverse()
{
    return CCEaseBackInOut::create(m_pInner->reverse());
}

NS_CC_END
