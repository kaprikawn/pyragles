@echo off

SET VCVARS_DIR=C:\"Program Files (x86)"\"Microsoft Visual Studio"\2019\Community\VC\Auxiliary\Build\
SET GAME_DIR=C:\dev\AlphaFlight

rem add scripts dir to path
set path=%GAME_DIR%\misc;%path%

rem setup build environment
call %VCVARS_DIR%\vcvarsall.bat x64

rem change into game dir
C:
cd \dev\AlphaFlight
