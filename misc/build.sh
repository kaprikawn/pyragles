#!/bin/bash

SCRIPT_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
ROOT_DIR="$( cd $SCRIPT_DIR/.. ; pwd -P )"
BUILD_DIR=$ROOT_DIR/build
SRC_DIR=$ROOT_DIR/src

# echo script is $SCRIPT_DIR
# echo root is $ROOT_DIR
# echo build is $BUILD_DIR
# echo src is $SRC_DIR

DEBUG_BUILD=1
DEFAULT_FULLSCREEN_ONLY=0

IS_PYRA=0
STR=$( uname -a )
SUB=pyra
# if uname -a contains pyra then assume we're running on a pyra
if [[ "$STR" == *"$SUB"* ]]; then
  IS_PYRA=1
  DEFAULT_FULLSCREEN_ONLY=1
fi

BUILD_CALL="g++ $SRC_DIR/main_linux.cpp -o AlphaFlight"
if [[ $DEBUG_BUILD -eq 1 ]]; then
  BUILD_CALL="$BUILD_CALL -g -DDEBUG"
fi

if [[ $IS_PYRA -eq 1 ]]; then
  BUILD_CALL="$BUILD_CALL `pkg-config --cflags --libs sdl2 glesv2`"
else
  BUILD_CALL="$BUILD_CALL `sdl2-config --cflags --libs` -lGL"
fi

mkdir -p $BUILD_DIR

pushd $BUILD_DIR

eval $BUILD_CALL
echo $BUILD_CALL


popd

exit 0
