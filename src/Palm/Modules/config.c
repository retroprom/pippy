/* Generated automatically from ../../Modules/config.c.in by makesetup. */
/* -*- C -*- ***********************************************
Copyright 1991-1995 by Stichting Mathematisch Centrum, Amsterdam,
The Netherlands.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Stichting Mathematisch
Centrum or CWI or Corporation for National Research Initiatives or
CNRI not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

While CWI is the initial source for this software, a modified version
is made available by the Corporation for National Research Initiatives
(CNRI) at the Internet address ftp://ftp.python.org.

STICHTING MATHEMATISCH CENTRUM AND CNRI DISCLAIM ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH
CENTRUM OR CNRI BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.

******************************************************************/

/* Module configuration */

/* !!! !!! !!! This file is edited by the makesetup script !!! !!! !!! */

/* This file contains the table of built-in modules.
   See init_builtin() in import.c. */

#include "Python.h"



/* -- ADDMODULE MARKER 1 -- */

extern void PyMarshal_Init();
extern void initimp();
extern void initsocket()  SEG_SOCKETMODULE_C;
extern void initselect()  SEG_SELECTMODULE_C;
extern void initpalmnet() SEG_PALMNETMODULE_C;
extern void initerrno() SEG_ERRNOMODULE_C;
extern void inittime() SEG_TIMEMODULE_C;
extern void initpalmsys() SEG_PALMSYSMODULE_C;
extern void initpalmdm() SEG_DMMODULE_C;
extern void initpalmapps() SEG_APPSMODULE_C;
extern void initinstrument();
extern void initpalmevent();
extern void initpalmform();

struct _inittab _PyImport_Inittab[] = {


/* -- ADDMODULE MARKER 2 -- */

	/* This module "lives in" with marshal.c */
	{"marshal", PyMarshal_Init},

	/* This lives it with import.c */
	{"imp", initimp},

	/* These entries are here for sys.builtin_module_names */
	{"__main__", NULL},
	{"__builtin__", NULL},
	{"sys", NULL},
	{"socket", initsocket},
	{"select", initselect},
	{"palmnet", initpalmnet},
	{"errno", initerrno},
	{"time", inittime},
	{"palmsys", initpalmsys},
	{"palmdm", initpalmdm},
	{"palmapps", initpalmapps},
	{"instrument", initinstrument},
	{"palmevent", initpalmevent},
	{"palmform", initpalmform},

	/* Sentinel */
	{0, 0}
};
