#!/bin/bash

SCRIPT_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
ROOT_DIR="$( cd $SCRIPT_DIR/.. ; pwd -P )"
BUILD_DIR=$ROOT_DIR/build
SRC_DIR=$ROOT_DIR/src
SDL_BUILD_DIR=$HOME/build/SDL2_PYRA

# echo script is $SCRIPT_DIR
# echo root is $ROOT_DIR
# echo build is $BUILD_DIR
# echo src is $SRC_DIR

DEBUG_BUILD=1
DEFAULT_FULLSCREEN_ONLY=0
INPUT_DEBUG=1

IS_PYRA=0
STR=$( uname -a )
SUB=pyra
# if uname -a contains pyra then assume we're running on a pyra
if [[ "$STR" == *"$SUB"* ]]; then
  IS_PYRA=1
  DEFAULT_FULLSCREEN_ONLY=1
fi

if [[ $IS_PYRA -eq 1 ]]; then
  # have to build SDL2 version 0.9
  SDL_VERSION=2.0.9
  mkdir -pv $SDL_BUILD_DIR
  cd $SDL_BUILD_DIR || exit 1
  if [[ ! -d build/.libs ]]; then
    if [[ ! -e SDL2-${SDL_VERSION}.tar.gz ]]; then
      echo Downloading SDL2 source
      wget https://www.libsdl.org/release/SDL2-${SDL_VERSION}.tar.gz
    fi
    tar zxf SDL2-${SDL_VERSION}.tar.gz --strip-components=1
    echo "Configuring SDL2"
    ./configure
    echo "Building SDL2"
    make -j2
  fi
fi

BUILD_CALL="g++ $SRC_DIR/main_linux.cpp -o AlphaFlight"
if [[ $DEBUG_BUILD -eq 1 ]]; then
  BUILD_CALL="$BUILD_CALL -g -DDEBUG"
fi

if [[ $DEFAULT_FULLSCREEN_ONLY -eq 1 ]]; then
  BUILD_CALL="$BUILD_CALL -DFULLSCREEN_ONLY"
fi

if [[ $INPUT_DEBUG -eq 1 ]]; then
  BUILD_CALL="$BUILD_CALL -DINPUT_DEBUG"
fi

mkdir -pv $BUILD_DIR

if [[ $IS_PYRA -eq 1 ]]; then
  BUILD_CALL="$BUILD_CALL -L${SDL_BUILD_DIR}/build/.libs -L/opt/omap5-sgx-ddk-um-linux/lib -D_REENTRANT -I${SDL_BUILD_DIR}/include -lSDL2 -lGLESv2"
  
  if [[ ! -e $BUILD_DIR/run.sh ]]; then
    echo '#!/bin/bash' > $BUILD_DIR/run.sh
    echo 'export SDL_OPENGL_ES_DRIVER=1 SDL_VIDEO_EGL_DRIVER=/opt/omap5-sgx-ddk-um-linux/lib/libEGL.so.1' >>  $BUILD_DIR/run.sh
    echo './AlphaFlight' >> $BUILD_DIR/run.sh
    chmod +x $BUILD_DIR/run.sh
  fi

else
  BUILD_CALL="$BUILD_CALL `sdl2-config --cflags --libs` -lGL"
fi

pushd $BUILD_DIR

eval $BUILD_CALL
echo $BUILD_CALL


popd

exit 0
