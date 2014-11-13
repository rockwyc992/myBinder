#
# res/Makefile
#
PROG = resource

DLL = lib$(PROG).dll
BIN = ../bin
CC = gcc
WINDRES = windres

CFLAGS = -Os -O2 -Wall \
	$(SOURCE_FLAGS) -g \
	-I.
# -D NDEBUG

OBJS = \
	resource.o

RESOURCE = \
	$(PROG).h \
	$(PROG).rc \
	$(PROG).txt

CREAT = creat_$(PROG)

all: $(DLL)

$(CREAT).exe: $(CREAT).c
	$(CC) $(CFLAGS) $(CREAT).c -o $(CREAT).exe

$(RESOURCE): $(CREAT).exe
	$(CREAT)

$(OBJS): $(RESOURCE)
	$(WINDRES) $(PROG).rc $(PROG).o


$(DLL): $(OBJS)
	gcc --shared $(CFLAGS) -o $(DLL) $(OBJS)
	cp $(DLL) $(BIN)/$(DLL)

clean:
	rm -f $(DLL) $(OBJS) $(RESOURCE) $(CREAT).exe
