srcdir=		..
VPATH=		..
CC=		m68k-palmos-coff-gcc
AR=		m68k-palmos-coff-ar
RANLIB=		m68k-palmos-coff-ranlib
DEFS=		-DHAVE_CONFIG_H
VERSION=	1.5

LIBRARY=	libpython$(VERSION).a
LDLIBRARY=      libpython$(VERSION).a
PALM_CFLAGS=	-fno-builtin -Wall -I.. -I../libc/include -I../../Include

INCLDIR=	../Include
OPT=	-g -O2
CFLAGS=	$(OPT) -I$(INCLDIR) -I. $(PALM_CFLAGS) $(DEFS)

OBJS = palmglue.o

all: $(OBJS) frozen_importer.o python_utils.o
	$(AR) cr $(LIBRARY) palmglue.o
	$(RANLIB) $(LIBRARY)

palmglue.o: palmglue.c

add2lib: $(OBJS)
	$(AR) cr $(LIBRARY) $(OBJS)
	touch add2lib

clean:
	rm -f $(OBJS) add2lib

