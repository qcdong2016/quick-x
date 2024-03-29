/*
 * Copyright (c) 2010-2012 cocos2d-x.org
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2008 Radu Gruian
 *
 * Copyright (c) 2011 Vit Valentin
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * Original code by Radu Gruian: http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio.So
 *
 * Adapted to cocos2d-x by Vit Valentin
 *
 * Adapted from cocos2d-x to cocos2d-iphone by Ricardo Quesada
 */
#include "ccMacros.h"
#include "CCActionCatmullRom.h"
//#include <xutility>

using namespace std;

NS_CC_BEGIN;

// CatmullRom Spline formula:
Vec2 ccCardinalSplineAt(Vec2 &p0, Vec2 &p1, Vec2 &p2, Vec2 &p3, float tension, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    
	/*
	 * Formula: s(-ttt + 2tt - t)P1 + s(-ttt + tt)P2 + (2ttt - 3tt + 1)P2 + s(ttt - 2tt + t)P3 + (-2ttt + 3tt)P3 + s(ttt - tt)P4
	 */
    float s = (1 - tension) / 2;
	
    float b1 = s * ((-t3 + (2 * t2)) - t);                      // s(-t3 + 2 t2 - t)P1
    float b2 = s * (-t3 + t2) + (2 * t3 - 3 * t2 + 1);          // s(-t3 + t2)P2 + (2 t3 - 3 t2 + 1)P2
    float b3 = s * (t3 - 2 * t2 + t) + (-2 * t3 + 3 * t2);      // s(t3 - 2 t2 + t)P3 + (-2 t3 + 3 t2)P3
    float b4 = s * (t3 - t2);                                   // s(t3 - t2)P4
    
    float x = (p0.x*b1 + p1.x*b2 + p2.x*b3 + p3.x*b4);
    float y = (p0.y*b1 + p1.y*b2 + p2.y*b3 + p3.y*b4);
	
	return ccp(x,y);
}

/* Implementation of CCCardinalSplineTo
 */

CCCardinalSplineTo* CCCardinalSplineTo::create(float duration, const std::vector<Vec2>& points, float tension)
{
    CCCardinalSplineTo *ret = new CCCardinalSplineTo();
    if (ret)
    {
        if (ret->initWithDuration(duration, points, tension))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

bool CCCardinalSplineTo::initWithDuration(float duration, const std::vector<Vec2>& points, float tension)
{
    CCAssert(points.size() > 0, "Invalid configuration. It must at least have one control point");

    if (CCActionInterval::initWithDuration(duration))
    {
        this->setPoints(points);
        this->m_fTension = tension;
        
        return true;
    }
    
    return false;
}

CCCardinalSplineTo::~CCCardinalSplineTo()
{
}

CCCardinalSplineTo::CCCardinalSplineTo()
: m_fDeltaT(0.f)
, m_fTension(0.f)
{
}

void CCCardinalSplineTo::startWithTarget(cocos2d::CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
	
//    m_fDeltaT = (float) 1 / _points->count();
    
    // Issue #1441
    m_fDeltaT = (float) 1 / (_points.size() - 1);

    m_previousPosition = pTarget->getPosition();
    m_accumulatedDiff = CCPointZero;
}

void CCCardinalSplineTo::paste(CCObject* o)
{
	Super::paste(o);
    dynamic_cast<SelfType*>(o)->initWithDuration(this->getDuration(), this->_points, this->m_fTension);
}

void CCCardinalSplineTo::update(float time)
{
    unsigned int p;
    float lt;
	
	// eg.
	// p..p..p..p..p..p..p
	// 1..2..3..4..5..6..7
	// want p to be 1, 2, 3, 4, 5, 6
    if (time == 1)
    {
        p = _points.size() - 1;
        lt = 1;
    }
    else 
    {
        p = time / m_fDeltaT;
        lt = (time - m_fDeltaT * (float)p) / m_fDeltaT;
    }
    
	// Interpolate    
    Vec2 pp0 = _points[p-1];
    Vec2 pp1 = _points[p+0];
    Vec2 pp2 = _points[p+1];
    Vec2 pp3 = _points[p+2];
	
    Vec2 newPos = ccCardinalSplineAt(pp0, pp1, pp2, pp3, m_fTension, lt);
	
#if CC_ENABLE_STACKABLE_ACTIONS
    // Support for stacked actions
    CCNode *node = m_pTarget;
    Vec2 diff = ccpSub( node->getPosition(), m_previousPosition);
    if( diff.x !=0 || diff.y != 0 ) {
        m_accumulatedDiff = ccpAdd( m_accumulatedDiff, diff);
        newPos = ccpAdd( newPos, m_accumulatedDiff);
    }
#endif
    
    this->updatePosition(newPos);
}

void CCCardinalSplineTo::updatePosition(cocos2d::Vec2 &newPos)
{
    m_pTarget->setPosition(newPos);
    m_previousPosition = newPos;
}

CCActionInterval* CCCardinalSplineTo::reverse()
{
	std::vector<Vec2> reverse;
	for (auto i = _points.rbegin(); i != _points.rend(); i++)
		reverse.push_back(*i);
    return CCCardinalSplineTo::create(m_fDuration, reverse, m_fTension);
}

/* CCCardinalSplineBy
 */

CCCardinalSplineBy* CCCardinalSplineBy::create(float duration, const std::vector<Vec2>& points, float tension)
{
    CCCardinalSplineBy *ret = new CCCardinalSplineBy();
    if (ret)
    {
        if (ret->initWithDuration(duration, points, tension))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

CCCardinalSplineBy::CCCardinalSplineBy() : m_startPosition(0,0)
{
}

void CCCardinalSplineBy::updatePosition(cocos2d::Vec2 &newPos)
{
    Vec2 p = ccpAdd(newPos, m_startPosition);
    m_pTarget->setPosition(p);
    m_previousPosition = p;
}

CCActionInterval* CCCardinalSplineBy::reverse()
{
	std::vector<Vec2> points = _points;
	//
	// convert "absolutes" to "diffs"
	//
	Vec2 p = points[0];
    for (unsigned int i = 1; i < points.size(); ++i)
    {
        Vec2 current = points[i];
        Vec2 diff = ccpSub(current, p);
		points[i] = diff;
        
        p = current;
    }
	
	
	// convert to "diffs" to "reverse absolute"
	std::vector<Vec2> reverse;
	for (auto i = _points.rbegin(); i != _points.rend(); i++)
		reverse.push_back(*i);
	
	// 1st element (which should be 0,0) should be here too
    
	p = reverse[reverse.size() - 1];
	reverse.pop_back();
    
    p = ccpNeg(p);
	reverse.insert(reverse.begin(), p);
    
    for (unsigned int i = 1; i < reverse.size(); ++i)
    {
        Vec2 current = reverse[i];
        current = ccpNeg(current);
        Vec2 abs = ccpAdd(current, p);
		reverse[i] = abs;
        
        p = abs;
    }
	
    return CCCardinalSplineBy::create(m_fDuration, reverse, m_fTension);
}

void CCCardinalSplineBy::startWithTarget(cocos2d::CCNode *pTarget)
{    
    CCCardinalSplineTo::startWithTarget(pTarget);
    m_startPosition = pTarget->getPosition();
}

/* CCCatmullRomTo
 */

CCCatmullRomTo* CCCatmullRomTo::create(float dt, const std::vector<Vec2>& points)
{
    CCCatmullRomTo *ret = new CCCatmullRomTo();
    if (ret)
    {
        if (ret->initWithDuration(dt, points))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

bool CCCatmullRomTo::initWithDuration(float dt, const std::vector<Vec2>& points)
{
    if (CCCardinalSplineTo::initWithDuration(dt, points, 0.5f))
    {
        return true;
    }
    
    return false;
}

/* CCCatmullRomBy
 */

CCCatmullRomBy* CCCatmullRomBy::create(float dt, const std::vector<Vec2>& points)
{
    CCCatmullRomBy *ret = new CCCatmullRomBy();
    if (ret)
    {
        if (ret->initWithDuration(dt, points))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

bool CCCatmullRomBy::initWithDuration(float dt, const std::vector<Vec2>& points)
{
    if (CCCardinalSplineTo::initWithDuration(dt, points, 0.5f))
    {
        return true;
    }
    
    return false;
}

NS_CC_END;

