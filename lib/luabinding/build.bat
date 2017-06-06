@echo off
set DIR=%~dp0
set TOLUA=tolua++.exe

cd /d "%DIR%"
%TOLUA% -L "%DIR%basic.lua" -o "..\cocos2d-x\scripting\LuaCocos2d.cpp" Cocos2d.tolua
