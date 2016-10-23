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

#ifndef __ACTIONS_FINITETIME_H__
#define __ACTIONS_FINITETIME_H__

#include "CCAction.h"

NS_CC_BEGIN

/**
@brief
Base class actions that do have a finite time duration.
Possible actions:
- An action with a duration of 0 seconds
- An action with a duration of 35.5 seconds

Infinite time actions are valid
*/
class CC_DLL CCFiniteTimeAction : public CCAction
{
	CCOBJECT(CCFiniteTimeAction, CCAction)
public:
	/**
	*  @js ctor
	*/
	CCFiniteTimeAction()
		: m_fDuration(0)
	{}
	/**
	*  @js NA
	*  @lua NA
	*/
	virtual ~CCFiniteTimeAction() {}
	//! get duration in seconds of the action
	inline float getDuration(void) { return m_fDuration; }
	//! set duration in seconds of the action
	inline void setDuration(float duration) { m_fDuration = duration; }

	/** returns a reversed action */
	virtual CCFiniteTimeAction* reverse(void);
protected:
	//! duration in seconds
	float m_fDuration;
};

class CCActionInterval;
class CCRepeatForever;

/**
@brief Changes the speed of an action, making it take longer (speed>1)
or less (speed<1) time.
Useful to simulate 'slow motion' or 'fast forward' effect.
@warning This action can't be Sequenceable because it is not an CCIntervalAction
*/
class CC_DLL CCSpeed : public CCAction
{
	CCOBJECT(CCSpeed, CCAction)
public:
	/**
	*  @js ctor
	*/
	CCSpeed()
		: m_fSpeed(0.0)
		, m_pInnerAction(NULL)
	{}
	/**
	*  @js NA
	*  @lua NA
	*/
	virtual ~CCSpeed(void);

	inline float getSpeed(void) { return m_fSpeed; }
	/** alter the speed of the inner function in runtime */
	inline void setSpeed(float fSpeed) { m_fSpeed = fSpeed; }

	/** initializes the action */
	bool initWithAction(CCActionInterval *pAction, float fSpeed);
	/**
	*  @js NA
	*  @lua NA
	*/
	virtual void paste(CCObject* o);
	virtual void startWithTarget(CCNode* pTarget);
	virtual void stop();
	virtual void step(float dt);
	virtual bool isDone(void);
	virtual CCActionInterval* reverse(void);

	void setInnerAction(CCActionInterval *pAction);

	inline CCActionInterval* getInnerAction()
	{
		return m_pInnerAction;
	}

public:
	/** create the action */
	static CCSpeed* create(CCActionInterval* pAction, float fSpeed);
protected:
	float m_fSpeed;
	CCActionInterval *m_pInnerAction;
};

/**
@brief CCFollow is an action that "follows" a node.

Eg:
layer->runAction(CCFollow::actionWithTarget(hero));

Instead of using CCCamera as a "follower", use this action instead.
@since v0.99.2
*/
class CC_DLL CCFollow : public CCAction
{
	CCOBJECT(CCFollow, CCAction)
public:
	/**
	*  @js ctor
	*/
	CCFollow()
		: m_pobFollowedNode(NULL)
		, m_bBoundarySet(false)
		, m_bBoundaryFullyCovered(false)
		, m_fLeftBoundary(0.0)
		, m_fRightBoundary(0.0)
		, m_fTopBoundary(0.0)
		, m_fBottomBoundary(0.0)
	{}
	/**
	*  @js NA
	*  @lua NA
	*/
	virtual ~CCFollow(void);

	inline bool isBoundarySet(void) { return m_bBoundarySet; }
	/** alter behavior - turn on/off boundary */
	inline void setBoudarySet(bool bValue) { m_bBoundarySet = bValue; }

	/** initializes the action with a set boundary */
	bool initWithTarget(CCNode *pFollowedNode, const CCRect& rect = CCRectZero);

	virtual void step(float dt);
	virtual bool isDone(void);
	virtual void stop(void);

public:
	/** creates the action with a set boundary,
	It will work with no boundary if @param rect is equal to CCRectZero.
	*/
	static CCFollow* create(CCNode *pFollowedNode, const CCRect& rect = CCRectZero);
protected:
	// node to follow
	CCNode *m_pobFollowedNode;

	// whether camera should be limited to certain area
	bool m_bBoundarySet;

	// if screen size is bigger than the boundary - update not needed
	bool m_bBoundaryFullyCovered;

	// fast access to the screen dimensions
	CCPoint m_obHalfScreenSize;
	CCPoint m_obFullScreenSize;

	// world boundaries
	float m_fLeftBoundary;
	float m_fRightBoundary;
	float m_fTopBoundary;
	float m_fBottomBoundary;
};

// end of actions group
/// @}

NS_CC_END

#endif // __ACTIONS_CCACTION_H__
