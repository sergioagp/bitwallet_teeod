TEEC_EXPORT ?= /home/spereira/Projects/TEEOD/teeod-lib/
CROSS_COMPILE ?= /home/spereira/Projects/tools/Xilinx/Vitis/2022.2/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-

CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

SRCS = main.c


CFLAGS += -Wall -I../ta/include -I$(TEEC_EXPORT) -I./include -I.
#Add/link other required libraries here
LDADD += -lteeodc -L$(TEEC_EXPORT) -lm

# Extract the parent directory name as the binary name
DIR := $(shell dirname "$(CURDIR)")
PARENT_DIR := $(shell basename "$(DIR)")

BINARY := $(PARENT_DIR).elf

OBJS := $(SRCS:.c=.o)

.PHONY: all
all: $(BINARY)
	@echo "$< build complete"

$(BINARY): $(OBJS)
	@echo "Linking $(BINARY)..."
	@$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDADD)

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -f $(OBJS) $(BINARY)

.PHONY: copy
copy: $(BINARY)
	@echo "Copying $(BINARY) to the $(SD_CARD_PATH)"
	@cp $(BINARY) $(SD_CARD_PATH)
