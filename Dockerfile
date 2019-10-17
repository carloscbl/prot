FROM ubuntu:19.04


RUN apt update && apt install -y \
    git \
    gcc \
    g++ \
    coreutils \
    make \
    wget \
    cmake 
    #Still looking fo the correct package

ADD ./ /opt/prot
WORKDIR /opt/prot
RUN ./dependencies.sh
RUN ./build.sh
