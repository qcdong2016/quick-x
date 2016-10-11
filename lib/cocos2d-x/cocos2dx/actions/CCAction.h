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

#ifndef __ACTIONS_CCACTION_H__
#define __ACTIONS_CCACTION_H__

#include "cocoa/CCObject.h"
#include "cocoa/CCGeometry.h"
#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN

enum {
    //! Default tag
    kCCActionTagInvalid = -1,
};

/**
 * @addtogroup actions
 * @{
 */

/** 
@brief Base class for CCAction objects.
 */
class CC_DLL CCAction : public CCObject 
{
	CCOBJECT(CCAction, CCObject)
public:
    /**
     * @js ctor
     */
    CCAction(void);
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCAction(void);
    /**
     * @js NA
     * @lua NA
     */
    const char* description();
    /**
     * @js NA
     * @lua NA
     */
    virtual void paste(CCObject* o);

    //! return true if the action has finished
    virtual bool isDone(void);

    //! called before the action start. It will also set the target.
    virtual void startWithTarget(CCNode *pTarget);

    /** 
    called after the action has finished. It will set the 'target' to nil.
    IMPORTANT: You should never call "[action stop]" manually. Instead, use: "target->stopAction(action);"
    */
    virtual void stop(void);

    //! called every frame with it's delta time. DON'T override unless you know what you are doing.
    virtual void step(float dt);

    /** 
    called once per frame. time a value between 0 and 1

    For example: 
    - 0 means that the action just started
    - 0.5 means that the action is in the middle
    - 1 means that the action is over
    */
    virtual void update(float time);
    
    inline CCNode* getTarget(void) { return m_pTarget; }
    /** The action will modify the target properties. */
    inline void setTarget(CCNode *pTarget) { m_pTarget = pTarget; }
    
    inline CCNode* getOriginalTarget(void) { return m_pOriginalTarget; } 
    /** Set the original target, since target can be nil.
    Is the target that were used to run the action. Unless you are doing something complex, like CCActionManager, you should NOT call this method.
    The target is 'assigned', it is not 'retained'.
    @since v0.8.2
    */
    inline void setOriginalTarget(CCNode *pOriginalTarget) { m_pOriginalTarget = pOriginalTarget; }

    inline int getTag(void) { return m_nTag; }
    inline void setTag(int nTag) { m_nTag = nTag; }

public:
    /** Create an action */
    static CCAction* create();
protected:
    CCNode    *m_pOriginalTarget;
    /** The "target".
    The target will be set with the 'startWithTarget' method.
    When the 'stop' method is called, target will be set to nil.
    The target is 'assigned', it is not 'retained'.
    */
    CCNode    *m_pTarget;
    /** The action tag. An identifier of the action */
    int     m_nTag;
};

// end of actions group
/// @}

NS_CC_END

#endif // __ACTIONS_CCACTION_H__
