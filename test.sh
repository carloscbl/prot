#!/bin/bash
PARAMS=$1
./build.sh

echo $PARAMS
cd build
./test_prot $PARAMS