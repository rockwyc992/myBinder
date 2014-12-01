#
# main/Makefile
#
PROG = myBinder

EXE = $(PROG).exe
BIN = ../bin/
CC = gcc

CFLAGS = -Os -O2 -Wall \
	 $(SOURCE_FLAGS) -g \
	 -I../include \
	 -I../res
# -D NDEBUG

OBJS = main.o \
	../lib/*.o \
	../res/*.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(EXE) $(CFLAGS) $(OBJS)
	cp $(EXE) ../bin/$(EXE)

clean:
	rm -f $(EXE) $(OBJS)
