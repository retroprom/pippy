SUBDIRS= the_libc the_python the_glue the_glib the_server

all: $(SUBDIRS)

PIPPY_VERSION= 0.6beta
PIPPY_WIN_VERSION=0_6beta
VERSION=	1.5
LIBRARY= libpython$(VERSION).a


LIBC_SEGMENT_DEFS = DEFS='-DUSE_SEGMENTS -DHAVE_CONFIG -DSEG_NAME=pseglibc -DQ_SEG_NAME=\"pseglibc\"'


TAGS:
	make -f Makefile.python TAGS

# the following builds python using segments
the_libc:
	(cd libc; make $(LIBC_SEGMENT_DEFS) ; cd ..)
	touch the_libc

the_python: the_libc
	make -f Makefile.python $(LIBC_SEGMENT_DEFS) all
	(cd libc; make $(LIBC_SEGMENT_DEFS) LIBRARY=../$(LIBRARY) add2lib ; cd ..)
	touch the_python

the_glue: the_python
	(cd PalmGlue; make $(LIBC_SEGMENT_DEFS) LIBRARY=../$(LIBRARY) add2lib; cd ..)
	touch the_glue

the_server: the_glue
	(cd Server;  make $(LIBC_SEGMENT_DEFS) LIBRARY=../$(LIBRARY) add2lib; cd ..)
	touch the_server

the_resources: the_glue
	(cd resources; make; cd ..)
	touch the_resources

the_glib: the_resources the_server
	make -f Makefile.glib
	touch the_glib

clean_glib:
	make -f Makefile.glib clean

clean_glue: clean_glib
	make -f Makefile.glue clean

clean_python: clean_glue
	make -f Makefile clean
	rm -f the_python


PIPPY_NAME=pippy-$(PIPPY_VERSION)
PIPPY_SRC=$(PIPPY_NAME)-src
PIPPY_SRC_DIR=/tmp/$(PIPPY_SRC)
PIPPY_BIN=pippy_$(PIPPY_WIN_VERSION)
PIPPY_BIN_DIR=/tmp/$(PIPPY_BIN)

dist: all dist_src dist_bin

dist_bin:
	rm -rf $(PIPPY_BIN_DIR)
	mkdir $(PIPPY_BIN_DIR)
	cp pylib.prc Interactive/pippy.prc $(PIPPY_BIN_DIR)
	cp ../../docs/{README.interactive,LICENSE} $(PIPPY_BIN_DIR)
	(cd /tmp; zip -r $(PIPPY_BIN).zip $(PIPPY_BIN))
	mv /tmp/$(PIPPY_BIN).zip .
	(cd /tmp; tar czvf $(PIPPY_BIN).tar.gz $(PIPPY_BIN))
	mv /tmp/$(PIPPY_BIN).tar.gz .


dist_src:
	rm -rf $(PIPPY_SRC_DIR)
	mkdir $(PIPPY_SRC_DIR)
	mkdir $(PIPPY_SRC_DIR)/prc
	cp pylib.prc Interactive/pippy.prc $(PIPPY_SRC_DIR)/prc
	cp ../../docs/README.interactive $(PIPPY_SRC_DIR)/prc
	(cd /tmp; cvs -q -z3 -d:pserver:anonymous@endeavors.org:/cvsroot export -D tomorrow -d $(PIPPY_SRC) pippy)
	(cd /tmp; tar czf $(PIPPY_SRC).tar.gz $(PIPPY_SRC))
	mv /tmp/$(PIPPY_SRC).tar.gz .

clean:
	make -f Makefile.python clean
	make -f Makefile.glib clean
	(cd PalmGlue; make clean)
	(cd libc; make clean)
	(cd resources; make clean)
	(cd Server; make clean)
	rm -f the_*
	rm -f *.bin
