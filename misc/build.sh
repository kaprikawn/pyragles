#!/bin/bash

SCRIPT_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
ROOT_DIR="$( cd $SCRIPT_DIR/.. ; pwd -P )"
BUILD_DIR=$ROOT_DIR/build
SRC_DIR=$ROOT_DIR/src

echo script is $SCRIPT_DIR
echo root is $ROOT_DIR
echo build is $BUILD_DIR
echo src is $SRC_DIR

mkdir -p $BUILD_DIR

pushd $BUILD_DIR

g++ $SRC_DIR/main_linux.cpp -o AlphaFlight -g `sdl2-config --cflags --libs` -lGL

popd

exit 0

