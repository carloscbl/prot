FROM ubuntu:19.04


RUN apt update && apt install -y \
    git \
    gcc \
    g++ \
    coreutils \
    make \
    wget \
    curl \
    cmake \
    python3.7 \
    python3.7 \
    python3-pip \
    libmariadbclient-dev libcpprest-dev libpistache-dev

RUN curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
RUN python3.7 get-pip.py
RUN python3.7 -m pip install Pyparsing

COPY ./ /opt/prot
WORKDIR /opt/prot
RUN ./dependencies.sh
RUN ./build.sh

