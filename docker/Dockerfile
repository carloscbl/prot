FROM ubuntu:19.04

RUN apt-get update && apt install -y \
apt-utils \
git \
build-essential \
make cmake \
libboost1.67-dev \
libboost-test1.67-dev \
libboost-test-dev \
libboost-program-options1.67-dev

#alias docker_run_compilation='docker run -ti --rm --name ${USER}_$(date +%Y%m%d_%H%M%S) -e HOME -e USER -v /etc/passwd:/etc/passwd:ro -v /etc/shadow:/etc/shadow:ro -v /etc/group:/etc/group:ro -v /etc/localtime:/etc/localtime:ro -v /etc/timezone:/etc/timezone:ro -v ${HOME}:${HOME} -v /opt/workspace:/opt/workspace -w /opt/workspace/${USER} -u ${UID}:${GID} compilation /bin/bash'