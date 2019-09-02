LDFLAGS=-nostdlib
OBJCOPY=objcopy

SRCS=$(wildcard *.c)
ASMS=$(wildcard *.s)
LDSCRIPT=linker.ld
OBJS=$(SRCS:.c=.o) $(ASMS:.s=.o)
TARGET=efi-stub

all: $(TARGET)

$(TARGET): $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) -Wl,-dT,$(call strip,$(LDSCRIPT)) $(OBJS) -o $@.o
	$(OBJCOPY) -O binary $@.o $@

clean:
	@rm -rf $(OBJS) $(TARGET)

.PHONY: clean
