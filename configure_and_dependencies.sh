#!/bin/bash
DIR2=$(pwd)
sudo add-apt-repository ppa:jonathonf/gcc-9.0
sudo apt-get install gcc-9

#sudo update-alternatives --remove-all gcc 
#sudo update-alternatives --remove-all g++

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 900 --slave /usr/bin/g++ g++ /usr/bin/g++-9

sh ./dependencies.sh

cd ${DIR2}
mkdir build
cd build
cmake ..
make
