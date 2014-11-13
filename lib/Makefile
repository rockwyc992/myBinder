#
# lib/Makefile
#
PROG = myBinder

DLL = lib$(PROG).dll
BIN = ../bin
CC = gcc

CFLAGS = -Os -O2 -Wall \
	$(SOURCE_FLAGS) -g \
	-I../include \
	-I../res
# -D NDEBUG

OBJS = \
	dir.o \
	zip.o


.c.o:
	$(CC) $(CFLAGS) -c $*.c

all: $(DLL)

$(DLL): $(OBJS)
	gcc --shared $(CFLAGS) -o $(DLL) $(OBJS)
	cp $(DLL) $(BIN)/$(DLL)

clean:
	rm -f $(DLL) $(OBJS)
