/****************************************************************************
 Copyright (c) 2011 cocos2d-x.org

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

#include "CCLuaEngine.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCScriptEventDispatcher.h"
#include "CCScheduler.h"
#include "nodes/CCNode.h"

#include "engine/CCDirector.h"
#include "nodes/CCLayer.h"

NS_CC_BEGIN

CCLuaEngine* CCLuaEngine::m_defaultEngine = NULL;

CCLuaEngine* CCLuaEngine::defaultEngine(void)
{
    if (!m_defaultEngine)
    {
        m_defaultEngine = new CCLuaEngine();
        m_defaultEngine->init();
    }
    return m_defaultEngine;
}

CCLuaEngine::~CCLuaEngine(void)
{
    CC_SAFE_RELEASE(m_stack);
    m_defaultEngine = NULL;
}

bool CCLuaEngine::init(void)
{
    m_stack = CCLuaStack::create();
    m_stack->retain();
    return true;
}

void CCLuaEngine::addSearchPath(const char* path)
{
    m_stack->addSearchPath(path);
}

void CCLuaEngine::addLuaLoader(lua_CFunction func)
{
    m_stack->addLuaLoader(func);
}

void CCLuaEngine::removeScriptObjectByCCObject(CCObject* pObj)
{
    m_stack->removeScriptObjectByCCObject(pObj);
}

void CCLuaEngine::removeScriptHandler(int nHandler)
{
    m_stack->removeScriptHandler(nHandler);
}

int CCLuaEngine::executeString(const char *codes)
{
    int ret = m_stack->executeString(codes);
    m_stack->clean();
    return ret;
}

int CCLuaEngine::executeScriptFile(const char* filename)
{
    int ret = m_stack->executeScriptFile(filename);
    m_stack->clean();
    return ret;
}

int CCLuaEngine::executeGlobalFunction(const char* functionName, int numArgs /* = 0 */)
{
    int ret = m_stack->executeGlobalFunction(functionName, numArgs);
    m_stack->clean();
    return ret;
}

int CCLuaEngine::executeNodeTouchEvent(CCNode* pNode, int eventType, CCTouch *pTouch, int phase)
{
    m_stack->clean();
    CCLuaValueDict event;
    switch (eventType)
    {
        case CCTOUCHBEGAN:
            event["name"] = CCLuaValue::stringValue("began");
            break;

        case CCTOUCHMOVED:
            event["name"] = CCLuaValue::stringValue("moved");
            break;

        case CCTOUCHENDED:
            event["name"] = CCLuaValue::stringValue("ended");
            break;

        case CCTOUCHCANCELLED:
            event["name"] = CCLuaValue::stringValue("cancelled");
            break;

        default:
            CCAssert(false, "INVALID touch event");
            return 0;
    }

    event["mode"] = CCLuaValue::intValue(kCCTouchesOneByOne);
    switch (phase)
    {
        case NODE_TOUCH_CAPTURING_PHASE:
            event["phase"] = CCLuaValue::stringValue("capturing");
            break;

        case NODE_TOUCH_TARGETING_PHASE:
            event["phase"] = CCLuaValue::stringValue("targeting");
            break;

        default:
            event["phase"] = CCLuaValue::stringValue("unknown");
    }

    const Vec2 pt = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    event["x"] = CCLuaValue::floatValue(pt.x);
    event["y"] = CCLuaValue::floatValue(pt.y);
    const Vec2 prev = CCDirector::sharedDirector()->convertToGL(pTouch->getPreviousLocationInView());
    event["prevX"] = CCLuaValue::floatValue(prev.x);
    event["prevY"] = CCLuaValue::floatValue(prev.y);

    m_stack->pushCCLuaValueDict(event);

    int eventInt = (phase == NODE_TOUCH_CAPTURING_PHASE) ? NODE_TOUCH_CAPTURE_EVENT : NODE_TOUCH_EVENT;
    CCArray *listeners = pNode->getAllScriptEventListeners();
    CCScriptHandlePair *p;
    int ret = 1;
    for (int i = listeners->count() - 1; i >= 0; --i)
    {
        p = dynamic_cast<CCScriptHandlePair*>(listeners->objectAtIndex(i));
        if (p->event != eventInt || p->removed) continue;

        if (eventType == CCTOUCHBEGAN)
        {
            // enable listener when touch began
            p->enabled = true;
        }

        if (p->enabled)
        {
            m_stack->copyValue(1);
            int listenerRet = m_stack->executeFunctionByHandler(p->listener, 1);
            if (listenerRet == 0)
            {
                if (phase == NODE_TOUCH_CAPTURING_PHASE && (eventType == CCTOUCHBEGAN || eventType == CCTOUCHMOVED))
                {
                    ret = 0;
                }
                else if (phase == NODE_TOUCH_TARGETING_PHASE && eventType == CCTOUCHBEGAN)
                {
                    // if listener return false when touch began, disable this listener
                    p->enabled = false;
                    ret = 0;
                }
            }
            m_stack->settop(1);
        }
    }

    //CCLOG("executeNodeTouchEvent %p, ret = %d, event = %d, phase = %d", pNode, ret, eventType, phase);
    m_stack->clean();

    return ret;
}

int CCLuaEngine::executeNodeTouchesEvent(CCNode* pNode, int eventType, CCSet *pTouches, int phase)
{
    m_stack->clean();
    CCLuaValueDict event;
    switch (eventType)
    {
        case CCTOUCHBEGAN:
            event["name"] = CCLuaValue::stringValue("began");
            break;

        case CCTOUCHMOVED:
            event["name"] = CCLuaValue::stringValue("moved");
            break;

        case CCTOUCHENDED:
            event["name"] = CCLuaValue::stringValue("ended");
            break;

        case CCTOUCHCANCELLED:
            event["name"] = CCLuaValue::stringValue("cancelled");
            break;

        case CCTOUCHADDED:
            event["name"] = CCLuaValue::stringValue("added");
            break;

        case CCTOUCHREMOVED:
            event["name"] = CCLuaValue::stringValue("removed");
            break;

        default:
            return 0;
    }

    event["mode"] = CCLuaValue::intValue(kCCTouchesAllAtOnce);
    switch (phase)
    {
        case NODE_TOUCH_CAPTURING_PHASE:
            event["phase"] = CCLuaValue::stringValue("capturing");
            break;

        case NODE_TOUCH_TARGETING_PHASE:
            event["phase"] = CCLuaValue::stringValue("targeting");
            break;

        default:
            event["phase"] = CCLuaValue::stringValue("unknown");
    }

    CCLuaValueDict points;
    CCDirector* pDirector = CCDirector::sharedDirector();
    char touchId[16];
    for (CCSetIterator touchIt = pTouches->begin(); touchIt != pTouches->end(); ++touchIt)
    {
        CCLuaValueDict point;
        CCTouch* pTouch = (CCTouch*)*touchIt;
        sprintf(touchId, "%d", pTouch->getID());
        point["id"] = CCLuaValue::stringValue(touchId);

        const Vec2 pt = pDirector->convertToGL(pTouch->getLocationInView());
        point["x"] = CCLuaValue::floatValue(pt.x);
        point["y"] = CCLuaValue::floatValue(pt.y);
        const Vec2 prev = pDirector->convertToGL(pTouch->getPreviousLocationInView());
        point["prevX"] = CCLuaValue::floatValue(prev.x);
        point["prevY"] = CCLuaValue::floatValue(prev.y);

        points[touchId] = CCLuaValue::dictValue(point);
    }
    event["points"] = CCLuaValue::dictValue(points);
    m_stack->pushCCLuaValueDict(event);

    int eventInt = (phase == NODE_TOUCH_CAPTURING_PHASE) ? NODE_TOUCH_CAPTURE_EVENT : NODE_TOUCH_EVENT;
    CCArray *listeners = pNode->getAllScriptEventListeners();
    CCScriptHandlePair *p;
    for (int i = listeners->count() - 1; i >= 0; --i)
    {
        p = dynamic_cast<CCScriptHandlePair*>(listeners->objectAtIndex(i));
        if (p->event != eventInt || p->removed) continue;
        m_stack->copyValue(1);
        m_stack->executeFunctionByHandler(p->listener, 1);
        m_stack->settop(1);
    }

    m_stack->clean();
    
    return 1;
}

int CCLuaEngine::executeEvent(int nHandler, const char* pEventName, CCObject* pEventSource /* = NULL*/, const char* pEventSourceClassName /* = NULL*/)
{
    m_stack->pushString(pEventName);
    if (pEventSource)
    {
        m_stack->pushCCObject(pEventSource, pEventSourceClassName ? pEventSourceClassName : "CCObject");
    }
    int ret = m_stack->executeFunctionByHandler(nHandler, pEventSource ? 2 : 1);
    m_stack->clean();
    return ret;
}

bool CCLuaEngine::handleAssert(const char *msg)
{
    bool ret = m_stack->handleAssert(msg);
    m_stack->clean();
    return ret;
}

int CCLuaEngine::reallocateScriptHandler(int nHandler)
{
    int nRet = m_stack->reallocateScriptHandler(nHandler);
    m_stack->clean();
    return nRet;
}

bool CCLuaEngine::parseConfig(CCScriptEngineProtocol::ConfigType type, const std::string& str)
{
    lua_getglobal(m_stack->getLuaState(), "__onParseConfig");
    if (!lua_isfunction(m_stack->getLuaState(), -1))
    {
        CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", "__onParseConfig");
        lua_pop(m_stack->getLuaState(), 1);
        return false;
    }
    
    m_stack->pushInt((int)type);
    m_stack->pushString(str.c_str());
    
    return m_stack->executeFunction(2);
}

NS_CC_END
