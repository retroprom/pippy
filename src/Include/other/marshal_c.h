#ifndef MARSHAL_C_H
#define MARSHAL_C_H

#include "proto_defs.h"

static void w_more Py_PROTO((char c,WFILE *p)) SEG_MARSHAL_C;
static void w_string Py_PROTO((char *s,int n,WFILE *p)) SEG_MARSHAL_C;
static void w_short Py_PROTO((int x,WFILE *p)) SEG_MARSHAL_C;
static void w_long Py_PROTO((long x,WFILE *p)) SEG_MARSHAL_C;
static int r_string Py_PROTO((char *s,int n,RFILE *p)) SEG_MARSHAL_C;
static void w_object Py_PROTO((PyObject *v,WFILE *p)) SEG_MARSHAL_C;
static int r_short Py_PROTO((RFILE *p)) SEG_MARSHAL_C;
static long r_long Py_PROTO((RFILE *p)) SEG_MARSHAL_C;
static long r_long64 Py_PROTO((RFILE *p)) SEG_MARSHAL_C;
static PyObject *r_object Py_PROTO((RFILE *p)) SEG_MARSHAL_C;
PYCFUNCTION_PROTO(marshal_dump, PyObject) SEG_MARSHAL_C;
PYCFUNCTION_PROTO(marshal_load, PyObject) SEG_MARSHAL_C;
PYCFUNCTION_PROTO(marshal_dumps, PyObject) SEG_MARSHAL_C;
PYCFUNCTION_PROTO(marshal_loads, PyObject) SEG_MARSHAL_C;
void PyMarshal_Init() SEG_MARSHAL_C;



#endif /* MARSHAL_C_H */
