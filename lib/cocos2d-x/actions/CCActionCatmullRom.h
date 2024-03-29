/*
 * Copyright (c) 2012 cocos2d-x.org
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


#ifndef __CCACTION_CATMULLROM_H__
#define __CCACTION_CATMULLROM_H__

#include <vector>

#include "CCActionInterval.h"
#include "nodes/CCNode.h"
#include "cocoa/CCGeometry.h"

NS_CC_BEGIN;

/**
 * @addtogroup actions
 * @{
 */

/** Cardinal Spline path.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
@ingroup Actions
 */
class CC_DLL CCCardinalSplineTo : public CCActionInterval
{
    CCOBJECT(CCCardinalSplineTo, CCActionInterval)
public:

    /** creates an action with a Cardinal Spline array of points and tension 
     *  @code
     *  when thie function bound to js,the input params are changed
     *  js:var create(var duration,var pointTable,var tension)
     *  @endcode
     */
    static CCCardinalSplineTo* create(float duration, const std::vector<Vec2>& points, float tension);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCCardinalSplineTo();
    /**
     *  @js ctor
     *  @lua NA
     */
    CCCardinalSplineTo();
    
    /** initializes the action with a duration and an array of points 
     *  @lua NA
     */
    bool initWithDuration(float duration, const std::vector<Vec2>& points, float tension);
    
    /**
     *  @js NA
     *  @lua NA
     */
	virtual void paste(CCObject* o);
	/**
     *  @lua NA
     */
    virtual void startWithTarget(CCNode *pTarget);
    /**
     *  @lua NA
     */
    virtual void update(float time);
    virtual CCActionInterval* reverse();
    /**
     *  @lua NA
     */
    virtual void updatePosition(Vec2 &newPos);
    
    inline const std::vector<Vec2>& getPoints() { return _points; }
    /**
     *  @js NA
     */
    inline void  setPoints(const std::vector<Vec2>& points)
    {
        _points = points;
    }
    
protected:
	std::vector<Vec2> _points;
    float m_fDeltaT;
    float m_fTension;
    Vec2	m_previousPosition;
    Vec2	m_accumulatedDiff;
};

/** Cardinal Spline path.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
 @ingroup Actions
 */
class CC_DLL CCCardinalSplineBy : public CCCardinalSplineTo 
{
public:
    
    /** creates an action with a Cardinal Spline array of points and tension 
     *  @code
     *  when thie function bound to js,the input params are changed
     *  js:var create(var duration,var pointTable,var tension)
     *  @endcode
     */
    static CCCardinalSplineBy* create(float duration, const std::vector<Vec2>& points, float tension);
    /**
     *  @js NA
     *  @lua NA
     */
    CCCardinalSplineBy();
    /**
     *  @lua NA
     */
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCActionInterval* reverse();
    /**
     *  @lua NA
     */
    virtual void updatePosition(Vec2 &newPos);
protected:
    Vec2 m_startPosition;
};

/** An action that moves the target with a CatmullRom curve to a destination point.
 A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 @ingroup Actions
 */
class CC_DLL CCCatmullRomTo : public CCCardinalSplineTo
{
    CCOBJECT(CCCatmullRomTo, CCCardinalSplineTo)
public:
    
    /** creates an action with a Cardinal Spline array of points and tension 
     *  @code
     *  when this function bound to js,the input params are changed
     *  js:var create(var dt,var pointTable)
     *  @endcode
     */
    static CCCatmullRomTo* create(float dt, const std::vector<Vec2>& points);

    /** initializes the action with a duration and an array of points 
     *  @js NA
     *  @lua NA
     */
    bool initWithDuration(float dt, const std::vector<Vec2>& points);
};

/** An action that moves the target with a CatmullRom curve by a certain distance.
 A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 @ingroup Actions
 */
class CC_DLL CCCatmullRomBy : public CCCardinalSplineBy
{
    CCOBJECT(CCCatmullRomBy, CCCardinalSplineBy)
public:
    
    /** creates an action with a Cardinal Spline array of points and tension 
     *  @code
     *  when this function bound to js,the input params are changed
     *  js:var create(var dt,var pointTable)
     *  @endcode
     */
    static CCCatmullRomBy* create(float dt, const std::vector<Vec2>& points);

    /** initializes the action with a duration and an array of points 
     *  @js NA
     *  @lua NA
     */
    bool initWithDuration(float dt, const std::vector<Vec2>& points);
};

/** Returns the Cardinal Spline position for a given set of control points, tension and time */
extern CC_DLL Vec2 ccCardinalSplineAt(Vec2 &p0, Vec2 &p1, Vec2 &p2, Vec2 &p3, float tension, float t);

// end of actions group
/// @}

NS_CC_END;

#endif // __CCACTION_CATMULLROM_H__
