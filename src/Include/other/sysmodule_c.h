#ifndef SYSMODULE_C_H
#define SYSMODULE_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from object.c */
/* end forward decls */
PYCFUNCTION_PROTO(sys_exc_info, PyObject) SEG_SYSMODULE_C;
PYCFUNCTION_PROTO(sys_exit, PyObject) SEG_SYSMODULE_C;
PYCFUNCTION_PROTO(sys_settrace, PyObject) SEG_SYSMODULE_C;
PYCFUNCTION_PROTO(sys_setprofile, PyObject) SEG_SYSMODULE_C;
PYCFUNCTION_PROTO(sys_setcheckinterval, PyObject) SEG_SYSMODULE_C;
#ifdef USE_MALLOPT
PYCFUNCTION_PROTO(sys_mdebug, PyObject) SEG_SYSMODULE_C;
#endif
PYCFUNCTION_PROTO(sys_getrefcount, PyObject) SEG_SYSMODULE_C;
#ifdef COUNT_ALLOCS
PYCFUNCTION_PROTO(sys_getcounts, PyObject) SEG_SYSMODULE_C;
#endif
static PyObject *list_builtin_module_names() SEG_SYSMODULE_C;
static PyObject *makepathobject Py_PROTO((char *path,int delim)) SEG_SYSMODULE_C;
static PyObject *makeargvobject Py_PROTO((int argc,char **argv)) SEG_SYSMODULE_C;
static void mywrite Py_PROTO((char *name,FILE *fp,const char *format,va_list va)) SEG_SYSMODULE_C;



#endif /* SYSMODULE_C_H */
