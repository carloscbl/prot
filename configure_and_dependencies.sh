#!/bin/bash
DIR2=$(pwd)

sh ./dependencies.sh

cd ${DIR2}
sh ./build.sh
