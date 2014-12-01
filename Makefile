#
# Makefile
#
PROG = myBinder

EXE = $(PROG).exe

all: $(PROG)

$(PROG):
	@cd res; make
	@cd lib; make
	@cd main; make

clean:
	@cd res; make clean
	@cd lib; make clean
	@cd main; make clean
	rm -f bin/*
