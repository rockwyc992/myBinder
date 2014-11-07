#main/Makefile
PROG = myBinder

EXE = $(PROG).exe
CC = gcc

CFLAGS = -Os -O2 -Wall \
	 $(SOURCE_FLAGS) -g \
	 -Iinclude
# -D NDEBUG

LIBS = -L../lib
LDFLAGS = -l$(PROG)

OBJS = main.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o ../$(EXE) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -f ../$(EXE) $(OBJS)
