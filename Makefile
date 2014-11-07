#
# Makefile
#
PROG = myBinder

EXE = $(PROG).exe
CC = gcc

CFLAGS = -Os -O2 -Wall \
	 $(SOURCE_FLAGS) -g \
	 -Iinclude
# -D NDEBUG

all: $(PROG)

$(PROG):
	@cd lib; make
	@cd main; make

clean:
	@cd lib; make clean
	@cd main; make clean
	rm -f bin/*
