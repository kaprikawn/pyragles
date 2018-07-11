#!/bin/sh

cd ..

if [[ ! -d buillder ]]
then
  meson builddir || exit 1
fi

cd builddir

ninja || exit 1

cd src

if [[ ! -d assets ]]
then
  mkdir assets
fi

cp ../../src/assets/* assets

./exe

exit 0

