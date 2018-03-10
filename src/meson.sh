#!/bin/sh

cd ..

if [[ ! -d builddir ]]
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

exit 0

