#!/bin/bash

SUDO=sudo
if [ "$1" == "--docker" ]; then
    SUDO=""
fi
$SUDO add-apt-repository -y ppa:kip/pistache
$SUDO apt update && $SUDO apt install -y gcc-9 g++-9 make wget tar python3.7 python3-pip libboost1.67-dev libmariadbclient-dev libpistache-dev
cd thirdparty

if [[ "$1" == *"--custom-boost"* ]]; then
    if [[ ! -d boost_1_69_0 ]]; then
        wget https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz
        tar -xvf boost_1_69_0.tar.gz
        cd boost_1_69_0
        $SUDO ./bootstrap.sh 
        # #$SUDO ./bootstrap.sh --with-python=/usr/local/bin/python3 --with-python-version=3.7 --with-python-root=/usr/local/lib/python3.7 --prefix=/usr/
        $SUDO ./b2 clean
        $SUDO ./b2 --enable-unicode=ucs4 -j7 install
        cd ..
    fi
fi

#date
cd HowardHinnantDate/date
mkdir build
cd build
cmake ..
make -j 5
$SUDO make install
cd ../../..

#sqlpp11
cd sql/sqlpp11
mkdir build
cd build
cmake ..
make -j 5
$SUDO make install
cd ../../..

#connector
cd sql/sqlpp11-connector-mysql
mkdir build
cd build
cmake -DUSE_MARIADB=1 - -DDATE_INCLUDE_DIR=/usr/local/include/ ..
make -j 5
$SUDO make install
cd ../../..

# Get deps for python parser
python3 -m pip install -r ../requirements.txt

# Get docker to build openapi generation
# https://hub.docker.com/r/openapitools/openapi-generator-cli/
# https://github.com/OpenAPITools/openapi-generator#16---docker
# One option is the "Codewind OpenAPI Tools" extension for visual code from IBM
# But better is the rebuild_api.sh