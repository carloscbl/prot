#!/bin/bash
PARAMS=$1
./build.sh \
&& echo $PARAMS \
&& cd build \
&& PROT_DB_PASSW=Prot#654321 ./test_prot $PARAMS
#&& python3 prepare_test_env.py \

#export $(grep -v '^#' .env | xargs)