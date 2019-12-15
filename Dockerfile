FROM ubuntu:19.04


RUN apt update && apt install -y \
    software-properties-common \
    git \
    gcc-9 \
    g++-9 \
    coreutils \
    make \
    cmake \
    wget \
    tar \
    curl \
    cmake \
    python3.7 \
    python3-pip \
    libboost1.67-dev libmariadbclient-dev \
    && add-apt-repository -y ppa:kip/pistache-unstable \
    && apt update \
    && apt install -y libpistache-dev



COPY ./ /opt/prot
WORKDIR /opt/prot

RUN python3.7 -m pip install -r requirements.txt

RUN ./dependencies.sh --docker
RUN ./build.sh

