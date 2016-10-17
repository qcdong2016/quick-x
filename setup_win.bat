@echo off
set QUICKX_ROOT=%~dp0

set SETX=setx
ver | find "XP" > nul
if %ERRORLEVEL% == 0 (
    set SETX=%QUICKX_ROOT%\bin\win32\setx.exe
)


echo.
echo config:
echo.
echo QUICKX_ROOT = "%QUICKX_ROOT%"
echo.

%SETX% QUICKX_ROOT %QUICKX_ROOT%

echo.

pause

