/*
** Lua binding: cocos2dx_httprequest_luabinding
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
#include "CCLuaValue.h"

using namespace cocos2d;


/* Exported function */
TOLUA_API int tolua_cocos2dx_httprequest_luabinding_open (lua_State* tolua_S);

#include "CCNetwork.h"
using namespace cocos2d;

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CCObject");
 toluafix_add_type_mapping(CLASS_HASH_CODE(typeid(CCObject)), "CCObject");
 tolua_usertype(tolua_S,"CCHTTPRequest");
 toluafix_add_type_mapping(CLASS_HASH_CODE(typeid(CCHTTPRequest)), "CCHTTPRequest");
 tolua_usertype(tolua_S,"CCNetwork");
 toluafix_add_type_mapping(CLASS_HASH_CODE(typeid(CCNetwork)), "CCNetwork");
}

/* method: createWithUrl of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_createWithUrl00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_createWithUrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const char* url = ((const char*)  tolua_tostring(tolua_S,2,0));
  int method = ((int)  tolua_tonumber(tolua_S,3,kCCHTTPRequestMethodGET));
 {
  CCHTTPRequest* tolua_ret = (CCHTTPRequest*)  CCHTTPRequest::createWithUrl(url,method);
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCHTTPRequest");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createWithUrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRequestUrl of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setRequestUrl00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setRequestUrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  const char* url = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRequestUrl'", NULL);
#endif
 {
  self->setRequestUrl(url);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRequestUrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRequestUrl of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getRequestUrl00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getRequestUrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRequestUrl'", NULL);
#endif
 {
  std::string tolua_ret = (std::string)  self->getRequestUrl();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRequestUrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addRequestHeader of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addRequestHeader00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addRequestHeader00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  const char* header = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addRequestHeader'", NULL);
#endif
 {
  self->addRequestHeader(header);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addRequestHeader'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addPOSTValue of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addPOSTValue00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addPOSTValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  const char* key = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* value = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addPOSTValue'", NULL);
#endif
 {
  self->addPOSTValue(key,value);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addPOSTValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPOSTData of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setPOSTData00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setPOSTData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  const char* data = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPOSTData'", NULL);
#endif
 {
  self->setPOSTData(data);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPOSTData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addFormFile of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addFormFile00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addFormFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isstring(tolua_S,4,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* filePath = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* fileType = ((const char*)  tolua_tostring(tolua_S,4,"application/octet-stream"));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addFormFile'", NULL);
#endif
 {
  self->addFormFile(name,filePath,fileType);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addFormFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addFormContents of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addFormContents00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addFormContents00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* value = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addFormContents'", NULL);
#endif
 {
  self->addFormContents(name,value);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addFormContents'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCookieString of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setCookieString00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setCookieString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  const char* cookie = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCookieString'", NULL);
#endif
 {
  self->setCookieString(cookie);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCookieString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCookieString of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getCookieString00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getCookieString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCookieString'", NULL);
#endif
 {
  std::string tolua_ret = (std::string)  self->getCookieString();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCookieString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAcceptEncoding of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setAcceptEncoding00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setAcceptEncoding00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  int acceptEncoding = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAcceptEncoding'", NULL);
#endif
 {
  self->setAcceptEncoding(acceptEncoding);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAcceptEncoding'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTimeout of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setTimeout00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setTimeout00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  int timeout = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTimeout'", NULL);
#endif
 {
  self->setTimeout(timeout);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTimeout'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: start of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_start00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_start00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'start'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->start();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'start'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: cancel of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_cancel00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_cancel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'cancel'", NULL);
#endif
 {
  self->cancel();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cancel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getState of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getState00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getState'", NULL);
#endif
 {
  int tolua_ret = (int)  self->getState();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResponseStatusCode of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseStatusCode00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseStatusCode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getResponseStatusCode'", NULL);
#endif
 {
  int tolua_ret = (int)  self->getResponseStatusCode();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getResponseStatusCode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResponseHeadersString of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseHeadersString00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseHeadersString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getResponseHeadersString'", NULL);
#endif
 {
  std::string tolua_ret = (std::string)  self->getResponseHeadersString();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getResponseHeadersString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResponseString of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseData00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getResponseString'", NULL);
#endif
 {
  std::string tolua_ret = (std::string)  self->getResponseString();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getResponseData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasResponseData of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_hasResponseData00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_hasResponseData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasResponseData'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->hasResponseData();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasResponseData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getResponseDataLength of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseDataLength00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseDataLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getResponseDataLength'", NULL);
#endif
 {
  int tolua_ret = (int)  self->getResponseDataLength();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getResponseDataLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: saveResponseData of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_saveResponseData00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_saveResponseData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
  const char* filename = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'saveResponseData'", NULL);
#endif
 {
  int tolua_ret = (int)  self->saveResponseData(filename);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'saveResponseData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getErrorCode of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getErrorCode00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getErrorCode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getErrorCode'", NULL);
#endif
 {
  int tolua_ret = (int)  self->getErrorCode();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getErrorCode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getErrorMessage of class  CCHTTPRequest */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getErrorMessage00
static int tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getErrorMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"CCHTTPRequest",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  CCHTTPRequest* self = (CCHTTPRequest*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getErrorMessage'", NULL);
#endif
 {
  std::string tolua_ret = (std::string)  self->getErrorMessage();
 tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getErrorMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: createHTTPRequest of class  CCNetwork */
#ifndef TOLUA_DISABLE_tolua_cocos2dx_httprequest_luabinding_CCNetwork_createHTTPRequest00
static int tolua_cocos2dx_httprequest_luabinding_CCNetwork_createHTTPRequest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"CCNetwork",0,&tolua_err) ||
 !tolua_isstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const char* url = ((const char*)  tolua_tostring(tolua_S,2,0));
  int method = ((int)  tolua_tonumber(tolua_S,3,kCCHTTPRequestMethodGET));
 {
  CCHTTPRequest* tolua_ret = (CCHTTPRequest*)  CCNetwork::createHTTPRequest(url,method);
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCHTTPRequest");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createHTTPRequest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_cocos2dx_httprequest_luabinding_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_constant(tolua_S,"kCCHTTPRequestMethodGET",kCCHTTPRequestMethodGET);
 tolua_constant(tolua_S,"kCCHTTPRequestMethodPOST",kCCHTTPRequestMethodPOST);
 tolua_constant(tolua_S,"kCCHTTPRequestAcceptEncodingIdentity",kCCHTTPRequestAcceptEncodingIdentity);
 tolua_constant(tolua_S,"kCCHTTPRequestAcceptEncodingGzip",kCCHTTPRequestAcceptEncodingGzip);
 tolua_constant(tolua_S,"kCCHTTPRequestAcceptEncodingDeflate",kCCHTTPRequestAcceptEncodingDeflate);
 tolua_constant(tolua_S,"kCCHTTPRequestStateIdle",kCCHTTPRequestStateIdle);
 tolua_constant(tolua_S,"kCCHTTPRequestStateCleared",kCCHTTPRequestStateCleared);
 tolua_constant(tolua_S,"kCCHTTPRequestStateInProgress",kCCHTTPRequestStateInProgress);
 tolua_constant(tolua_S,"kCCHTTPRequestStateCompleted",kCCHTTPRequestStateCompleted);
 tolua_constant(tolua_S,"kCCHTTPRequestStateCancelled",kCCHTTPRequestStateCancelled);
 tolua_constant(tolua_S,"kCCHTTPRequestStateFailed",kCCHTTPRequestStateFailed);
 tolua_cclass(tolua_S,"CCHTTPRequest","CCHTTPRequest","CCObject",NULL);
 tolua_beginmodule(tolua_S,"CCHTTPRequest");
  tolua_function(tolua_S,"createWithUrl",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_createWithUrl00);
  tolua_function(tolua_S,"setRequestUrl",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setRequestUrl00);
  tolua_function(tolua_S,"getRequestUrl",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getRequestUrl00);
  tolua_function(tolua_S,"addRequestHeader",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addRequestHeader00);
  tolua_function(tolua_S,"addPOSTValue",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addPOSTValue00);
  tolua_function(tolua_S,"setPOSTData",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setPOSTData00);
  tolua_function(tolua_S,"addFormFile",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addFormFile00);
  tolua_function(tolua_S,"addFormContents",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_addFormContents00);
  tolua_function(tolua_S,"setCookieString",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setCookieString00);
  tolua_function(tolua_S,"getCookieString",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getCookieString00);
  tolua_function(tolua_S,"setAcceptEncoding",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setAcceptEncoding00);
  tolua_function(tolua_S,"setTimeout",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_setTimeout00);
  tolua_function(tolua_S,"start",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_start00);
  tolua_function(tolua_S,"cancel",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_cancel00);
  tolua_function(tolua_S,"getState",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getState00);
  tolua_function(tolua_S,"getResponseStatusCode",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseStatusCode00);
  tolua_function(tolua_S,"getResponseHeadersString",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseHeadersString00);
  tolua_function(tolua_S,"getResponseData",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseData00);
  tolua_function(tolua_S,"hasResponseData",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_hasResponseData00);
  tolua_function(tolua_S,"getResponseDataLength",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getResponseDataLength00);
  tolua_function(tolua_S,"saveResponseData",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_saveResponseData00);
  tolua_function(tolua_S,"getErrorCode",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getErrorCode00);
  tolua_function(tolua_S,"getErrorMessage",tolua_cocos2dx_httprequest_luabinding_CCHTTPRequest_getErrorMessage00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"CCNetwork","CCNetwork","",NULL);
 tolua_beginmodule(tolua_S,"CCNetwork");
  tolua_function(tolua_S,"createHTTPRequest",tolua_cocos2dx_httprequest_luabinding_CCNetwork_createHTTPRequest00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_cocos2dx_httprequest_luabinding (lua_State* tolua_S) {
 return tolua_cocos2dx_httprequest_luabinding_open(tolua_S);
};
#endif

