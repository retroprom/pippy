#ifndef DICTOBJECT_C_H
#define DICTOBJECT_C_H

#include "proto_defs.h"

static dictentry *lookdict Py_PROTO((dictobject *, PyObject *, long)) SEG_DICTOBJECT_C;
static void insertdict Py_PROTO((dictobject *, PyObject *, long, PyObject *)) SEG_DICTOBJECT_C;
static int dictresize Py_PROTO((dictobject *, int)) SEG_DICTOBJECT_C;
static PyObject *characterize Py_PROTO((dictobject *, dictobject *, PyObject **)) SEG_DICTOBJECT_C;

INQUIRY_PROTO(dict_length, dictobject) SEG_DICTOBJECT_C;
OBJOBJARGPROC_PROTO(dict_ass_sub, dictobject) SEG_DICTOBJECT_C;
DESTRUCTOR_PROTO(dict_dealloc, dictobject) SEG_DICTOBJECT_C;
PRINTFUNC_PROTO(dict_print, dictobject) SEG_DICTOBJECT_C;
GETATTRFUNC_PROTO(dict_getattr, dictobject) SEG_DICTOBJECT_C;
CMPFUNC_PROTO(dict_compare, dictobject) SEG_DICTOBJECT_C;
REPRFUNC_PROTO(dict_repr, dictobject) SEG_DICTOBJECT_C;
BINARYFUNC1_PROTO(dict_has_key, register dictobject) SEG_DICTOBJECT_C;
BINARYFUNC1_PROTO(dict_keys, register dictobject) SEG_DICTOBJECT_C;
BINARYFUNC1_PROTO(dict_items, register dictobject) SEG_DICTOBJECT_C;
BINARYFUNC1_PROTO(dict_values, register dictobject) SEG_DICTOBJECT_C;
BINARYFUNC1_PROTO(dict_update, register dictobject) SEG_DICTOBJECT_C;
BINARYFUNC1_PROTO(dict_clear, register dictobject) SEG_DICTOBJECT_C;
BINARYFUNC1_PROTO(dict_copy, register dictobject) SEG_DICTOBJECT_C;
BINARYFUNC1_PROTO(dict_get, register dictobject) SEG_DICTOBJECT_C;

static PyObject *dict_subscript Py_PROTO((dictobject *mp, register PyObject *key)) SEG_DICTOBJECT_C;


#endif /* DICTOBJECT_C_H */

