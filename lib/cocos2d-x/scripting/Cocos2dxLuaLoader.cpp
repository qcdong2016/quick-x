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
#include "Cocos2dxLuaLoader.h"
#include <string>
#include <algorithm>

#include "CCLuaStack.h"
#include "IO/FileSystem.h"

using namespace cocos2d;

extern "C"
{
    int cocos2dx_lua_loader(lua_State *L)
    {
        std::string filename(luaL_checkstring(L, 1));

        lua_getglobal(L, kCCLuaDebuggerGlobalKey);
        if (lua_toboolean(L, -1))
        {
            return luaL_loadfile(L, filename.c_str()) == 0 ? 1 : 0;
        }
        lua_pop(L, 1);

        // replace . -> /
        for (unsigned i = 0; i < filename.size(); i++) {
            if (filename[i] == '.')
                filename[i] = '/';
        }

        std::string chunkName = FileSystem::join("scripts", filename) + ".lua";

		SharedPtr<MemBuffer> bf = FileSystem::readAll(chunkName);
        if (bf)
        {
			CCLuaStack::lua_loadbuffer(L, (char*)bf->getData(), (int)bf->getSize(), chunkName.c_str());
			return 1;
        }

        return 0;
    }
}
