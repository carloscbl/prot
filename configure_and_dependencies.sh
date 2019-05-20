#!/bin/bash
DIR2=$(pwd)

sh ./dependencies.sh

cd ${DIR2}
mkdir build
cd build
cmake ..
make
