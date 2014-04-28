all: geckoboot.bin

ASFLAGS += -Wa,-mimplicit-it=always

%.bin: %.o
	$(OBJCOPY) -O binary $< $@

clean:
	-rm geckoboot.bin
	-rm geckoboot.o

install:

.PHONY: all clean install
.PRECIOUS: geckoboot.o
