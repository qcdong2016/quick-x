/*
** Lua binding: spine
** Generated automatically by tolua++-1.0.92 on 08/17/16 18:26:03.
*/

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

extern "C" {
#include "tolua_fix.h"
}

#include <map>
#include <string>
#include "CCLuaEngine.h"
#include "audio/SimpleAudioEngine.h"

using namespace cocos2d;

/* Exported function */
TOLUA_API int  tolua_spine_open (lua_State* tolua_S);

#include "SkeletonAnimation.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"SkeletonAnimation");
 toluafix_add_type_mapping(CLASS_HASH_CODE(typeid(SkeletonAnimation)), "SkeletonAnimation");
}

/* get function: timeScale of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_get_SkeletonAnimation_timeScale
static int tolua_get_SkeletonAnimation_timeScale(lua_State* tolua_S)
{
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timeScale'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->timeScale);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: timeScale of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_set_SkeletonAnimation_timeScale
static int tolua_set_SkeletonAnimation_timeScale(lua_State* tolua_S)
{
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'timeScale'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->timeScale = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: debugSlots of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_get_SkeletonAnimation_debugSlots
static int tolua_get_SkeletonAnimation_debugSlots(lua_State* tolua_S)
{
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'debugSlots'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->debugSlots);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: debugSlots of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_set_SkeletonAnimation_debugSlots
static int tolua_set_SkeletonAnimation_debugSlots(lua_State* tolua_S)
{
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'debugSlots'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->debugSlots = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: debugBones of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_get_SkeletonAnimation_debugBones
static int tolua_get_SkeletonAnimation_debugBones(lua_State* tolua_S)
{
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'debugBones'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->debugBones);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: debugBones of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_set_SkeletonAnimation_debugBones
static int tolua_set_SkeletonAnimation_debugBones(lua_State* tolua_S)
{
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'debugBones'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->debugBones = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: premultipliedAlpha of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_get_SkeletonAnimation_premultipliedAlpha
static int tolua_get_SkeletonAnimation_premultipliedAlpha(lua_State* tolua_S)
{
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'premultipliedAlpha'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->premultipliedAlpha);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: premultipliedAlpha of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_set_SkeletonAnimation_premultipliedAlpha
static int tolua_set_SkeletonAnimation_premultipliedAlpha(lua_State* tolua_S)
{
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'premultipliedAlpha'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->premultipliedAlpha = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: setToSetupPose of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_setToSetupPose00
static int tolua_spine_SkeletonAnimation_setToSetupPose00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setToSetupPose'", NULL);
#endif
  {
   self->setToSetupPose();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setToSetupPose'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBonesToSetupPose of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_setBonesToSetupPose00
static int tolua_spine_SkeletonAnimation_setBonesToSetupPose00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBonesToSetupPose'", NULL);
#endif
  {
   self->setBonesToSetupPose();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBonesToSetupPose'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSlotsToSetupPose of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_setSlotsToSetupPose00
static int tolua_spine_SkeletonAnimation_setSlotsToSetupPose00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSlotsToSetupPose'", NULL);
#endif
  {
   self->setSlotsToSetupPose();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSlotsToSetupPose'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setSkin of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_setSkin00
static int tolua_spine_SkeletonAnimation_setSkin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
  const char* skinName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setSkin'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->setSkin(skinName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setSkin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBlendFunc of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_setBlendFunc00
static int tolua_spine_SkeletonAnimation_setBlendFunc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
	 !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
	 !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S, "invalid 'self' in function 'setBlendFunc'", NULL);
#endif

  int src = tolua_tonumber(tolua_S, 2, 0);
  int dst = tolua_tonumber(tolua_S, 3, 0);

  ccBlendFunc arg;
  arg.src = src;
  arg.dst = dst;
  self->setBlendFunc(arg);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBlendFunc'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createWithFile of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_createWithFile00
static int tolua_spine_SkeletonAnimation_createWithFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* skeletonDataFile = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* atlasFile = ((const char*)  tolua_tostring(tolua_S,3,0));
  float scale = ((float)  tolua_tonumber(tolua_S,4,0));
  {
   SkeletonAnimation* tolua_ret = (SkeletonAnimation*)  SkeletonAnimation::create(skeletonDataFile,atlasFile,scale);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"SkeletonAnimation");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMix of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_setMix00
static int tolua_spine_SkeletonAnimation_setMix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
  const char* fromAnimation = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* toAnimation = ((const char*)  tolua_tostring(tolua_S,3,0));
  float duration = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMix'", NULL);
#endif
  {
   self->setMix(fromAnimation,toAnimation,duration);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAnimation of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_setAnimation00
static int tolua_spine_SkeletonAnimation_setAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
	 !tolua_isboolean(tolua_S, 4, 0, &tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
  int trackIndex = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
  bool loop = ((bool)  tolua_toboolean(tolua_S,4,0));

#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAnimation'", NULL);
#endif

   spTrackEntry* entry = self->setAnimation(trackIndex,name,loop);
   tolua_pushusertype(tolua_S, entry, "spTrackEntry");
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE


/* method: addAnimation of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_addAnimation00
static int tolua_spine_SkeletonAnimation_addAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
  int trackIndex = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
  bool loop = ((bool)  tolua_toboolean(tolua_S,4,0));
  float delay = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addAnimation'", NULL);
#endif
  {
   self->addAnimation(trackIndex,name,loop,delay);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearTracks of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_clearTracks00
static int tolua_spine_SkeletonAnimation_clearTracks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearTracks'", NULL);
#endif
  {
   self->clearTracks();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearTracks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearTrack of class  SkeletonAnimation */
#ifndef TOLUA_DISABLE_tolua_spine_SkeletonAnimation_clearTrack00
static int tolua_spine_SkeletonAnimation_clearTrack00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SkeletonAnimation",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
  int trackIndex = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearTrack'", NULL);
#endif
  {
   self->clearTrack(trackIndex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearTrack'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE


// 
// /* set function: eventListener of class  SkeletonAnimation */
// static int tolua_set_SkeletonAnimation_setListener00(lua_State* tolua_S)
// {
// 	tolua_Error tolua_err;
// 	if (!tolua_isusertype(tolua_S, 1, "SkeletonAnimation", 0, &tolua_err))
// 	{
// 		tolua_error(tolua_S, "#ferror in function 'setListener'.", &tolua_err);
// 		return 0;
// 	}
//   SkeletonAnimation* self = (SkeletonAnimation*)  tolua_tousertype(tolua_S,1,0);
// #ifndef TOLUA_RELEASE
// 
//   if (
// 	  tolua_isstring(tolua_S, 2, 0, &tolua_err) &&
// 	  toluafix_isfunction(tolua_S, 3, "LUA_FUNCTION", 0, &tolua_err) &&
// 	  tolua_isnoobj(tolua_S, 4, &tolua_err)
// 	  )
//   {
// 	  const char* what = tolua_tostring(tolua_S, 2, 0);
// 	  SharedPtr<LuaFunction> listener(new LuaFunction(tolua_S, 3));
// 
// 	  if (!strcmp(what, "end")) self->endListener = listener;
// 	  else if (!strcmp(what, "event"))self->eventListener = listener;
// 	  else if (!strcmp(what, "complete"))self->completeListener = listener;
// 	  else if (!strcmp(what, "start"))self->startListener = listener;
//   }
//   else if (
// 	  tolua_isusertype(tolua_S, 2, "spTrackEntry", 0, &tolua_err) &&
// 	  tolua_isstring(tolua_S, 3, 0, &tolua_err) &&
// 	  toluafix_isfunction(tolua_S, 4, "LUA_FUNCTION", 0, &tolua_err) &&
// 	  tolua_isnoobj(tolua_S, 5, &tolua_err)
// 	  )
//   {
// 	  spTrackEntry* entry = (spTrackEntry*)tolua_tousertype(tolua_S, 2, 0);
// 	  const char* what = tolua_tostring(tolua_S, 3, 0);
// 
// 	  SharedPtr<LuaFunction> listener(new LuaFunction(tolua_S, 4));
// 
// 	  if (!strcmp(what, "end")) self->setEndListener(entry, listener);
// 	  else if (!strcmp(what, "event"))self->setEventListener(entry, listener);
// 	  else if (!strcmp(what, "complete"))self->setCompleteListener(entry, listener);
// 	  else if (!strcmp(what, "start"))self->setStartListener(entry, listener);
//   }
//   else
//   {
//       tolua_error(tolua_S, "#ferror in function 'setListener'.", &tolua_err);
//       return 0;
//   }
// #endif
// 
//   
// 
//  return 0;
// }

/* Open function */
TOLUA_API int tolua_spine_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"SkeletonAnimation","SkeletonAnimation","CCNode",NULL);
  tolua_beginmodule(tolua_S,"SkeletonAnimation");
   tolua_variable(tolua_S,"timeScale",tolua_get_SkeletonAnimation_timeScale,tolua_set_SkeletonAnimation_timeScale);
   tolua_variable(tolua_S,"debugSlots",tolua_get_SkeletonAnimation_debugSlots,tolua_set_SkeletonAnimation_debugSlots);
   tolua_variable(tolua_S,"debugBones",tolua_get_SkeletonAnimation_debugBones,tolua_set_SkeletonAnimation_debugBones);
   tolua_variable(tolua_S,"premultipliedAlpha",tolua_get_SkeletonAnimation_premultipliedAlpha,tolua_set_SkeletonAnimation_premultipliedAlpha);
   tolua_function(tolua_S,"setToSetupPose",tolua_spine_SkeletonAnimation_setToSetupPose00);
   tolua_function(tolua_S,"setBonesToSetupPose",tolua_spine_SkeletonAnimation_setBonesToSetupPose00);
   tolua_function(tolua_S,"setSlotsToSetupPose",tolua_spine_SkeletonAnimation_setSlotsToSetupPose00);
   tolua_function(tolua_S,"setSkin",tolua_spine_SkeletonAnimation_setSkin00);
   tolua_function(tolua_S,"setBlendFunc",tolua_spine_SkeletonAnimation_setBlendFunc00);
   tolua_function(tolua_S,"create",tolua_spine_SkeletonAnimation_createWithFile00);
   tolua_function(tolua_S,"setMix",tolua_spine_SkeletonAnimation_setMix00);
   tolua_function(tolua_S,"setAnimation",tolua_spine_SkeletonAnimation_setAnimation00);
   tolua_function(tolua_S,"addAnimation",tolua_spine_SkeletonAnimation_addAnimation00);
   tolua_function(tolua_S,"clearTracks",tolua_spine_SkeletonAnimation_clearTracks00);
   tolua_function(tolua_S,"clearTrack",tolua_spine_SkeletonAnimation_clearTrack00);
  // tolua_function(tolua_S, "setListener", tolua_set_SkeletonAnimation_setListener00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_spine (lua_State* tolua_S) {
 return tolua_spine_open(tolua_S);
};
#endif

