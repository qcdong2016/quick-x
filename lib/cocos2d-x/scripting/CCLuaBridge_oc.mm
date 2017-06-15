

#include "scripting/CCLuaStack.h"
#import <Foundation/Foundation.h>

static void pushValue(lua_State *L, void *val)
{
    id oval = (id)val;
    if (oval == nil)
    {
        lua_pushnil(L);
    }
    else if ([oval isKindOfClass:[NSNumber class]])
    {
        NSNumber *number = (NSNumber *)oval;
        const char *numberType = [number objCType];
        if (strcmp(numberType, @encode(BOOL)) == 0)
        {
            lua_pushboolean(L, [number boolValue]);
        }
        else if (strcmp(numberType, @encode(int)) == 0)
        {
            lua_pushinteger(L, [number intValue]);
        }
        else
        {
            lua_pushnumber(L, [number floatValue]);
        }
    }
    else if ([oval isKindOfClass:[NSString class]])
    {
        lua_pushstring(L, [oval cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    else if ([oval isKindOfClass:[NSDictionary class]])
    {
        lua_newtable(L);
        
        for (id key in oval)
        {
            const char *key_ = [[NSString stringWithFormat:@"%@", key] cStringUsingEncoding:NSUTF8StringEncoding];
            lua_pushstring(L, key_);
            pushValue(L, [oval objectForKey:key]);
            lua_rawset(L, -3);
        }
        return;
    }
    else if ([oval isKindOfClass:[NSArray class]])
    {
        // added by guorui.chen
        lua_newtable(L);
        int count = [oval count];
        for (int i=0; i<count; ++i)
        {
            int len = lua_objlen(L, -1);
            pushValue(L, [oval objectAtIndex:i]);
            lua_rawseti(L, -2, len+1);
        }
        return;
    }
    else
    {
        lua_pushstring(L, [[NSString stringWithFormat:@"%@", oval] cStringUsingEncoding:NSUTF8StringEncoding]);
    }
}


int callStaticMethod(lua_State* L)
{
    if (!(lua_isstring(L, 1) && lua_isstring(L, 2)))
    {
        lua_pushboolean(L, false);
        lua_pushstring(L, "invalid param.");
        return 2;
    }
    
    const char *className  = lua_tostring(L, 1);
    const char *methodName = lua_tostring(L, 2);
    
    
    Class targetClass = NSClassFromString([NSString stringWithCString:className encoding:NSUTF8StringEncoding]);
    if (!targetClass)
    {
        lua_pushboolean(L, false);
        lua_pushstring(L, "class not found.");
        return 2;
    }
    
    SEL methodSel;
    bool hasArguments = lua_istable(L, 3);
    if (hasArguments)
    {
        NSString *methodName_ = [NSString stringWithCString:methodName encoding:NSUTF8StringEncoding];
        methodName_ = [NSString stringWithFormat:@"%@:", methodName_];
        methodSel = NSSelectorFromString(methodName_);
    }
    else
    {
        methodSel = NSSelectorFromString([NSString stringWithCString:methodName encoding:NSUTF8StringEncoding]);
    }
    
    if (methodSel == (SEL)0)
    {
        lua_pushboolean(L, false);
        lua_pushstring(L, "method not found.");
        return 2;
    }
    
    NSMethodSignature *methodSig = [targetClass methodSignatureForSelector:(SEL)methodSel];
    if (methodSig == nil)
    {
        lua_pushboolean(L, false);
        lua_pushstring(L, "cound not get method signature.");
        return 2;
    }
    
    @try {
        NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSig];
        [invocation setTarget:targetClass];
        [invocation setSelector:methodSel];
        NSUInteger returnLength = [methodSig methodReturnLength];
        const char *returnType = [methodSig methodReturnType];
        
        if (hasArguments)
        {
            NSMutableDictionary* d = [[NSMutableDictionary alloc] init];
            
            lua_pushnil(L);
            while (lua_next(L, -2))
            {
                NSString* key = [NSString stringWithCString:lua_tostring(L, -2) encoding:NSUTF8StringEncoding];
                switch (lua_type(L, -1))
                {
                    case LUA_TNUMBER:
                    {
                        lua_Number value = lua_tonumber(L, -1);
                        [d setValue:[NSNumber numberWithDouble:value] forKey:key];
                        break;
                    }
                    case LUA_TBOOLEAN:
                    {
                        bool value = !!lua_toboolean(L, -1);
                        [d setValue:[NSNumber numberWithDouble:value] forKey:key];
                        break;
                    }
                    case LUA_TSTRING:
                    {
                        const char* value = lua_tostring(L, -1);
                        NSString* str = [NSString stringWithCString:value encoding:NSUTF8StringEncoding];
                        [d setValue:str forKey:key];
                        break;
                    }
                    case LUA_TFUNCTION:
                    {
                        lua_pushvalue(L, -1);
                        int funcId = luaL_ref(L, LUA_REGISTRYINDEX);
                        [d setValue:[NSNumber numberWithInteger:funcId] forKey:key];
                        break;
                    }
                    case LUA_TNIL:
                    {
                        [d setValue:[[NSNull alloc] init] forKey:key];
                        break;
                    }
                    default:
                    {
                        cocos2d::CCLog("value type `%s` not support.", lua_typename(L, -1));
                        break;
                    }
                }
                lua_pop(L, 1);
            }
            
            [invocation setArgument:&d atIndex:2];
        }
        
        [invocation invoke];
        
        lua_pushboolean(L, 1);
        if (returnLength > 0)
        {
            if (strcmp(returnType, "@") == 0)
            {
                id ret;
                [invocation getReturnValue:&ret];
                pushValue(L, ret);
            }
            else if (strcmp(returnType, "c") == 0) // BOOL
            {
                char ret;
                [invocation getReturnValue:&ret];
                lua_pushboolean(L, ret);
            }
            else if (strcmp(returnType, "i") == 0) // int
            {
                int ret;
                [invocation getReturnValue:&ret];
                lua_pushinteger(L, ret);
            }
            else if (strcmp(returnType, "f") == 0) // float
            {
                float ret;
                [invocation getReturnValue:&ret];
                lua_pushnumber(L, ret);
            }
            else
            {
                NSLog(@"not support return type = %s", returnType);
                lua_pushnil(L);
            }
        }
        else
        {
            lua_pushnil(L);
        }
        return 2;
    }
    @catch (NSException *exception)
    {
        NSLog(@"EXCEPTION THROW: %@", exception);
        lua_pushboolean(L, 0);
        lua_pushstring(L, [[exception description] UTF8String]);
        return 2; 
    }
}
