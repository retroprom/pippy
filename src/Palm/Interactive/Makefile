OBJS = PythonGUI.o app.o ../PalmGlue/python_utils.o util.o ../PalmGlue/frozen_importer.o  ../pylib-stub-a4.o

CC = m68k-palmos-coff-gcc
AS = m68k-palmos-coff-as -m68000 -mno-68881 -l

#uncomment this if you want to build a gdb debuggable version
#DEFINES = -DDEBUG
PYTHON_ROOT=/home/jcollins/PalmPython/src
INCLUDES=-I$(PYTHON_ROOT)/Include -I$(PYTHON_ROOT)/Palm

PREFS = ../PalmGlue/pref0000.bin
RESOURCES = ../resources/*.bin
LIBC_PATH = ..
LIBC_INCLUDE = $(LIBC_PATH)/libc/include
PALM_CFLAGS= -I$(LIBC_INCLUDE) -I../../Include -I.. -I../PalmGlue -I../../Modules

CFLAGS = -fno-builtin -O0 -g -Wall $(DEFINES) $(PALM_CFLAGS)
PILRC = pilrc
OBJRES = m68k-palmos-coff-obj-res
BUILDPRC = build-prc
CC = m68k-palmos-coff-gcc
AS = m68k-palmos-coff-as -m68000 -mno-68881 -l

PILRC = pilrc
OBJRES = m68k-palmos-coff-obj-res
BUILDPRC = build-prc

ICON_TEXT = "Pippy"
CREATOR = PyAp
PRC = pippy.prc

STAMP=$(shell time_stamp)
DIR=pippy-$(STAMP)

all: $(PRC)

final:
	make clean
	python1.5 -OO ../Resourcer/resourcer -x re -x os -x StringIO -R \
		default_modules.py
	make

snapshot:   # put this in a script
	mkdir /tmp/$(DIR)
	cp $(PRC) /tmp/$(DIR)
	cp ../pylib.prc /tmp/$(DIR)
	(cd /tmp; tar czvf $(DIR).tgz $(DIR); zip $(DIR).zip -r $(DIR))
	mv /tmp/$(DIR).{zip,tgz} .

$(PRC): code.stamp bin.stamp
	cp $(PREFS) .
	cp $(RESOURCES) .
	$(BUILDPRC) $@ $(ICON_TEXT) $(CREATOR) *.grc *.bin

code.stamp: PythonGUI
	$(OBJRES) PythonGUI
	touch code.stamp

bin.stamp: PythonGUI.rcp
	$(PILRC) PythonGUI.rcp
	touch bin.stamp

PythonGUI: $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $(OBJS) -L. -lglibstub

clean:
	rm -f *.o PythonGUI *.stamp *.[pg]rc *.bin

util.o: util.c util.h
PythonGUI.o: PythonGUI.c PythonGUI.h util.h
