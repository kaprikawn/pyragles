#!/bin/sh

cd ..

if [[ ! -d buillder ]]
then
  meson builddir
fi

cd builddir

ninja

cd src

if [[ ! -d assets ]]
then
  mkdir assets
fi

cp ../../src/assets/* assets

./exe

exit 0

