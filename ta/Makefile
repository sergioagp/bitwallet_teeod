# Project name and directories
repo-dir-app := $(shell pwd)
TA_DEV_KIT_PATH := /home/spereira/Projects/TEEOD/ta_dev
CROSS_COMPILE ?= /usr/bin/arm-none-eabi-

target-app ?= $(local_module)
build-dir-app ?= $(repo-dir-app)/build

-include sub.mk Android.mk
srcs-app	:= $(srcs-y)
inc-app		:= $(global-incdirs-y) 
export srcs-app inc-app build-dir-app target-app repo-dir-app


# Make targets
.PHONY: all
all:
	@$(MAKE) -C $(TA_DEV_KIT_PATH) CROSS_COMPILE=$(CROSS_COMPILE)

.PHONY: clean
clean:
	@$(MAKE) -C $(TA_DEV_KIT_PATH) clean

.PHONY: copy
copy: $(BINARY)
	@echo "Copying $(BINARY) to the $(SD_CARD_PATH)"
	@cp $(build-dir-app)/$(local_module).bin $(SD_CARD_PATH)
