#ifndef COCOS2DX_SCRIPTING_LUA_COCOS2DX_SUPPORT_LUA_COCOS2DX_COCOSTUDIO_MANUAL_H
#define COCOS2DX_SCRIPTING_LUA_COCOS2DX_SUPPORT_LUA_COCOS2DX_COCOSTUDIO_MANUAL_H

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

#include "engine/CCObject.h"

LUA_API int register_all_cocos2dx_studio_manual(lua_State* tolua_S);

#endif //#define COCOS2DX_SCRIPTING_LUA_COCOS2DX_SUPPORT_LUA_COCOS2DX_COCOSTUDIO_MANUAL_H
