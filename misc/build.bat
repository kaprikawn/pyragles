@echo off
SET SCRIPT_DIR=%~dp0

pushd %SCRIPT_DIR%
cd ..
SET ROOT_DIR=%cd%
popd

SET BUILD_DIR=%ROOT_DIR%\build

SET SDL_ROOT_DIR=C:\dev\SDL2-2.0.14
rem SET GLUT_ROOT_DIR=C:\dev\freeglut-3.2.1
SET GLEW_ROOT_DIR=C:\dev\glew-2.2.0
SET VCVARS_DIR=C:\"Program Files (x86)"\"Microsoft Visual Studio"\2019\Community\VC\Auxiliary\Build\

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

pushd %BUILD_DIR%

call %VCVARS_DIR%\vcvarsall.bat x64

cl -Zi /EHsc -wd4312 -DDEBUG=1 ..\src\main_windows.cpp user32.lib gdi32.lib Shlwapi.lib opengl32.lib /I%SDL_ROOT_DIR%\include /I%GLEW_ROOT_DIR%/include /link /LIBPATH:%SDL_ROOT_DIR%\lib\x64 /LIBPATH:%GLEW_ROOT_DIR%\lib\Release\x64 SDL2.lib SDL2main.lib glew32.lib /OUT:AlphaFlight.exe

if not exist %BUILD_DIR%\SDL2.dll echo f | xcopy %SDL_ROOT_DIR%\lib\x64\SDL2.dll %BUILD_DIR%\SDL2.dll
if not exist %BUILD_DIR%\glew32.dll echo f | xcopy %GLEW_ROOT_DIR%\bin\Release\x64\glew32.dll %BUILD_DIR%\glew32.dll

popd
