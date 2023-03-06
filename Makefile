export V?=0

# If _HOST or _TA specific compilers are not specified, then use CROSS_COMPILE
TEEC_EXPORT ?= /home/spereira/Projects/TEEOD/teeod-lib/
HOST_CROSS_COMPILE ?= /home/spereira/Projects/tools/Xilinx/Vitis/2022.2/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-
TA_CROSS_COMPILE ?= /usr/bin/arm-none-eabi-

.PHONY: all
all:
	$(MAKE) -C host CROSS_COMPILE="$(HOST_CROSS_COMPILE)" --no-builtin-variables
	$(MAKE) -C ta CROSS_COMPILE="$(TA_CROSS_COMPILE)"

.PHONY: clean
clean:
	$(MAKE) -C host clean
	$(MAKE) -C ta clean