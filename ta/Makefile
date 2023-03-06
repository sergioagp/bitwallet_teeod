# Project name and directories
repo-dir-app := $(shell pwd)
TA_DEV_KIT_PATH := /home/spereira/Projects/TEEOD/ta_dev
CROSS_COMPILE ?= /usr/bin/arm-none-eabi-

target-app ?= 614789f2-39c0-4ebf-b235-92b32ac107ed
build-dir-app ?= $(repo-dir-app)/build

-include sub.mk
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