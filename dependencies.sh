#!/bin/bash
sudo snap install cmake --classic
#required a cmake > 3.14
sudo apt install -y gcc-9 g++-9 make wget tar libmariadbclient-dev
cd thirdparty

if [[ ! -d boost_1_69_0 ]]; then
    wget https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz
    tar -xvf boost_1_69_0.tar.gz
    cd boost_1_69_0
    sudo ./bootstrap.sh 
    # #sudo ./bootstrap.sh --with-python=/usr/local/bin/python3 --with-python-version=3.7 --with-python-root=/usr/local/lib/python3.7 --prefix=/usr/
    sudo ./b2 clean
    sudo ./b2 --enable-unicode=ucs4 -j7 install
    cd ..
fi

#date
cd HowardHinnantDate/date
mkdir build
cd build
snap run cmake ..
make -j 5
sudo make install
cd ../../..

#sqlpp11
cd sql/sqlpp11
mkdir build
cd build
snap run cmake ..
make -j 5
sudo make install
cd ../../..

#connector
cd sql/sqlpp11-connector-mysql
mkdir build
cd build
snap run cmake -DUSE_MARIADB=1 - -DDATE_INCLUDE_DIR=/usr/local/include/ ..
make -j 5
sudo make install
cd ../../..