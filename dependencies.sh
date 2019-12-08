#!/bin/bash
sudo snap install cmake --classic
#required a cmake > 3.14
sudo add-apt-repository ppa:kip/pistache
sudo apt update
sudo apt install -y gcc-9 g++-9 make wget tar python3-pip libmariadbclient-dev libcpprest-dev libpistache-dev
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

# Get deps for python parser
python3 -m pip install -r ../requirements.txt

# Construct ORM objs
cd ../api/db_schemas
python3 /usr/local/bin/sqlpp11-ddl2cpp test_prot.sql ../generated/test_prot test_prot

# Get docker to build openapi generation
# https://hub.docker.com/r/openapitools/openapi-generator-cli/
# https://github.com/OpenAPITools/openapi-generator#16---docker
# The best option is the "Codewind OpenAPI Tools" extension for visual code from IBM

docker run --rm -u $(id -u ${USER}):$(id -g ${USER}) -v ${PWD}:/local openapitools/openapi-generator-cli generate \
    -i /local/api/prot.v1.yaml \
    -g cpp-pistache-server \
    -o /local/rest_server