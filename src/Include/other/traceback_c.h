#ifndef TRACEBACK_C_H
#define TRACEBACK_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from object.c */
 /* end forward decls */

DESTRUCTOR_PROTO(tb_dealloc, tracebackobject) SEG_TRACEBACK_C;
GETATTRFUNC_PROTO(tb_getattr, tracebackobject) SEG_TRACEBACK_C;


static tracebackobject *newtracebackobject Py_PROTO((tracebackobject *,PyFrameObject *,int,int)) SEG_TRACEBACK_C;
static int tb_displayline Py_PROTO((PyObject *,char *,int,char *)) SEG_TRACEBACK_C;
static int tb_printinternal Py_PROTO((tracebackobject *, PyObject *, int)) SEG_TRACEBACK_C;

#endif /* TRACEBACK_C_H */
