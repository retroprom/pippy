#ifndef TUPLEOBJECT_C_H
#define TUPLEOBJECT_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from tupleobject.c */

static PyObject * tupleconcat Py_PROTO((register PyTupleObject *a, register PyObject *bb));
INQUIRY_PROTO(tuplelength, PyTupleObject) SEG_TUPLEOBJECT_C;
INTARGFUNC_PROTO(tuplerepeat, PyTupleObject) SEG_TUPLEOBJECT_C;
INTARGFUNC_PROTO(tupleitem, PyTupleObject) SEG_TUPLEOBJECT_C;
INTINTARGFUNC_PROTO(tupleslice, PyTupleObject) SEG_TUPLEOBJECT_C;
DESTRUCTOR_PROTO(tupledealloc, PyTupleObject) SEG_TUPLEOBJECT_C;
PRINTFUNC_PROTO(tupleprint, PyTupleObject) SEG_TUPLEOBJECT_C;
CMPFUNC_PROTO(tuplecompare, PyTupleObject) SEG_TUPLEOBJECT_C;
REPRFUNC_PROTO(tuplerepr, PyTupleObject) SEG_TUPLEOBJECT_C;
HASHFUNC_PROTO(tuplehash, PyTupleObject) SEG_TUPLEOBJECT_C;


#endif /* TUPLEOBJECT_C_H */
