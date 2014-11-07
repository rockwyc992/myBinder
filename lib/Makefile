#lib/Makefile
PROG = myBinder

DLL = lib$(PROG).dll
CC = gcc

CFLAGS = -Os -O2 -Wall \
	$(SOURCE_FLAGS) -g \
	-I../include
# -D NDEBUG

OBJS = \
	dir.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c

all: $(DLL)

$(DLL): $(OBJS)
	gcc --shared $(CFLAGS) -o $(DLL) $(OBJS)

clean:
	rm -f $(DLL) $(OBJS)
