#!/bin/bash

apt -y update
apt -y dist-upgrade

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

function install_nodejs {
    version=$1
    apt -y remove nodejs
    curl -sL https://deb.nodesource.com/setup_${version}.x | bash - && apt -y install -y nodejs && npm i node-pre-gyp -g
}

function build {
    export arch=$1
    export node_version=$2
    export CC=${!arch}gcc
    export CXX=${!arch}g++
    export LD=${!arch}ld
    export STRIP=${!arch}strip

    dir=/build_${arch}_${node_version}
    mkdir $dir
    cd $dir
    cp ${src_dir}/* .
    npm install
    node-pre-gyp rebuild --build-from-source --target_arch=${arch}
    node-pre-gyp package --target_arch=${arch}
    find build -iname "*.tar.gz" -exec cp {} ${src_dir} \;
}

download_deb armv7l
dpkg -i *.deb && rm *.deb
for node_version in "12" "13";
do
    install_nodejs $node_version
    build arm $node_version
done
dpkg -r timeswipe

download_deb aarch64
dpkg -i *.deb && rm *.deb
for node_version in "12" "13";
do
    install_nodejs $node_version
    build arm64 $node_version
done
dpkg -r timeswipe

