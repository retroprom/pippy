SUBDIRS= the_libc the_python the_glue the_glib

all: $(SUBDIRS)

PIPPY_VERSION= 0.6beta
VERSION=	1.5
LIBRARY= libpython$(VERSION).a


LIBC_SEGMENT_DEFS = DEFS='-DUSE_SEGMENTS -DHAVE_CONFIG -DSEG_NAME=pseglibc -DQ_SEG_NAME=\"pseglibc\"'

all:  the_libc the_python the_glue the_resources the_glib


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
the_resources: the_glue
	(cd resources; make; cd ..)
	touch the_resources

the_glib: the_resources
	make -f Makefile.glib
	touch the_glib

clean_glib:
	make -f Makefile.glib clean

clean_glue: clean_glib
	make -f Makefile.glue clean

clean_python: clean_glue
	make -f Makefile clean
	rm -f the_python

PIPPY_NAME=Pippy-$(PIPPY_VERSION)
PIPPY_DIR=/tmp/$(PIPPY_NAME)
dist: all
	rm -rf $(PIPPY_DIR)
	mkdir $(PIPPY_DIR)
	mkdir $(PIPPY_DIR)/prc
	cp pylib.prc Interactive/python.prc $(PIPPY_DIR)/prc
	cd /tmp
	cvs -d:pserver:jcollins@endeavors.org:/cvsroot co -d $(PIPPY_DIR) PalmPython
	tar czvf $(PIPPY_NAME).tar.gz $(PIPPY_NAME)

clean:
	make -f Makefile.python clean
	make -f Makefile.glib clean
	(cd PalmGlue; make clean)
	(cd libc; make clean)
	(cd resources; make clean)
	rm -f the_*
	rm *.bin

