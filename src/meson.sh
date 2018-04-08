#!/bin/sh

cd ..

if [[ -d builddir ]]
then
  #rm -rf builddir
fi

if [[ ! -d buillder ]]
then
  meson builddir
fi

cd builddir

ninja

cd src

if [[ ! -d shaders ]]
then
  mkdir shaders
fi

cp ../../src/shaders/* shaders

./exe

exit 0

