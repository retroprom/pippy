#ifndef SLICEOBJECT_C_H
#define SLICEOBJECT_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from sliceobject.c */

REPRFUNC_PROTO(ellipsis_repr, PyObject) SEG_SLICEOBJECT_C;

DESTRUCTOR_PROTO(slice_dealloc, PySliceObject) SEG_SLICEOBJECT_C;
GETATTRFUNC_PROTO(slice_getattr, PySliceObject) SEG_SLICEOBJECT_C;
REPRFUNC_PROTO(slice_repr, PySliceObject) SEG_SLICEOBJECT_C;

#endif /* SLICEOBJECT_C_H */
