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
    libmariadbclient-dev \
    && add-apt-repository -y ppa:kip/pistache-unstable \
    && add-apt-repository -y ppa:mhier/libboost-latest \
    && apt update \
    && apt install -y libpistache-dev libboost1.70-dev



COPY ./ /opt/prot
WORKDIR /opt/prot

RUN python3.7 -m pip install -r requirements.txt

RUN ./dependencies.sh --docker
RUN ./build.sh
WORKDIR /opt/prot/build
ENTRYPOINT [ "/bin/bash" ]
