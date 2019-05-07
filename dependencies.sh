#!/bin/bash
cd thirdparty
wget https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz
tar -xvf boost_1_69_0.tar.gz
cd boost_1_69_0
./bootstrap.sh --prefix=/usr/
./b2 -j 7 
sudo ./b2 -j 7 install
