/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

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

#ifndef __CC_TOUCH_H__
#define __CC_TOUCH_H__

#include "engine/CCObject.h"
#include "cocoa/CCGeometry.h"

NS_CC_BEGIN

/**
 * @addtogroup input
 * @{
 */
#define CCTOUCHBEGAN        0
#define CCTOUCHMOVED        1
#define CCTOUCHENDED        2
#define CCTOUCHCANCELLED    3
#define CCTOUCHADDED        4
#define CCTOUCHREMOVED      5
class CC_DLL CCTouch : public CCObject
{
public:
    /**
     * @js ctor
     */
    CCTouch()
    : m_nId(0)
    , m_startPointCaptured(false)
    {
    }

    /** returns the current touch location in OpenGL coordinates */
    Vec2 getLocation() const;
    /** returns the previous touch location in OpenGL coordinates */
    Vec2 getPreviousLocation() const;
    /** returns the start touch location in OpenGL coordinates */
    Vec2 getStartLocation() const;
    /** returns the delta of 2 current touches locations in screen coordinates */
    Vec2 getDelta() const;
    /** returns the current touch location in screen coordinates */
    Vec2 getLocationInView() const;
    /** returns the previous touch location in screen coordinates */
    Vec2 getPreviousLocationInView() const;
    /** returns the start touch location in screen coordinates */
    Vec2 getStartLocationInView() const;

    void setTouchInfo(int id, float x, float y)
    {
        m_nId = id;
        m_prevPoint = m_point;
        m_point.x   = x;
        m_point.y   = y;
        if (!m_startPointCaptured)
        {
            m_startPoint = m_point;
            m_startPointCaptured = true;
        }
    }
    /**
     *  @js getId
     */
    int getID() const
    {
        return m_nId;
    }

    CCTouch *copy();

private:
    int m_nId;
    bool m_startPointCaptured;
    Vec2 m_startPoint;
    Vec2 m_point;
    Vec2 m_prevPoint;
};

class CC_DLL CCEvent : public CCObject
{
};

class CCNode;
class CCSet;

class CC_DLL CCTouchTargetNode : public CCObject
{
public:
	static CCTouchTargetNode *create(CCNode *node);
	~CCTouchTargetNode();

	CCNode *getNode();

	int getTouchMode();

	int getTouchId();
	void setTouchId(int touchId);

	CCTouch *findTouch(CCSet *touches);
	static CCTouch *findTouchFromTouchesSet(CCSet *touches, int touchId);

private:
	CCTouchTargetNode(CCNode *node);

	CCNode *m_node;
	int m_touchMode;
	int m_touchId;
};


// end of input group
/// @}

NS_CC_END

#endif  // __PLATFORM_TOUCH_H__
