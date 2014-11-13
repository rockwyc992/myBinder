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

LIBS = -L../bin
LDFLAGS = -l$(PROG) -lresource

OBJS = main.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(BIN)/$(EXE) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -f ../$(EXE) $(OBJS)
