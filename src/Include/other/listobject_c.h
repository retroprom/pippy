#ifndef LISTOBJECT_C_H
#define LISTOBJECT_C_H

#include "proto_defs.h"
static int roundupsize Py_PROTO((int n)) SEG_LISTOBJECT_C;
static int ins1 Py_PROTO((PyListObject *self, int where, PyObject *v)) SEG_LISTOBJECT_C;
static PyObject *ins Py_PROTO((PyListObject *self, int where, PyObject *v)) SEG_LISTOBJECT_C;
BINARYFUNC1_PROTO(listinsert, PyListObject) SEG_LISTOBJECT_C;
BINARYFUNC1_PROTO(listappend, PyListObject) SEG_LISTOBJECT_C;
BINARYFUNC1_PROTO(listextend, PyListObject) SEG_LISTOBJECT_C;
BINARYFUNC1_PROTO(listpop, PyListObject) SEG_LISTOBJECT_C;
static int docompare Py_PROTO((PyObject *x, PyObject *y, PyObject *compare)) SEG_LISTOBJECT_C;
static int binarysort Py_PROTO((PyObject **lo, PyObject **hi,PyObject **start,PyObject *compare)) SEG_LISTOBJECT_C;
static int samplesortslice Py_PROTO((PyObject **lo,PyObject **hi,PyObject *compare)) SEG_LISTOBJECT_C;
BINARYFUNC1_PROTO(listsort, PyListObject) SEG_LISTOBJECT_C;
BINARYFUNC1_PROTO(listreverse, PyListObject) SEG_LISTOBJECT_C;
BINARYFUNC1_PROTO(listindex, PyListObject) SEG_LISTOBJECT_C;
BINARYFUNC1_PROTO(listcount, PyListObject) SEG_LISTOBJECT_C;
BINARYFUNC1_PROTO(listremove, PyListObject) SEG_LISTOBJECT_C;
static PyObject *immutable_list_getattr Py_PROTO((PyListObject *f, char *name)) SEG_LISTOBJECT_C;
static int immutable_list_ass () SEG_LISTOBJECT_C;

BINARYFUNC1_PROTO(list_concat, PyListObject) SEG_LISTOBJECT_C;
INQUIRY_PROTO(list_length, PyListObject) SEG_LISTOBJECT_C;
INTARGFUNC_PROTO(list_repeat, PyListObject) SEG_LISTOBJECT_C;
INTARGFUNC_PROTO(list_item, PyListObject) SEG_LISTOBJECT_C;
INTINTARGFUNC_PROTO(list_slice, PyListObject) SEG_LISTOBJECT_C;
INTOBJARGPROC_PROTO(list_ass_item, PyListObject) SEG_LISTOBJECT_C;
INTINTOBJARGPROC_PROTO(list_ass_slice, PyListObject) SEG_LISTOBJECT_C;
DESTRUCTOR_PROTO(list_dealloc, PyListObject) SEG_LISTOBJECT_C;
PRINTFUNC_PROTO(list_print, PyListObject) SEG_LISTOBJECT_C;
GETATTRFUNC_PROTO(list_getattr, PyListObject) SEG_LISTOBJECT_C;
CMPFUNC_PROTO(list_compare, PyListObject) SEG_LISTOBJECT_C;
REPRFUNC_PROTO(list_repr, PyListObject) SEG_LISTOBJECT_C;



#endif /* LISTOBJECT_C_H */
