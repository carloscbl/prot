FROM alpine:edge as basestep

# install ca-certificates so that HTTPS works consistently
RUN apk add --no-cache ca-certificates

RUN apk update && apk add --no-cache --virtual \
    bash-completion \
    git \
    gcc \
    g++ \
    coreutils \
    make \
    cmake \
    boost-dev
#libboost1.67-dev \
#libboost-test1.67-dev \
#libboost-test-dev \
#libboost-program-options1.67-dev

#RUN mount --bind ./ /opt/prot 

#ADD ./ /opt/prot
WORKDIR /opt/prot

#alias docker_run_compilation='docker run -ti --rm --name ${USER}_$(date +%Y%m%d_%H%M%S) -e HOME -e USER -v /etc/passwd:/etc/passwd:ro -v /etc/shadow:/etc/shadow:ro -v /etc/group:/etc/group:ro -v /etc/localtime:/etc/localtime:ro -v /etc/timezone:/etc/timezone:ro -v ${HOME}:${HOME} -v /opt/workspace:/opt/workspace -w /opt/workspace/${USER} -u ${UID}:${GID} compilation /bin/bash'

#EXPOSE 5050
#docker run -p 8888:5000 yourusername/catnip

#docker run --rm -it -v ~/prot/:/opt/prot  docker:latest rm build/* && cmake .. && make -j 7