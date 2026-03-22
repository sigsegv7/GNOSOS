#!/bin/sh

#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

dd if=/dev/zero of=$1 bs=1k count=1440
mformat -i $1 -f 1440 ::
mmd -i $1 ::/EFI
mmd -i $1 ::/EFI/BOOT
mcopy -i $1 BOOTX64.EFI ::/EFI/BOOT
