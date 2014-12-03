#
# res/Makefile
#
PROG = resource

BIN = ../bin
CC = gcc
WINDRES = windres

CFLAGS = -Os -O2 -Wall -g \
	$(SOURCE_FLAGS) \
	-I.
# -D NDEBUG

OBJS = \
	resource.o

RESOURCE = \
	$(PROG).h \
	$(PROG).rc \
	$(PROG).txt

CREATE = create_$(PROG)

all: $(OBJS)

$(CREATE).exe: $(CREATE).c
	$(CC) $(CFLAGS) $(CREATE).c -o $(CREATE).exe

$(RESOURCE): $(CREATE).exe
	$(CREATE)

$(OBJS): $(RESOURCE)
	$(WINDRES) $(PROG).rc $(PROG).o

clean:
	rm -f $(DLL) $(OBJS) $(RESOURCE) $(CREATE).exe
