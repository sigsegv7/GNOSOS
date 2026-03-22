#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

include ../mk/default.mk

.PHONY: all
all: boot

.PHONY: boot
boot:
	cd boot/arch/$(ARCH); make $(BOOT_PASSDOWN_ARGS)
