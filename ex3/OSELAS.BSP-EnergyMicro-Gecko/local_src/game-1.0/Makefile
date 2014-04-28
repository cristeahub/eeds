#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CC = arm-cortexm3-uclinuxeabi-gcc
CFLAGS += -O2 -g -Wall -W -std=c99 #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
LDFLAGS += -lm

OBJECTS = gfx.o pieces.o digit.o

all: game

game: main.c ${OBJECTS}
	${CC} ${CFLAGS} main.c ${OBJECTS} -o game ${LDFLAGS}

clean:
	-rm -f main *.o

install:


.PHONY: all install clean

# end of development makefile
