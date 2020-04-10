#!/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_DIR=$PWD
ASSETS_DIR_DEV=$PROJECT_DIR/src/assets
ASSETS_DIR_BUILD=$PROJECT_DIR/build/assets
MESON_BUILD=0
CLEAN_BUILD=0

let cores=`grep -c ^processor /proc/cpuinfo`

if [[ $# -gt 0 ]]; then
  for var in "$@"
  do
    if [[ $var == "--clean" ]]; then
      CLEAN_BUILD=1
    fi
    
    if [[ $var == "--meson" ]]; then
      MESON_BUILD=1
      ASSETS_DIR_BUILD=$PROJECT_DIR/builddir/src/assets
    fi
  done
fi

cd $PROJECT_DIR || exit 1

# if clean build then delete existing build dir
if [[ $CLEAN_BUILD -eq 1 ]]; then
  if [[ $MESON_BUILD -eq 1 ]]; then
    if [[ -d builddir ]]; then
      rm -rfv builddir
    fi
  else
    if [[ -d build ]]; then
      rm -rfv build
    fi
  fi
fi

if [[ ! -d $ASSETS_DIR_BUILD ]]; then
  mkdir -pv $ASSETS_DIR_BUILD
fi

# make sure all the assets dir files are correct in the build dir
for f in $ASSETS_DIR_DEV/*.*
do
  file=$(basename $f)
  if [[ -e $ASSETS_DIR_BUILD/$file ]]; then
    md5HashSrc=( $(md5sum $ASSETS_DIR_DEV/$file | cut -d ' ' -f 1 ) )
    md5HashDest=( $(md5sum $ASSETS_DIR_BUILD/$file | cut -d ' ' -f 1 ) )
    
    if [[ $md5HashSrc != $md5HashDest ]]; then
      echo $file is different - updating
      if [[ -e $ASSETS_DIR_BUILD/$file ]]; then
        rm -f $ASSETS_DIR_BUILD/$file
      fi
      cp -v $ASSETS_DIR_DEV/$file $ASSETS_DIR_BUILD/$file
    fi
  else
    cp -v $ASSETS_DIR_DEV/$file $ASSETS_DIR_BUILD/$file
  fi
done

if [[ $MESON_BUILD -eq 1 ]]; then
  # meson
  meson builddir
  cd builddir
  ninja
else
  # cmake
  cd build
  cmake ..
  make -j${cores}
fi

exit 0
