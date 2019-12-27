FROM ubuntu:19.04

ARG NBUILDCORES=5
#--build-arg NBUILDCORES=12

RUN  apt update && apt install -y \
    software-properties-common \
    git \
    gcc-9 \
    g++-9 \
    gdb \
    coreutils \
    make \
    cmake \
    wget \
    tar \
    curl \
    cmake \
    python3.7 \
    python3-pip \
    libmariadbclient-dev \
    && add-apt-repository -y ppa:kip/pistache-unstable \
    && add-apt-repository -y ppa:mhier/libboost-latest \
    && apt update && apt install -y libpistache-dev libboost1.70-dev



COPY ./ /opt/prot
WORKDIR /opt/prot
RUN dpkg -i ./thirdparty/cmake-data_3.15.4-1ubuntu2_all.deb \
&& dpkg -i ./thirdparty/cmake_3.15.4-1ubuntu2_amd64.deb\
&& apt-get install -f

RUN python3.7 -m pip install -r requirements.txt

WORKDIR /opt/prot/thirdparty/HowardHinnantDate/date/build
#date
RUN rm -rf * \
&& cmake .. \
&& make -j $NBUILDCORES \
&& make install

WORKDIR /opt/prot/thirdparty/sql/sqlpp11/build
#sqlpp11
RUN rm -rf * \
&& cmake .. \
&& make -j $NBUILDCORES \
&& make install

WORKDIR /opt/prot/thirdparty/sql/sqlpp11-connector-mysql/build
#connector
RUN rm -rf * \
&& cmake -DUSE_MARIADB=1 - -DDATE_INCLUDE_DIR=/usr/local/include/ .. \
&& make -j $NBUILDCORES \
&& make install

WORKDIR /opt/prot/
RUN ./build.sh
WORKDIR /opt/prot/build
ENTRYPOINT [ "/bin/bash" ]
