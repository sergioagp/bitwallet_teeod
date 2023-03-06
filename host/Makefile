TEEC_EXPORT ?= /home/spereira/Projects/TEEOD/teeod-lib/
CROSS_COMPILE ?= /home/spereira/Projects/tools/Xilinx/Vitis/2022.2/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-

CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

OBJS = main.o

CFLAGS += -Wall -I../ta/include -I$(TEEC_EXPORT) -I./include
#Add/link other required libraries here
LDADD += -lteeodc -L$(TEEC_EXPORT)

BINARY = helloworld_example

.PHONY: all
all: $(BINARY)
	@echo "$< build complete"

$(BINARY): $(OBJS)
	@echo "Linking $(BINARY)..."
	@$(CC) $(LDFLAGS) -o $@ $< $(LDADD)

.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -f $(OBJS) $(BINARY)

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@
