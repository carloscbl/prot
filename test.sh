#!/bin/bash
PARAMS=$1
./build.sh

echo $PARAMS
build/test/test_prot $PARAMS