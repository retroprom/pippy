PYBASE=/cygdrive/c/cygwin/usr/local
PIPPYSRCBASE=../../src
RESOURCERDIR=$(PIPPYSRCBASE)/Palm/Resourcer
PYTHONPATH=.:$(RESOURCERDIR):$(PIPPYSRCBASE)/lib:$(PYBASE)/lib:$(PIPPYSRCBASE)/Palm/Tests:$(PIPPYSRCBASE)/Palm/Interactive:$(PIPPYSRCBASE)/Palm/Importer

PYTHON=$(PYBASE)/bin/python1.5.exe

DEFAULT_MODULES = $(PIPPYSRCBASE)/Palm/Interactive/default_modules.py

all:: app.rcp

$(DEFAULT_MODULES):
	

app.rcp: app.py Makefile $(DEFAULT_MODULES)
	$(PYTHON) -OO $(RESOURCERDIR)/Resourcer.py -x re -x os -x StringIO -R app.py

clean::
	rm *.bin app.c app.rcp
