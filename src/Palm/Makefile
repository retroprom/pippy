# Generated automatically from Makefile.in by configure.
########################################################################
# Copyright 1991-1995 by Stichting Mathematisch Centrum, Amsterdam,
# The Netherlands.
#
#                         All Rights Reserved
#
# Permission to use, copy, modify, and distribute this software and its
# documentation for any purpose and without fee is hereby granted,
# provided that the above copyright notice appear in all copies and that
# both that copyright notice and this permission notice appear in
# supporting documentation, and that the names of Stichting Mathematisch
# Centrum or CWI or Corporation for National Research Initiatives or
# CNRI not be used in advertising or publicity pertaining to
# distribution of the software without specific, written prior
# permission.
# 
# While CWI is the initial source for this software, a modified version
# is made available by the Corporation for National Research Initiatives
# (CNRI) at the Internet address ftp://ftp.python.org.
# 
# STICHTING MATHEMATISCH CENTRUM AND CNRI DISCLAIM ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH
# CENTRUM OR CNRI BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
# DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
# PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
# TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
########################################################################

# Substitutions by configure
VERSION=	1.5
srcdir=		..
VPATH=		..
CC=		m68k-palmos-coff-gcc
AR=		m68k-palmos-coff-ar
RANLIB=		m68k-palmos-coff-ranlib
DEFS=		-DHAVE_CONFIG_H

# Machine-dependent subdirectories
MACHDEP=	m68k-palmos-coff

# Install prefix for architecture-independent files
prefix=		../pypalm

# Install prefix for architecture-dependent files
exec_prefix=	${prefix}

# Expanded directories
BINDIR=		$(exec_prefix)/bin
LIBDIR=		$(exec_prefix)/lib
MANDIR=		$(prefix)/man
INCLUDEDIR=	$(prefix)/include
CONFINCLUDEDIR=	$(exec_prefix)/include
SCRIPTDIR=	$(prefix)/lib

# Detailed destination directories
BINLIBDEST=	$(LIBDIR)/python$(VERSION)
LIBDEST=	$(SCRIPTDIR)/python$(VERSION)
INCLUDEPY=	$(INCLUDEDIR)/python$(VERSION)
CONFINCLUDEPY=	$(CONFINCLUDEDIR)/python$(VERSION)
LIBP=		$(LIBDIR)/python$(VERSION)

# Symbols used for using shared libraries
SO=		.so
LDSHARED=	ld
CCSHARED=	
LINKFORSHARED=	
DESTSHARED=	$(BINLIBDEST)/lib-dynload

# Shell used by make (some versions default to the login shell, which is bad)
SHELL=		/bin/sh

# Use ``EXE=.exe'' for Unix emulations on DOS/Windows (e.g. GNUWIN32)
EXE=

# Modes for directories, executables and data files created by the
# install process.  Default to group-writable directories but
# user-only-writable for executables and data files.
DIRMODE=	775
EXEMODE=	755
FILEMODE=	644

# Portable install script (configure doesn't always guess right)
INSTALL=	../install-sh -c
INSTALL_PROGRAM=${INSTALL} -m $(EXEMODE)
INSTALL_DATA=	${INSTALL} -m $(FILEMODE)

# Use this to make a link between python$(VERSION) and python in $(BINDIR)
LN=@LN@

# --with-PACKAGE options for configure script
# e.g. --with-readline --with-svr5 --with-solaris --with-thread
# (see README for an explanation)
WITH=		

# Compiler options passed to subordinate makes
OPT=		-g -O2

# Subdirectories where to run make recursively
SUBDIRS_SRC=	../Parser ../Objects ../Python ../Modules
SUBDIRS=	Parser Objects Python Modules

# Other subdirectories
SUBDIRSTOO=	Include Lib Misc Demo Grammar

# Files and directories to be distributed
DISTFILES=	README ChangeLog
DISTDIRS=	$(SUBDIRS) $(SUBDIRSTOO) Ext-dummy
DIST=		$(DISTFILES) $(DISTDIRS)

# Compilation flags for getbuildinfo.c only
CFLAGS=		$(OPT) -I. $(DEFS)

LIBRARY=	libpython$(VERSION).a
LDLIBRARY=      libpython$(VERSION).a

# Default target
all:		$(LIBRARY) python$(EXE)

# Build the interpreter
python$(EXE):	$(LIBRARY) buildno Modules/python.o
		expr `cat buildno` + 1 >buildno1
		mv -f buildno1 buildno
		$(CC) -c $(CFLAGS) -DBUILD=`cat buildno` \
		      $(srcdir)/Modules/getbuildinfo.c
		$(AR) cr $(LIBRARY) getbuildinfo.o
		$(RANLIB) $(LIBRARY)
		touch python$(EXE)
		true
#		cd Modules;  $(MAKE) OPT="$(OPT)" VERSION="$(VERSION)" \
#			prefix="$(prefix)" exec_prefix="$(exec_prefix)" \
#			LIBRARY=../$(LDLIBRARY) link

Modules/python.o: $(srcdir)/Modules/python.c
		cd Modules; $(MAKE) OPT="$(OPT)" python.o

buildno:
		echo 0 >buildno

# Build the library
$(LIBRARY):	$(SUBDIRS) $(SUBDIRS_SRC)
		if test ! -f $(LIBRARY); \
		then for i in $(SUBDIRS); do rm -f $$i/add2lib; done; true; \
		else true; fi
		for i in $(SUBDIRS); do \
			(cd $$i; $(MAKE) VERSION="$(VERSION)" add2lib); done

# This rule is only here for DG/UX!!!
libpython$(VERSION).so:	$(LIBRARY)
		case `uname -s | tr -d '/ ' | tr '[A-Z]' '[a-z]'` in \
		*dgux*) \
		    test -d dgux || mkdir dgux; \
		    (cd dgux;ar x ../$^;ld -G -o ../$@ * ); \
		    /bin/rm -rf ./dgux \
		    ;; \
		esac

# This rule is here for OPENSTEP/Rhapsody/MacOSX
libpython$(VERSION).dylib: $(LIBRARY)
		libtool -o $(LDLIBRARY) -dynamic $(OTHER_LIBTOOL_OPT) $(LIBRARY) -framework System @LIBTOOL_CRUFT@ 

Parser:
		cd Parser ; $(MAKE) OPT="$(OPT)" VERSION="$(VERSION)" \
			prefix="$(prefix)" exec_prefix="$(exec_prefix)" all

Python:
		cd Python ; $(MAKE) OPT="$(OPT)" VERSION="$(VERSION)" \
			prefix="$(prefix)" exec_prefix="$(exec_prefix)" all

Objects:
		cd Objects ; $(MAKE) OPT="$(OPT)" VERSION="$(VERSION)" \
			prefix="$(prefix)" exec_prefix="$(exec_prefix)" all

Modules:	Parser Python Objects
		cd Modules ; $(MAKE) OPT="$(OPT)" VERSION="$(VERSION)" \
			prefix="$(prefix)" exec_prefix="$(exec_prefix)" all

# Test the interpreter (twice, once without .pyc files, once with)
TESTOPTS=	
TESTPROG=	$(srcdir)/Lib/test/regrtest.py
TESTPYTHON=	./python$(EXE) -tt
test:		all
		-rm -f $(srcdir)/Lib/test/*.py[co]
		-PYTHONPATH= $(TESTPYTHON) $(TESTPROG) $(TESTOPTS)
		PYTHONPATH= $(TESTPYTHON) $(TESTPROG) $(TESTOPTS)

# Install everything
##install:	altinstall bininstall maninstall

# Install almost everything without disturbing previous versions
altinstall:	altbininstall libinstall inclinstall libainstall sharedinstall

# Install the interpreter (by creating a hard link to python$(VERSION))
bininstall:	altbininstall
		-if test -f $(BINDIR)/python$(EXE); \
		then rm -f $(BINDIR)/python$(EXE); \
		else true; \
		fi
		(cd $(BINDIR); $(LN) python$(VERSION)$(EXE) python$(EXE))

# Install the interpreter with $(VERSION) affixed
# This goes into $(exec_prefix)
altbininstall:	python$(EXE)
		@for i in $(BINDIR); \
		do \
			if test ! -d $$i; then \
				echo "Creating directory $$i"; \
				mkdir $$i; \
				chmod $(DIRMODE) $$i; \
			else	true; \
			fi; \
		done
		$(INSTALL_PROGRAM) python$(EXE) $(BINDIR)/python$(VERSION)$(EXE)
		if test -f libpython$(VERSION).so; then \
			$(INSTALL_DATA) libpython$(VERSION).so $(LIBDIR); \
		else	true; \
		fi

# Install the manual page
maninstall:
		@for i in $(MANDIR) $(MANDIR)/man1; \
		do \
			if test ! -d $$i; then \
				echo "Creating directory $$i"; \
				mkdir $$i; \
				chmod $(DIRMODE) $$i; \
			else	true; \
			fi; \
		done
		$(INSTALL_DATA) $(srcdir)/Misc/python.man \
			$(MANDIR)/man1/python.1

# Install the library
PLATDIR=	plat-$(MACHDEP)
MACHDEPS=	$(PLATDIR)
LIBSUBDIRS=	lib-old lib-tk test test/output $(MACHDEPS)
libinstall:	python $(srcdir)/Lib/$(PLATDIR)
		@for i in $(SCRIPTDIR) $(LIBDEST); \
		do \
			if test ! -d $$i; then \
				echo "Creating directory $$i"; \
				mkdir $$i; \
				chmod $(DIRMODE) $$i; \
			else	true; \
			fi; \
		done
		@for d in $(LIBSUBDIRS); \
		do \
			a=$(srcdir)/Lib/$$d; \
			if test ! -d $$a; then continue; else true; fi; \
			b=$(LIBDEST)/$$d; \
			if test ! -d $$b; then \
				echo "Creating directory $$b"; \
				mkdir $$b; \
				chmod $(DIRMODE) $$b; \
			else	true; \
			fi; \
		done
		@for i in $(srcdir)/Lib/*.py $(srcdir)/Lib/*.doc; \
		do \
			if test -x $$i; then \
				$(INSTALL_PROGRAM) $$i $(LIBDEST); \
				echo $(INSTALL_PROGRAM) $$i $(LIBDEST); \
			else \
				$(INSTALL_DATA) $$i $(LIBDEST); \
				echo $(INSTALL_DATA) $$i $(LIBDEST); \
			fi; \
		done
		@for d in $(LIBSUBDIRS); \
		do \
			a=$(srcdir)/Lib/$$d; \
			if test ! -d $$a; then continue; else true; fi; \
			b=$(LIBDEST)/$$d; \
			for i in $$a/*; \
			do \
				case $$i in \
				*CVS) ;; \
				*.py[co]) ;; \
				*~) ;; \
				*) \
					if test -d $$i; then continue; fi; \
					if test -x $$i; then \
					    echo $(INSTALL_PROGRAM) $$i $$b; \
					    $(INSTALL_PROGRAM) $$i $$b; \
					else \
					    echo $(INSTALL_DATA) $$i $$b; \
					    $(INSTALL_DATA) $$i $$b; \
					fi;; \
				esac; \
			done; \
		done
		PYTHONPATH=$(LIBDEST) \
			./python$(EXE) -t $(LIBDEST)/compileall.py $(LIBDEST)
		PYTHONPATH=$(LIBDEST) \
			./python$(EXE) -O $(LIBDEST)/compileall.py $(LIBDEST)

# Create the PLATDIR source directory, if one wasn't distributed..
$(srcdir)/Lib/$(PLATDIR):
		mkdir $(srcdir)/Lib/$(PLATDIR)
		cp $(srcdir)/Lib/plat-generic/regen $(srcdir)/Lib/$(PLATDIR)/regen
		export PATH; PATH="`pwd`:$$PATH"; \
		export PYTHONPATH; PYTHONPATH="`pwd`/Lib"; \
		cd $(srcdir)/Lib/$(PLATDIR); ./regen

# Install the include files
INCLDIRSTOMAKE=$(INCLUDEDIR) $(CONFINCLUDEDIR) $(INCLUDEPY) $(CONFINCLUDEPY)
inclinstall:
		@for i in $(INCLDIRSTOMAKE); \
		do \
			if test ! -d $$i; then \
				echo "Creating directory $$i"; \
				mkdir $$i; \
				chmod $(DIRMODE) $$i; \
			else	true; \
			fi; \
		done
		@for i in $(srcdir)/Include/*.h; \
		do \
			echo $(INSTALL_DATA) $$i $(INCLUDEPY); \
			$(INSTALL_DATA) $$i $(INCLUDEPY); \
		done
		$(INSTALL_DATA) config.h $(CONFINCLUDEPY)/config.h

# Install the library and miscellaneous stuff needed for extending/embedding
# This goes into $(exec_prefix)
LIBPL=		$(LIBP)/config
libainstall:	all
		@for i in $(LIBDIR) $(LIBP) $(LIBPL); \
		do \
			if test ! -d $$i; then \
				echo "Creating directory $$i"; \
				mkdir $$i; \
				chmod $(DIRMODE) $$i; \
			else	true; \
			fi; \
		done
		@if [ "$(MACHDEP)" != "beos" ] ; then \
			$(INSTALL_DATA) $(LIBRARY) $(LIBPL)/$(LIBRARY) ; \
			$(RANLIB) $(LIBPL)/$(LIBRARY) ; \
		fi
		$(INSTALL_DATA) Modules/config.c $(LIBPL)/config.c
		$(INSTALL_DATA) Modules/python.o $(LIBPL)/python.o
		$(INSTALL_DATA) $(srcdir)/Modules/config.c.in $(LIBPL)/config.c.in
		$(INSTALL_DATA) Modules/Makefile $(LIBPL)/Makefile
		$(INSTALL_DATA) Modules/Setup $(LIBPL)/Setup
		$(INSTALL_DATA) Modules/Setup.local $(LIBPL)/Setup.local
		$(INSTALL_DATA) Modules/Setup.thread $(LIBPL)/Setup.thread
		$(INSTALL_PROGRAM) $(srcdir)/Modules/makesetup $(LIBPL)/makesetup
		$(INSTALL_PROGRAM) $(srcdir)/install-sh $(LIBPL)/install-sh
		$(INSTALL_DATA) $(srcdir)/Misc/Makefile.pre.in $(LIBPL)/Makefile.pre.in
		@if [ -s Modules/python.exp -a \
			"`echo $(MACHDEP) | sed 's/^\(...\).*/\1/'`" = "aix" ]; then \
			echo; echo "Installing support files for building shared extension modules on AIX:"; \
			$(INSTALL_DATA) Modules/python.exp		\
					$(LIBPL)/python.exp;		\
			echo; echo "$(LIBPL)/python.exp";		\
			$(INSTALL_PROGRAM) $(srcdir)/Modules/makexp_aix	\
					$(LIBPL)/makexp_aix;		\
			echo "$(LIBPL)/makexp_aix";			\
			$(INSTALL_PROGRAM) $(srcdir)/Modules/ld_so_aix	\
					$(LIBPL)/ld_so_aix;		\
			echo "$(LIBPL)/ld_so_aix";			\
			echo; echo "See Misc/AIX-NOTES for details.";	\
		else true; \
		fi

# Install the dynamically loadable modules
# This goes into $(exec_prefix)
sharedinstall:
		cd Modules; $(MAKE) \
			OPT="$(OPT)" \
			VERSION="$(VERSION)" \
			SO="$(SO)" \
			LDSHARED="$(LDSHARED)" \
			CCSHARED="$(CCSHARED)" \
			LINKFORSHARED="$(LINKFORSHARED)" \
			DESTSHARED="$(DESTSHARED)" \
			prefix="$(prefix)" \
			exec_prefix="$(exec_prefix)" \
			sharedinstall

.PRECIOUS:	python$(EXE)

# Rerun configure with the same options as it was run last time,
# provided the config.status script exists
recheck:
		$(SHELL) config.status --recheck
		$(SHELL) config.status

# Create a tags file for vi
tags::
		ctags -w -t Include/*.h
		for i in $(SUBDIRS); do ctags -w -t -a $$i/*.[ch]; done
		sort tags -o tags

# Create a tags file for GNU Emacs
TAGS::
		etags ../Include/*.h ../Include/other/*.h
		for i in $(SUBDIRS_SRC); do etags -a $$i/*.[ch]; done

# Add dependencies to sub-Makefiles
depend:
		@for i in $(SUBDIRS); do \
			(echo making depend in subdirectory $$i; cd $$i; \
			 $(MAKE) depend); \
		done

# Sanitation targets -- clean leaves libraries, executables and tags
# files, which clobber removes those as well

localclean:
		-rm -f core *~ [@,#]* *.old *.orig *.rej

clean:		localclean
		-for i in $(SUBDIRS); do \
		    if test -d $$i; then \
			(echo making clean in subdirectory $$i; cd $$i; \
			 if test -f Makefile; \
			 then $(MAKE) clean; \
			 else $(MAKE) -f Makefile.*in clean; \
			 fi); \
		    else true; fi; \
		done

localclobber:	localclean
		-rm -f tags TAGS python$(EXE) $(LIBRARY) $(LDLIBRARY) *.o
		-rm -f config.log config.cache config.h

clobber:	localclobber
		-for i in $(SUBDIRS); do \
		    if test -d $$i; then \
			(echo clobbering subdirectory $$i; cd $$i; \
			 if test -f Makefile; \
			 then $(MAKE) clobber; \
			 else $(MAKE) -f $(srcdir)/Makefile*.in clobber; \
			 fi); \
		    else true; fi; \
		done

# Make things extra clean, before making a distribution:
# remove all generated files, even Makefile[.pre]
distclean:	clobber
		-$(MAKE) -f $(srcdir)/Makefile.in \
			SUBDIRS="$(SUBDIRSTOO)" clobber
		-rm -f config.status config.log config.cache config.h Makefile
		-rm -f buildno
		-rm -f Modules/Makefile
		-for i in $(SUBDIRS) $(SUBDIRSTOO); do \
			 for f in $$i/*.in; do \
				f=`basename "$$f" .in`; \
			 	if test "$$f" != "*"; then \
					echo rm -f "$$i/$$f"; \
				 	rm -f "$$i/$$f"; \
				fi; \
			 done; \
		done

# Check for smelly exported symbols (not starting with Py/_Py)
smelly: all
	for i in $(SUBDIRS); do \
		echo --- $$i ---; \
		nm -p $$i/lib$$i.a | \
		sed -n "/ [TDB] /s/.* //p" | grep -v "^_*Py" | sort -u; \
	done

# Find files with funny names
funny:
		find $(DISTDIRS) -type d \
			-o -name '*.[chs]' \
			-o -name '*.py' \
			-o -name '*.doc' \
			-o -name '*.sty' \
			-o -name '*.bib' \
			-o -name '*.dat' \
			-o -name '*.el' \
			-o -name '*.fd' \
			-o -name '*.in' \
			-o -name '*.tex' \
			-o -name '*,[vpt]' \
			-o -name 'Setup' \
			-o -name 'Setup.*' \
			-o -name README \
			-o -name Makefile \
			-o -name ChangeLog \
			-o -name Repository \
			-o -name Root \
			-o -name Entries \
			-o -name Tag \
			-o -name tags \
			-o -name TAGS \
			-o -name .cvsignore \
			-o -name MANIFEST \
			-o -print
