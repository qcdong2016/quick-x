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

#ifndef __CCINSTANT_ACTION_H__
#define __CCINSTANT_ACTION_H__

#include <string>
#include "CCActionFiniteTime.h"

NS_CC_BEGIN

/**
 * @addtogroup actions
 * @{
 */

/** 
@brief Instant actions are immediate actions. They don't have a duration like
the CCIntervalAction actions.
*/ 
class CC_DLL CCActionInstant : public CCFiniteTimeAction
{
	CCOBJECT(CCActionInstant, CCFiniteTimeAction)
public:
    /**
     *  @js ctor
     */
    CCActionInstant();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCActionInstant(){}
    // CCAction methods
    /**
     *  @js NA
     *  @lua NA
     */
    virtual bool isDone(void);
    virtual void step(float dt);
    virtual void update(float time);
    //CCFiniteTimeAction method
    virtual CCFiniteTimeAction * reverse(void);
};

/** @brief Show the node
*/
class CC_DLL CCShow : public CCActionInstant
{
    CCOBJECT(CCShow, CCActionInstant)
public:
    /**
     *  @js ctor
     *  @lua NA
     */
    CCShow(){}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCShow(){}
    //super methods
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);

public:

    /** Allocates and initializes the action */
    static CCShow * create();
};



/** 
@brief Hide the node
*/
class CC_DLL CCHide : public CCActionInstant
{
    CCOBJECT(CCHide, CCActionInstant)
public:
    /**
     *  @js ctor
     *  @lua NA
     */
    CCHide(){}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCHide(){}
    //super methods
    /**
     *  @lua NA
     */
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);

public:

    /** Allocates and initializes the action */
    static CCHide * create();
};

/** @brief Toggles the visibility of a node
*/
class CC_DLL CCToggleVisibility : public CCActionInstant
{
    CCOBJECT(CCToggleVisibility, CCActionInstant)
public:
    /**
     *  @js ctor
     */
    CCToggleVisibility(){}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCToggleVisibility(){}
    //super method
    virtual void update(float time);

public:

    /** Allocates and initializes the action */
    static CCToggleVisibility * create();
};

/** 
 @brief Remove the node
 @js NA
 @lua NA
 */
class CC_DLL CCRemoveSelf : public CCActionInstant
{
    CCOBJECT(CCRemoveSelf, CCActionInstant)
public:
	CCRemoveSelf(){}
	virtual ~CCRemoveSelf(){}
	//super methods
	virtual void update(float time);
	virtual CCFiniteTimeAction * reverse(void);
	virtual void paste(CCObject* o);
public:
	/** create the action */
	static CCRemoveSelf * create(bool isNeedCleanUp = true);
	/** init the action */
	bool init(bool isNeedCleanUp);
protected:
	bool m_bIsNeedCleanUp;
};

/** 
@brief Flips the sprite horizontally
@since v0.99.0
@js NA
*/
class CC_DLL CCFlipX : public CCActionInstant
{
    CCOBJECT(CCFlipX, CCActionInstant)
public:
    /**
     *  @js ctor
     */
    CCFlipX()
        :m_bFlipX(false)
    {}
    /**
     *  @js  NA
     *  @lua NA
     */
    virtual ~CCFlipX(){}

    /** create the action */
    static CCFlipX * create(bool x);

    /** init the action */
    bool initWithFlipX(bool x);
    //super methods
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);
    /**
     *  @lua NA
     */
    virtual void paste(CCObject* o);

protected:
    bool    m_bFlipX;
};

/** 
@brief Flips the sprite vertically
@since v0.99.0
@js NA
*/
class CC_DLL CCFlipY : public CCActionInstant
{
    CCOBJECT(CCFlipY, CCActionInstant)
public:
    /**
     *  @js ctor
     */
    CCFlipY()
        :m_bFlipY(false)
    {}
    /**
     *  @js  NA
     *  @lua NA
     */
    virtual ~CCFlipY(){}

    /** create the action */
    static CCFlipY * create(bool y);

    /** init the action */
    bool initWithFlipY(bool y);
    //super methods
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);
    /**
     *  @lua NA
     */
    virtual void paste(CCObject* o);

protected:
    bool    m_bFlipY;
};

/** @brief Places the node in a certain position
*/
class CC_DLL CCPlace : public CCActionInstant
{
	CCOBJECT(CCPlace, CCActionInstant)
public:
    /**
     *  @js ctor
     */
    CCPlace(){}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCPlace(){}

    /** creates a Place action with a position */
    static CCPlace * create(const Vec2& pos);
    /** Initializes a Place action with a position */
    bool initWithPosition(const Vec2& pos);
    //super methods
    virtual void update(float time);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual void paste(CCObject* o);
protected:
    Vec2 m_tPosition;
};

/** @brief Calls a 'callback'
*/
class CC_DLL CCCallFunc : public CCActionInstant
{
	CCOBJECT(CCCallFunc, CCActionInstant)
public:
    /**
     *  @js ctor
     */
    CCCallFunc() : _handler(0)
    {
    }
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCCallFunc();

    /** creates the action with the callback 

	* typedef void (CCObject::*SEL_CallFunc)();
	* @lua NA
	*/
	static CCCallFunc * create(EventHandler* handler);

    /** executes the callback 
     * @lua NA
     */
    virtual void execute();
    /** super methods
     * @lua NA
     */
    virtual void update(float time);
    /**
     * @js  NA
     * @lua NA
     */
	virtual void paste(CCObject* o);
protected:
	EventHandler* _handler;
};

NS_CC_END

#endif //__CCINSTANT_ACTION_H__
