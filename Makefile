# SPDX-License-Identifier: MIT

HOSTCC=gcc
HOSTCFLAGS=-Iinclude
CC=gcc
CFLAGS=-Wall -Werror -mno-red-zone -fno-stack-protector -fshort-wchar \
	   -Iinclude/edk2/MdePkg/Include \
	   -Iinclude/edk2/MdePkg/Include/X64 \
	   -Iinclude
LDFLAGS=-nostdlib
NM=nm
OBJCOPY=objcopy

SRCS=$(wildcard *.c)
ASMS=$(wildcard *.s)
LDSCRIPT=linker.ld
OBJS=$(SRCS:.c=.o) $(ASMS:.s=.o)
TOOLS=tools/build
TARGET=efi-stub

all: $(TARGET).efi

$(TARGET).efi: $(TARGET).bin $(TARGET).sym $(TOOLS)
	$(TOOLS) $(TARGET).sym $(TARGET).bin $@

$(TARGET).sym: $(TARGET).o
	$(NM) -n $< > $@

$(TARGET).bin: $(TARGET).o
	$(OBJCOPY) -O binary $< $@

$(TARGET).o: $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) -Wl,-dT,$(call strip,$(LDSCRIPT)) $(OBJS) -o $@

$(TOOLS): $(TOOLS).c
	$(HOSTCC) $(HOSTCFLAGS) $^ -o $@

clean:
	@rm -rf $(TOOLS) *.bin *.efi *.o *.sym

.PHONY: clean
