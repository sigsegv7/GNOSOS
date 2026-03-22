#!/bin/sh

#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

ARCH=$1

build_edk2() {
    if [ ! -d var/edk2 ]; then
        git clone https://git.mirocom.org/Mirocom/EDK2-fork var/EDK2-fork
        cd var/EDK2-fork
        git submodule update --init
        make -C BaseTools/Source/C
        cp -r ../../boot/arch/$ARCH/ MdeModulePkg/Application/Boot

        export EDK_TOOLS_PATH=$PWD/BaseTools
        export WORKSPACE=$PWD
        export EDK_TOOLS_PATH="$PWD/BaseTools"
        export PACKAGES_PATH="$PWD/edk2:$PWD/edk2-platforms:$PWD/edk2-non-osi"
        export CONF_PATH=$(pwd)/Conf
        source ./edksetup.sh

        cd ../../
    fi
}

get_deps() {
    build_edk2
}

mkdir -p var/
get_deps
