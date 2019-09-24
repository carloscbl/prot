FROM phusion/baseimage:latest


RUN apt update && apt install -y \
    git \
    gcc \
    g++ \
    coreutils \
    make \
    cmake
    #libboost1.65-dev
    #Still looking fo the correct package

ADD ./ /opt/prot
WORKDIR /opt/prot

RUN ./build.sh

