FROM ubuntu:19.04


RUN apt update && apt install -y \
    git \
    gcc \
    g++ \
    coreutils \
    make \
    wget \
    cmake \
    libmariadbclient-dev

RUN curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
RUN python3 get-pip.py
RUN python3 -m pip install Pyparsing

ADD ./ /opt/prot
WORKDIR /opt/prot
RUN ./dependencies.sh
RUN ./build.sh

