PYBASE=/cygdrive/c/gnu/Python
PIPPYSRCBASE=../../src
RESOURCERDIR=$(PIPPYSRCBASE)/Palm/Resourcer
PYTHONPATH=.:$(RESOURCERDIR):$(PIPPYSRCBASE)/lib:$(PYBASE)/lib:$(PIPPYSRCBASE)/Palm/Tests:$(PIPPYSRCBASE)/Palm/Interactive

PYTHON=$(PYBASE)/bin/python.exe

all:: app.rcp

app.rcp: app.py Makefile
	$(PYTHON) -OO $(RESOURCERDIR)/Resourcer.py -x re -x os -x StringIO -R app.py

clean::
	rm *.bin app.c app.rcp
