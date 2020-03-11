FROM ubuntu:18.04

RUN apt-get -y update && DEBIAN_FRONTEND=noninteractive apt -y install binutils-aarch64-linux-gnu g++-aarch64-linux-gnu gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf make cmake gcc g++ wget curl
RUN curl -sL https://deb.nodesource.com/setup_13.x | bash - && apt -y install -y nodejs && npm i node-pre-gyp -g

