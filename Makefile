CC=g++ -std=c++11
RANLIB=ranlib

LIBSRC= Thread.cpp Thread.h UserLevelManager.cpp UserLevelManager.h uthreads.cpp uthreads.h
LIBOBJ= $(LIBSRC:.cpp=.o)

INCS=-I.
CFLAGS = -Wall -g $(INCS)
LOADLIBES = -L./ 

OSMLIB = libuthreads.a
TARGETS = $(OSMLIB)

TAR=tar
TARFLAGS=-cvf
TARNAME=ex2.tar
TARSRCS= Makefile README Thread.cpp Thread.h UserLevelManager.cpp UserLevelManager.h uthreads.cpp 

all: $(TARGETS) 

$(TARGETS): $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) $(TARGETS) $(OSMLIB) $(OBJ) $(LIBOBJ) *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
