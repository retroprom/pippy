# Generated automatically from Makefile.in by configure.
# NOTE: Makefile.in is converted into Makefile by the configure script
# in the parent directory.  Once configure has run, you can recreate
# the Makefile by running just config.status.

# === Variables set by config.stat ===

VERSION=	1.5

srcdir=		../../Parser
VPATH=		../../Parser

CC=		m68k-palmos-coff-gcc
RANLIB=		m68k-palmos-coff-ranlib
AR=		m68k-palmos-coff-ar

DEFS=		-DHAVE_CONFIG_H
LIBS=		
PALM_CFLAGS=	-fno-builtin -Wall -I../libc/include -I../../Include 


# === Other things that are customizable but not by configure ===

INCLDIR=	$(srcdir)/../Include
OPT=		-g -O2
CFLAGS=		$(OPT) -I$(INCLDIR) -I.. $(PALM_CFLAGS) $(DEFS)

MKDEP=		mkdep
SHELL=		/bin/sh


# === Fixed definitions ===

POBJS=		acceler.o grammar1.o \
		listnode.o node.o parser.o \
		parsetok.o tokenizer.o bitset.o
#		metagrammar.o

# uncomment the following when WITHOUT_COMPILER == 1
# POBJS=

AROBJS=		$(POBJS) myreadline.o
OBJS=		$(AROBJS) intrcheck.o


PGENMAIN=	pgenmain.o

PGOBJS=		firstsets.o grammar.o pgen.o printgrammar.o

PGENOBJS=	$(PGENMAIN) $(POBJS) $(PGOBJS)

DOBJS=		$(PGENMAIN) $(OBJS) $(PGOBJS)

PGEN=		pgen

LIBRARY=	../libpython$(VERSION).a

# === Rules ===

all:		$(OBJS)

# This target is used by the master Makefile to add the objects to the library
add2lib:	$(OBJS)
		$(AR) cr $(LIBRARY) $(AROBJS)
		if test ! -f ../Modules/hassignal; \
		then echo adding intrcheck.o; $(AR) r $(LIBRARY) intrcheck.o; \
		else echo leaving intrcheck.o out; fi
		touch add2lib

$(PGEN):	$(PGENOBJS)
		$(CC) $(OPT) $(PGENOBJS) $(LIBS) -o $(PGEN)

clean:
		-rm -f *.o core *~ [@,#]* *.old *.orig *.rej add2lib

clobber:	clean
		-rm -f $(PGEN) *.a tags TAGS

depend:
		$(MKDEP) $(CFLAGS) `echo $(DOBJS) | tr ' ' '\012' | \
					sed 's|\(.*\)\.o|$(srcdir)/\1.c|'`

.PRECIOUS:	Makefile

acceler.o: acceler.c
grammar1.o: grammar1.c
intrcheck.o: intrcheck.c
listnode.o: listnode.c
myreadline.o: myreadline.c
node.o: node.c
parser.o: parser.c
parsetok.o: parsetok.c
tokenizer.o: tokenizer.c
bitset.o: bitset.c
firstsets.o: firstsets.c
grammar.o: grammar.c
metagrammar.o: metagrammar.c
pgen.o: pgen.c
printgrammar.o: printgrammar.c
pgenmain.o: pgenmain.c

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
