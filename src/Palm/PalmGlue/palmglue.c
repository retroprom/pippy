/* 

   Contained are routines necessary for gluing a main palm
   application to the python (.prc) GLib shared library.

*/



#include <Python.h>
#include <PalmTypes.h>

#ifdef PALMOS
#include <node.h>
#include <errcode.h>
#include <grammar.h>
#include <parsetok.h>
#include <compile.h>
#include "palmglue.h"

extern grammar _PyParser_Grammar; /* From graminit.c */
struct _frozen *PyImport_FrozenModules;

Err errno = 0;

void set_flags(f_dbg, f_verbose, f_interact, f_nosite, f_except, 
	       f_frozen, f_optimize)
	int f_dbg, f_verbose, f_interact, f_nosite;
	int f_except, f_frozen, f_optimize;
{
	Py_DebugFlag                 = f_dbg;
	Py_VerboseFlag               = f_verbose;
	Py_InteractiveFlag           = f_interact;
	Py_NoSiteFlag                = f_nosite;
	Py_UseClassExceptionsFlag    = f_except;
	Py_FrozenFlag                = f_frozen;
	Py_OptimizeFlag              = f_optimize;
}

void set_frozen_tab( struct _frozen *f)
{
	PyImport_FrozenModules = f;
}
#ifndef WITHOUT_COMPILER

int
run_SimpleString(command)
	char *command;
{
	PyObject *m, *d, *v;
	m = PyImport_AddModule("__main__");
	if (m == NULL)
		return -1;
	d = PyModule_GetDict(m);
	v = PyRun_String(command, Py_single_input, d, d);
	if (v == NULL) {
		PyErr_Print();
		return -1;
	}
	Py_DECREF(v);
	if (Py_FlushLine())
		PyErr_Clear();
	return 0;
}
#endif /* WITHOUT_COMPILER */

#endif
 
