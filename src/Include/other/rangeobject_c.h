#ifndef RANGEOBJECT_C_H
#define RANGEOBJECT_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from rangeobject.c */



BINARYFUNC1_PROTO(range_concat, rangeobject) SEG_RANGEOBJECT_C;
INQUIRY_PROTO(range_length, rangeobject) SEG_RANGEOBJECT_C;
INTARGFUNC_PROTO(range_repeat, rangeobject) SEG_RANGEOBJECT_C;
INTARGFUNC_PROTO(range_item, rangeobject) SEG_RANGEOBJECT_C;
INTINTARGFUNC_PROTO(range_slice, rangeobject) SEG_RANGEOBJECT_C;
DESTRUCTOR_PROTO(range_dealloc, rangeobject) SEG_RANGEOBJECT_C;
PRINTFUNC_PROTO(range_print, rangeobject) SEG_RANGEOBJECT_C;
GETATTRFUNC_PROTO(range_getattr, rangeobject) SEG_RANGEOBJECT_C;
CMPFUNC_PROTO(range_compare, rangeobject) SEG_RANGEOBJECT_C;
REPRFUNC_PROTO(range_repr, rangeobject) SEG_RANGEOBJECT_C;
BINARYFUNC1_PROTO(range_tolist, rangeobject) SEG_RANGEOBJECT_C;

#endif /* RANGEOBJECT_C_H */
