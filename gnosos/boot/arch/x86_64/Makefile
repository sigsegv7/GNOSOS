#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

.PHONY: all
all: $(IMG_NAME)

.PHONY: $(IMG_NAME)
$(IMG_NAME): BOOTX64.EFI
	./Host/GenImg.sh $(IMG_NAME)

.PHONY: BOOTX64.EFI
BOOTX64.EFI: edk
	cp ../../../var/EDK2-fork/Build/MdeModule/DEBUG_GCC5/X64/Boot.efi  $@

.PHONY: edk
edk:
	rsync -avr * ../../../var/EDK2-fork/MdeModulePkg/Application/Boot
	cd ../../../var/EDK2-fork; \
		source ./edksetup.sh; \
    	build -a X64 -t GCC5 -p MdeModulePkg/MdeModulePkg.dsc -m MdeModulePkg/Application/Boot/Boot.inf
