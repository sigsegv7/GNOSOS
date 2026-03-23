#!/bin/sh

#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

ARCH=x86_64

#
# Ensure that a list of programs are installed on the
# system
#
# <@>:  Program list
#
check_deps() {
    for dep in $@; do
        printf "Checking if $dep is installed... "
        which $dep &>/dev/null
        if [ $? -ne 0 ]; then
            echo "no"
            echo "Please install $dep!"
            exit 1
        fi

        echo "yes"
    done
}

#
# Bootstrap the OS
#
bootstrap_gnosos() {
    pushd gnosos/
    ./host/bootstrap.sh $ARCH
    popd
}

#
# Bootstrap all sub-projects
#
bootstrap_subproj() {
    bootstrap_gnosos
}

check_deps      \
    xorriso     \
    rsync       \
    git         \
    make

bootstrap_subproj
