PYBASE=/cygdrive/c/gnu/Python
PIPPYSRCBASE=../../src
RESOURCERDIR=$(PIPPYSRCBASE)/Palm/Resourcer
PYTHONPATH=.:$(RESOURCERDIR):$(PIPPYSRCBASE)/lib:$(PYBASE)/lib:$(PIPPYSRCBASE)/Palm/Tests:$(PIPPYSRCBASE)/Palm/Interactive

PYTHON=$(PYBASE)/bin/python.exe

all:: app.rcp

$(PIPPYSRCBASE)/Palm/Interactive/default_module.py: 

app.rcp: app.py Makefile $(PIPPYSRCBASE)/Palm/Interactive/default_module.py
	$(PYTHON) -OO $(RESOURCERDIR)/Resourcer.py -x re -x os -x StringIO -R app.py

clean::
	rm *.bin app.c app.rcp
