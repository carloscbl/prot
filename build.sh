#!/bin/bash

# Default number of cores to use if not specified via comnad line
CORES=7
# Directory where the application will be built
BUILDDIR=build

# Recreate the build directory

if [ -d "$BUILDDIR" ]; then
  rm -rf $BUILDDIR
fi
mkdir $BUILDDIR
cd $BUILDDIR

# Build the application

if [ -n "$1" ]; then
  CORES=$1
fi

cmake ..
make -j $CORES
