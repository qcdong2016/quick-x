
#ifndef __LUA_EXTRA_H_
#define __LUA_EXTRA_H_

#if __cplusplus
extern "C" {
#endif

#include "lauxlib.h"

void luaopen_lua_extensions(lua_State *L);
    
#if __cplusplus
}
#endif

#endif /* __LUA_EXTRA_H_ */
