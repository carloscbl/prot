#!/bin/bash
sudo apt install -y gcc-9 g++-9 make cmake wget tar libbson-dev libmongoc-dev libicu-dev
cd thirdparty
wget https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz
tar -xvf boost_1_69_0.tar.gz
cd boost_1_69_0
sudo ./bootstrap.sh 
#sudo ./bootstrap.sh --with-python=/usr/local/bin/python3 --with-python-version=3.7 --with-python-root=/usr/local/lib/python3.7 --prefix=/usr/
sudo ./b2 clean
sudo ./b2 --enable-unicode=ucs4 -j7 install

#For build mongocxx driver
# cd ..
# git clone https://github.com/mongodb/mongo-cxx-driver.git     --branch releases/stable --depth 1
# cd mongo-cxx-driver/build/
# cmake -DCMAKE_BUILD_TYPE=Release -DBSONCXX_POLY_USE_BOOST=1     -DCMAKE_INSTALL_PREFIX=/usr/local ..
# sudo make && sudo make install