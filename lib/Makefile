#
# lib/Makefile
#
PROG = myBinder

BIN = ../bin
CC = gcc

CFLAGS = -Os -O2 -Wall -g \
	$(SOURCE_FLAGS) \
	-I../include \
	-I../res
# -D NDEBUG

OBJS = \
	run.o \
	zip.o


.c.o:
	$(CC) $(CFLAGS) -c $*.c

all: $(OBJS)

clean:
	rm -f $(OBJS)
