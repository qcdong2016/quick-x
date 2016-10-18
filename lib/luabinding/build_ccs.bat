@echo off
set DIR=%~dp0
set TOLUA=tolua++.exe

cd /d "%DIR%"
%TOLUA% -L "%DIR%basic_ccs.lua" -o "%QUICKX_ROOT%\lib\cocos2d-x\scripting\cocoslua\LuaCocoStudio.cpp" CocoStudio.tolua
