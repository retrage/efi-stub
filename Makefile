HOSTCC=gcc
CFLAGS=-Iinclude
LDFLAGS=-nostdlib
NM=nm
OBJCOPY=objcopy

SRCS=$(wildcard *.c)
ASMS=$(wildcard *.s)
LDSCRIPT=linker.ld
OBJS=$(SRCS:.c=.o) $(ASMS:.s=.o)
TOOLS=tools/build
TARGET=efi-stub

all: $(TARGET)

$(TARGET): $(TARGET).efi $(TARGET).sym $(TOOLS)
	$(TOOLS) $(TARGET).sym $(TARGET).efi $@

$(TARGET).sym: $(TARGET).o
	$(NM) -n $< > $@

$(TARGET).efi: $(TARGET).o
	$(OBJCOPY) -O binary $< $@

$(TARGET).o: $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) -Wl,-dT,$(call strip,$(LDSCRIPT)) $(OBJS) -o $@

$(TOOLS): $(TOOLS).c
	$(HOSTCC) $(CFLAGS) $^ -o $@

clean:
	@rm -rf $(TOOLS) $(TARGET) *.efi *.o *.sym

.PHONY: clean
