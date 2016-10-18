#!/bin/bash
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
TOLUA="$QUICKX_ROOT/bin/mac/tolua++"

cd "$DIR/"
${TOLUA} -L "$DIR/basic.lua" -o "$QUICKX_ROOT/lib/cocos2d-x/scripting/LuaCocos2d.cpp" Cocos2d.tolua
