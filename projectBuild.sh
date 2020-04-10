#!/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_DIR=$PWD
ASSETS_DIR_DEV=$PROJECT_DIR/src/assets
ASSETS_DIR_BUILD=$PROJECT_DIR/build/assets

let cores=`grep -c ^processor /proc/cpuinfo`

cd $PROJECT_DIR || exit 1

if [[ $# -gt 0 ]]; then
  for var in "$@"
  do
    if [[ $var == "--clean" ]]; then
      if [[ -d build ]]; then
        rm -rfv build
      fi
    fi
  done
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

cd build || exit 1

cmake .. || exit 1

if [[ $cores -gt 1 ]]
then
  make -j${cores} || exit 1
else
  make || exit 1
fi

exit 0
