#!/bin/bash

export timeswipe_uri=https://proxychain.io
src_dir=`dirname $0`

export arm=arm-linux-gnueabihf-
export arm64=aarch64-linux-gnu-

npm install npm -g
npm i node-pre-gyp -g

function download_deb {
    arch=$1
    curl -s https://api.github.com/repos/panda-official/TimeSwipe/releases/latest | grep "browser_download_url.*timeswipe.*$arch.deb" | cut -d : -f 2,3 | tr -d \"  | wget -qi -
}

function build {
    export arch=$1
    export CC=${!arch}gcc
    export CXX=${!arch}g++
    export LD=${!arch}ld
    export STRIP=${!arch}strip

    mkdir /build_${arch}
    cd /build_${arch}
    cp ${src_dir}/* .
    npm install
    node-pre-gyp rebuild --build-from-source --target_arch=${arch}
    node-pre-gyp package --target_arch=${arch}
    dpkg -r timeswipe
    ls build/stage/*.gz
    mv build/stage/*.gz ${src_dir}
}

#wget ${timeswipe_uri}/timeswipe_0.0.6.armv7l.deb
#dpkg -i timeswipe_0.0.6.armv7l.deb
download_deb armv7l
dpkg -i *.deb && rm *.deb
build arm
dpkg -r timeswipe

#wget ${timeswipe_uri}/timeswipe_0.0.6.aarch64.deb
#dpkg -i timeswipe_0.0.6.aarch64.deb
download_deb aarch64
dpkg -i *.deb && rm *.deb
build arm64
dpkg -r timeswipe

