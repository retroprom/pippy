# a very rudimentary makefile to compile all pilrc resource files
# (.rcp files) in the directory.   Every .rcp file should include
# commonRsc.h to avoid resource ID conflicts.

PILRC= pilrc -I ../../Include

all:
	ls *rcp | xargs --max-args=1 $(PILRC)

clean:
	rm -f *.bin
