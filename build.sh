#!/bin/bash

# Default number of cores to use if not specified via comnad line
CORES=$(nproc --all)
# Directory where the application will be built
BUILDDIR=build
# clean
CLEAN=

# create the build directory for first time
if [ ! -d  "$BUILDDIR" ]; then
  mkdir $BUILDDIR
fi

if [[ "$2" == "clean" ]]; then
  CLEAN=$2
fi
cd $BUILDDIR

# Build the application

if [ -n "$1" ]; then
  CORES=$1
fi

# if [ -x "$(command -v clang)" ]; then
#   export CC=/usr/bin/clang
#   export CXX=/usr/bin/clang++
#   # export CXXFLAGS='-fuse-ld=lld'
#   # export CFLAGS='-fuse-ld=lld'
#   echo 'Using Clang'
# fi

# export CC=/usr/bin/gcc-
# export CXX=/usr/bin/g++-10

# cmake -DCMAKE_C_FLAGS="-fuse-ld=lld" -DCMAKE_CXX_FLAGS="-fuse-ld=lld" -DCMAKE_CXX_FLAGS="-fuse-ld=lld" ..
cmake ..
if [[ -n "$CLEAN" ]]; then
  make clean
fi


make -j $CORES
