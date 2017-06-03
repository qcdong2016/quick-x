/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org

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

#ifndef __SCRIPT_SUPPORT_H__
#define __SCRIPT_SUPPORT_H__

#include "touch_dispatcher/CCTouch.h"
#include "cocoa/CCSet.h"
#include <map>
#include <string>
#include <list>

typedef struct lua_State lua_State;

NS_CC_BEGIN

class CCNode;
class CCTimer;
class CCLayer;
class CCMenuItem;
class CCCallFunc;

enum ccScriptType {
    kScriptTypeNone = 0,
    kScriptTypeLua,
    kScriptTypeJavascript
};
/**
 * @js NA
 * @lua NA
 */
class CCScriptListenerHandle : public CCObject
{
public:
    static CCScriptListenerHandle *create(int listener);
    ~CCScriptListenerHandle(void);

    int getListener(void);
    int getHandle(void);

protected:
    CCScriptListenerHandle(int listener);

    int m_listener;
    int m_handle;

    static int s_nextHandleId;
};

/**
 * @addtogroup script_support
 * @{
 * @js NA
 * @lua NA
 */

class CCSchedulerScriptHandle : public CCScriptListenerHandle
{
public:
    // nHandler return by tolua_ref_function(), called from LuaCocos2d.cpp
    static CCSchedulerScriptHandle *create(int listener, float interval, bool paused);
    ~CCSchedulerScriptHandle(void);

    cocos2d::CCTimer *getTimer(void);
    bool isPaused(void);
    void markedForDeletion(void);
    bool isMarkedForDeletion(void);

private:
    CCSchedulerScriptHandle(int listener, float interval, bool paused);

    cocos2d::CCTimer *m_timer;
    bool m_paused;
    bool m_markedForDeletion;
};


// Don't make CCScriptEngineProtocol inherits from CCObject since setScriptEngine is invoked only once in AppDelegate.cpp,
// It will affect the lifecycle of ScriptCore instance, the autorelease pool will be destroyed before destructing ScriptCore.
// So a crash will appear on Win32 if you click the close button.
/**
 * @js NA
 * @lua NA
 */
class CC_DLL CCScriptEngineProtocol
{
public:
    virtual ~CCScriptEngineProtocol() {};

    /** Get script type */
    virtual ccScriptType getScriptType() { return kScriptTypeNone; };

    /** Remove script object. */
    virtual void removeScriptObjectByCCObject(CCObject* pObj) = 0;

    /** Remove script function handler, only CCLuaEngine class need to implement this function. */
    virtual void removeScriptHandler(int nHandler) {};

    /** Reallocate script function handler, only CCLuaEngine class need to implement this function. */
    virtual int reallocateScriptHandler(int nHandler) { return -1;}

    /**
     @brief Execute script code contained in the given string.
     @param codes holding the valid script code that should be executed.
     @return 0 if the string is executed correctly.
     @return other if the string is executed wrongly.
     */
    virtual int executeString(const char* codes) = 0;

    /**
     @brief Execute a script file.
     @param filename String object holding the filename of the script file that is to be executed
     */
    virtual int executeScriptFile(const char* filename) = 0;

    /**
     @brief Execute a scripted global function.
     @brief The function should not take any parameters and should return an integer.
     @param functionName String object holding the name of the function, in the global script environment, that is to be executed.
     @return The integer value returned from the script function.
     */
    virtual int executeGlobalFunction(const char* functionName, int numArgs = 0) = 0;

    /** functions for executing touch event */
	virtual int executeNodeTouchEvent(CCNode* pNode, int eventType, const Vec2& pos, int phase) = 0;

    /** function for common event */
    virtual int executeEvent(int nHandler, const char* pEventName, CCObject* pEventSource = NULL, const char* pEventSourceClassName = NULL) = 0;

    /** called by CCAssert to allow scripting engine to handle failed assertions
     * @return true if the assert was handled by the script engine, false otherwise.
     */
    virtual bool handleAssert(const char *msg) = 0;

    /**
     *
     */
    enum ConfigType
    {
        NONE,
        COCOSTUDIO,
    };
    virtual bool parseConfig(ConfigType type, const std::string& str) = 0;
};

/**
 CCScriptEngineManager is a singleton which holds an object instance of CCScriptEngineProtocl
 It helps cocos2d-x and the user code to find back LuaEngine object
 @since v0.99.5-x-0.8.5
 @js NA
 @lua NA
 */
class CC_DLL CCScriptEngineManager
{
public:
    ~CCScriptEngineManager(void);

    inline CCScriptEngineProtocol* getScriptEngine(void) {
        return m_pScriptEngine;
    }
    void setScriptEngine(CCScriptEngineProtocol *pScriptEngine);
    void removeScriptEngine(void);

    static CCScriptEngineManager* sharedManager(void);
    static void purgeSharedManager(void);

private:
    CCScriptEngineManager(void)
    : m_pScriptEngine(NULL)
    {
    }

    CCScriptEngineProtocol *m_pScriptEngine;
};

// end of script_support group
/// @}

NS_CC_END

#endif // __SCRIPT_SUPPORT_H__
