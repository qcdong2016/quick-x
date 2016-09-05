#include "lua_cocos2dx_cocostudio_manual.h"

#ifdef __cplusplus
extern "C" {
#endif
#include  "tolua_fix.h"
#ifdef __cplusplus
}
#endif

#include "cocos2d.h"
#include "CCLuaStack.h"
#include "CCLuaValue.h"
#include "CCLuaEngine.h"
#include "../BaseClasses/UIWidget.h"
#include "../UIWidgets/UICheckBox.h"
#include "../UIWidgets/UISlider.h"
#include "../UIWidgets/UITextField.h"
#include "../UIWidgets/ScrollWidget/UIPageView.h"
#include "../UIWidgets/ScrollWidget/UIScrollView.h"
#include "../UIWidgets/ScrollWidget/UIListView.h"


USING_NS_CC;

using namespace ui;

class LuaCocoStudioEventListener:public CCObject
{
public:
    LuaCocoStudioEventListener();
    virtual ~LuaCocoStudioEventListener();
    
    static LuaCocoStudioEventListener* create();
    
    virtual void eventCallbackFunc(CCObject* sender,int eventType);
    
    void setHandler(int handler){ m_lHandler = handler; }
    int  getHandler() { return m_lHandler; }
private:
    long m_lHandler;
};

LuaCocoStudioEventListener::LuaCocoStudioEventListener():m_lHandler(0)
{
    
}

LuaCocoStudioEventListener::~LuaCocoStudioEventListener()
{
    
}

LuaCocoStudioEventListener* LuaCocoStudioEventListener::create()
{
    LuaCocoStudioEventListener* listener = new LuaCocoStudioEventListener();
    if (NULL == listener)
        return NULL;
    
    listener->autorelease();
    
    return listener;
}

void LuaCocoStudioEventListener::eventCallbackFunc(CCObject* sender,int eventType)
{
    if (0 != m_lHandler)
    {
        CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();        
        CCLuaStack* pStack = pEngine->getLuaStack();
        pStack->pushCCObject(sender, "CCObject");
        pStack->pushInt(eventType);
        pStack->executeFunctionByHandler(m_lHandler, 2);
        pStack->clean();
    }
}

static int tolua_Cocos2dx_Widget_addTouchEventListener00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"Widget",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Widget* self = (Widget*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addTouchEventListener'", NULL);
#endif
        LuaCocoStudioEventListener* listener = LuaCocoStudioEventListener::create();
        if (NULL == listener)
        {
            tolua_error(tolua_S,"LuaCocoStudioEventListener create fail\n", NULL);
            return 0;
        }
        
        LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
        
        listener->setHandler(handler);
        
        CCDictionary* dict = static_cast<CCDictionary*>(self->getScriptObjectDict());
        if (NULL == dict)
        {
            dict = CCDictionary::create();
            self->setScriptObjectDict(dict);
        }
        dict->setObject(listener, "widgetTouchEvent");
        
        self->addTouchEventListener(listener, toucheventselector(LuaCocoStudioEventListener::eventCallbackFunc));
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addTouchEventListener'.",&tolua_err);
    return 0;
#endif
}

static void extendWidget(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "Widget");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "addTouchEventListener", tolua_Cocos2dx_Widget_addTouchEventListener00);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_Cocos2dx_CheckBox_addEventListenerCheckBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CheckBox",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CheckBox* self = (CheckBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEventListenerCheckBox'", NULL);
#endif
        LuaCocoStudioEventListener* listener = LuaCocoStudioEventListener::create();
        if (NULL == listener)
        {
            tolua_error(tolua_S,"LuaCocoStudioEventListener create fail\n", NULL);
            return 0;
        }
        
        LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
        
        listener->setHandler(handler);
        
        CCDictionary* dict = static_cast<CCDictionary*>(self->getScriptObjectDict());
        if (NULL == dict)
        {
            dict = CCDictionary::create();
            self->setScriptObjectDict(dict);
        }
        dict->setObject(listener, "checkBoxEventListener");
        
        self->addEventListenerCheckBox(listener, checkboxselectedeventselector(LuaCocoStudioEventListener::eventCallbackFunc));
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addEventListenerCheckBox'.",&tolua_err);
    return 0;
#endif
}

static void extendCheckBox(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "CheckBox");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "addEventListenerCheckBox", tolua_Cocos2dx_CheckBox_addEventListenerCheckBox00);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_Cocos2dx_Slider_addEventListenerSlider00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"Slider",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Slider* self = (Slider*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEventListenerSlider'", NULL);
#endif
        LuaCocoStudioEventListener* listener = LuaCocoStudioEventListener::create();
        if (NULL == listener)
        {
            tolua_error(tolua_S,"LuaCocoStudioEventListener create fail\n", NULL);
            return 0;
        }
        
        LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
        
        listener->setHandler(handler);
        
        CCDictionary* dict = static_cast<CCDictionary*>(self->getScriptObjectDict());
        if (NULL == dict)
        {
            dict = CCDictionary::create();
            self->setScriptObjectDict(dict);
        }
        dict->setObject(listener, "sliderEventListener");
        
        self->addEventListenerSlider(listener, sliderpercentchangedselector(LuaCocoStudioEventListener::eventCallbackFunc));
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addEventListenerSlider'.",&tolua_err);
    return 0;
#endif
}

static void extendSlider(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "Slider");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "addEventListenerSlider", tolua_Cocos2dx_Slider_addEventListenerSlider00);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_Cocos2dx_TextField_addEventListenerTextField00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"TextField",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        TextField* self = (TextField*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEventListenerTextField'", NULL);
#endif
        LuaCocoStudioEventListener* listener = LuaCocoStudioEventListener::create();
        if (NULL == listener)
        {
            tolua_error(tolua_S,"LuaCocoStudioEventListener create fail\n", NULL);
            return 0;
        }
        
        LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
        
        listener->setHandler(handler);
        
        CCDictionary* dict = static_cast<CCDictionary*>(self->getScriptObjectDict());
        if (NULL == dict)
        {
            dict = CCDictionary::create();
            self->setScriptObjectDict(dict);
        }
        dict->setObject(listener, "textfieldEventListener");
        
        self->addEventListenerTextField(listener, textfieldeventselector(LuaCocoStudioEventListener::eventCallbackFunc));
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addEventListenerTextField'.",&tolua_err);
    return 0;
#endif
}

static void extendTextFieldr(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "TextField");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "addEventListenerTextField", tolua_Cocos2dx_TextField_addEventListenerTextField00);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_Cocos2dx_PageView_addEventListenerPageView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"PageView",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        PageView* self = (PageView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEventListenerPageView'", NULL);
#endif
        LuaCocoStudioEventListener* listener = LuaCocoStudioEventListener::create();
        if (NULL == listener)
        {
            tolua_error(tolua_S,"LuaCocoStudioEventListener create fail\n", NULL);
            return 0;
        }
        
        LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
        
        listener->setHandler(handler);
        
        CCDictionary* dict = static_cast<CCDictionary*>(self->getScriptObjectDict());
        if (NULL == dict)
        {
            dict = CCDictionary::create();
            self->setScriptObjectDict(dict);
        }
        dict->setObject(listener, "pageViewEventListener");
        
        self->addEventListenerPageView(listener, pagevieweventselector(LuaCocoStudioEventListener::eventCallbackFunc));
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addEventListenerPageView'.",&tolua_err);
    return 0;
#endif
}

static void extendPageView(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "PageView");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "addEventListenerPageView", tolua_Cocos2dx_PageView_addEventListenerPageView00);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_Cocos2dx_ScrollView_addEventListenerScrollView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"ScrollView",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        ScrollView* self = (ScrollView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEventListenerScrollView'", NULL);
#endif
        LuaCocoStudioEventListener* listener = LuaCocoStudioEventListener::create();
        if (NULL == listener)
        {
            tolua_error(tolua_S,"LuaCocoStudioEventListener create fail\n", NULL);
            return 0;
        }
        
        LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
        
        listener->setHandler(handler);
        
        CCDictionary* dict = static_cast<CCDictionary*>(self->getScriptObjectDict());
        if (NULL == dict)
        {
            dict = CCDictionary::create();
            self->setScriptObjectDict(dict);
        }
        dict->setObject(listener, "scrollViewEventListener");
        
        self->addEventListenerScrollView(listener, scrollvieweventselector(LuaCocoStudioEventListener::eventCallbackFunc));
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addEventListenerScrollView'.",&tolua_err);
    return 0;
#endif
}

static void extendScrollView(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "ListView");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "addEventListenerScrollView", tolua_Cocos2dx_ScrollView_addEventListenerScrollView00);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_Cocos2dx_ListView_addEventListenerListView00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"ListView",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        ListView* self = (ListView*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addEventListenerListView'", NULL);
#endif
        LuaCocoStudioEventListener* listener = LuaCocoStudioEventListener::create();
        if (NULL == listener)
        {
            tolua_error(tolua_S,"LuaCocoStudioEventListener create fail\n", NULL);
            return 0;
        }
        
        LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
        
        listener->setHandler(handler);
        
        CCDictionary* dict = static_cast<CCDictionary*>(self->getScriptObjectDict());
        if (NULL == dict)
        {
            dict = CCDictionary::create();
            self->setScriptObjectDict(dict);
        }
        dict->setObject(listener, "listViewEventListener");
        
        self->addEventListenerListView(listener, listvieweventselector(LuaCocoStudioEventListener::eventCallbackFunc));
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addEventListenerListView'.",&tolua_err);
    return 0;
#endif
}

static void extendListView(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "ListView");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "addEventListenerListView", tolua_Cocos2dx_ListView_addEventListenerListView00);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_Cocos2dx_LayoutParameter_setMargin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"LayoutParameter",0,&tolua_err) ||
        !tolua_istable(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        LayoutParameter* self = (LayoutParameter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMargin'", NULL);
#endif
        Margin margin;
        lua_pushstring(tolua_S, "left");
        lua_gettable(tolua_S,2);
        margin.left = lua_isnil(tolua_S,-1) ? 0 : lua_tonumber(tolua_S,-1);
        lua_pop(tolua_S,1);
        
        lua_pushstring(tolua_S, "top");
        lua_gettable(tolua_S,2);
        margin.top = lua_isnil(tolua_S,-1) ? 0 : lua_tonumber(tolua_S,-1);
        lua_pop(tolua_S,1);
        
        lua_pushstring(tolua_S, "right");
        lua_gettable(tolua_S,2);
        margin.right = lua_isnil(tolua_S,-1) ? 0 : lua_tonumber(tolua_S,-1);
        lua_pop(tolua_S,1);
        
        lua_pushstring(tolua_S, "bottom");
        lua_gettable(tolua_S,2);
        margin.bottom = lua_isnil(tolua_S,-1) ? 0 : lua_tonumber(tolua_S,-1);
        lua_pop(tolua_S,1);
        
        self->setMargin(margin);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setMargin'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2dx_LayoutParameter_getMargin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"LayoutParameter",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        LayoutParameter* self = (LayoutParameter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMargin'", NULL);
#endif
        Margin margin = self->getMargin();
        
        lua_newtable(tolua_S);
        
        lua_pushstring(tolua_S, "left");
        lua_pushnumber(tolua_S, (lua_Number) margin.left);
        lua_rawset(tolua_S, -3);
        
        lua_pushstring(tolua_S, "top");
        lua_pushnumber(tolua_S, (lua_Number) margin.top);
        lua_rawset(tolua_S, -3);
        
        lua_pushstring(tolua_S, "right");
        lua_pushnumber(tolua_S, (lua_Number) margin.right);
        lua_rawset(tolua_S, -3);
        
        lua_pushstring(tolua_S, "bottom");
        lua_pushnumber(tolua_S, (lua_Number) margin.bottom);
        lua_rawset(tolua_S, -3);
        
        return 1;
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getMargin'.",&tolua_err);
    return 0;
#endif
}

static void extendLayoutParameter(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "LayoutParameter");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "setMargin", tolua_Cocos2dx_LayoutParameter_setMargin00);
        tolua_function(tolua_S, "getMargin", tolua_Cocos2dx_LayoutParameter_getMargin00);
    }
    lua_pop(tolua_S, 1);
}

int register_all_cocos2dx_studio_manual(lua_State* tolua_S)
{
    extendWidget(tolua_S);
    extendCheckBox(tolua_S);
    extendSlider(tolua_S);
    extendTextFieldr(tolua_S);
    extendPageView(tolua_S);
    extendScrollView(tolua_S);
    extendListView(tolua_S);
    extendLayoutParameter(tolua_S);
    return 0;
}
