OBJS = \
     strrchr.o vsprintf.o ctype.o setjmp.o \
     bcopy.o memmove.o strchr.o strncat.o strspn.o \
     memscan.o strcmp.o strncmp.o strstr.o \
     memset.o strcpy.o strncpy.o strtok.o \
     memcmp.o strnlen.o strtol.o \
     memcpy.o strcat.o strlen.o strpbrk.o strtoul.o sscanf.o fopen.o\
     malloc.o calloc.o realloc.o free.o fclose.o fwrite.o io.o mystdio.o

CC = m68k-palmos-coff-gcc
AS = m68k-palmos-coff-as -mno-68881 -m68000 -l

#uncomment this if you want to build a gdb debuggable version
#DEFINES = -DDEBUG

# To use segments, on the command line type:
# make clean; make USE_SEGMENTS=-DUSE_SEGMENTS
# The name of the segment is specified using SEG_NAME
# as shown below - careful with the quotes!
SEG_NAME = pseglibc
Q_SEG_NAME = \"$(SEG_NAME)\"
SEGMENT_LIST = pseglibc

USE_SEGMENTS=-DUSE_SEGMENTS
INCLUDES = -I./include 
CFLAGS = -O2 -Wall -g -fno-builtin $(DEFINES) $(INCLUDES) $(USE_SEGMENTS) -DQ_SEG_NAME=$(Q_SEG_NAME) -DSEG_NAME=$(SEG_NAME)
ASFLAGS = -Wall -g -fno-builtin $(DEFINES) $(INCLUDES) $(USE_SEGMENTS) -DQ_SEG_NAME=$(Q_SEG_NAME) -DSEG_NAME=$(SEG_NAME)


OBJRES = m68k-palmos-coff-obj-res
AR = m68k-palmos-coff-ar
RANLIB = m68k-palmos-coff-ranlib

BUILDPRC = build-prc

OURLIB = GLibc
FULLNAME = "Python C Library"
CREATOR = PyLc

all: $(OBJS)

libc.a: $(OBJS)
	rm -f libc.a
	$(AR) -r libc.a $(OBJS)
	$(RANLIB) libc.a

$(OURLIB).exp: libc.a
	m68k-palmos-coff-exportlist libc.a > $(OURLIB).exp

$(OURLIB)-hdr.s: $(OURLIB).exp
	m68k-palmos-coff-glibhdrgen < $(OURLIB).exp > $(OURLIB)-hdr.s

$(OURLIB)-stub-a4.s: $(OURLIB).exp
	m68k-palmos-coff-glibstubgen $(CREATOR) a4data < $(OURLIB).exp > $(OURLIB)-stub-a4.s


$(OURLIB): $(OBJS) multiseg.ld $(OURLIB)-hdr.o multisegstub.o
	$(CC) -shared $(CFLAGS) -T multiseg.ld -o $(OURLIB) $(OURLIB)-hdr.o \
	      $(OBJS) multisegstub.o -lNetSocket -lglibstub

multiseg.ld multisegstub.s:
	m68k-palmos-coff-multisegstubgen -l $(SEGMENT_LIST)

the_grc: $(OURLIB)
	$(OBJRES) -l $(OURLIB) $(SEGMENT_LIST)
	touch the_grc

$(OURLIB).prc: $(OURLIB) $(OBJS) $(OURLIB)-hdr.o the_grc
	$(BUILDPRC) -l $(OURLIB).prc $(FULLNAME) $(CREATOR) *.$(OURLIB).grc

add2lib: $(OBJS)
	$(AR) cr $(LIBRARY) $(OBJS)
	touch add2lib

clean:
	rm -f *.o $(OURLIB)-hdr* $(OURLIB) *.[pg]rc
	rm -f $(OURLIB)-stub* $(OURLIB)-mwstub* errs *.ld

