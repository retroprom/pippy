PYBASE=/cygdrive/c/gnu/Python
YOSEMITEBASE=../../../yosemite
PIPPYSRCBASE=../../src
RESOURCERDIR=$(PIPPYSRCBASE)/Palm/Resourcer
PYTHONPATH=.:$(YOSEMITEBASE)/scales:$(YOSEMITEBASE)/utility:$(YOSEMITEBASE)/platform:$(RESOURCERDIR):$(PIPPYSRCBASE)/lib:$(PYBASE)/lib
PYTHON=$(PYBASE)/bin/python.exe

all:: clean app.rcp

app.rcp: app.py Makefile
	$(PYTHON) $(RESOURCERDIR)/Resourcer.py app.py

clean::
	rm *.bin app.c
