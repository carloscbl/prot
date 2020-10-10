FROM ubuntu:19.10 as min-stage

ARG NBUILDCORES=3
#--build-arg NBUILDCORES=12

RUN apt update && apt install -y \
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
    libcurl4-openssl-dev \
    cmake \
    python3.7 \
    python3-pip \
    libmariadbclient-dev \
    && add-apt-repository -y ppa:pistache+team/unstable \
    && add-apt-repository -y ppa:mhier/libboost-latest \
    && apt update && apt install -y libpistache-dev libboost1.70-dev
    #&& apt upgrade -y

FROM min-stage as build-stage

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


FROM min-stage as production-stage
COPY  --from=build-stage /opt/prot/build/api-server /opt/prot/build/api-server
COPY  --from=build-stage /opt/prot/build/jobs-watcher /opt/prot/build/jobs-watcher
COPY  --from=build-stage /opt/prot/build/test_prot /opt/prot/build/test_prot
COPY  --from=build-stage /opt/prot/build/liblprot.a /opt/prot/build/liblprot.a
COPY  --from=build-stage /opt/prot/source /opt/prot/source
COPY  --from=build-stage /opt/prot/jobs_watcher /opt/prot/jobs_watcher
COPY  --from=build-stage /opt/prot/rest_server /opt/prot/rest_server
COPY  --from=build-stage /opt/prot/api /opt/prot/api
COPY  --from=build-stage /opt/prot/.vscode /opt/prot/.vscode
COPY  --from=build-stage /opt/prot/apps /opt/prot/apps



RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install tzdata

ENV TZ=Etc/UTC
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN dpkg-reconfigure --frontend noninteractive tzdata

WORKDIR /opt/prot/build
ENTRYPOINT [ "/bin/bash" ]
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*