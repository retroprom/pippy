#ifndef FUNCOBJECT_C_H
#define FUNCOBJECT_C_H

#include "proto_defs.h"


DESTRUCTOR_PROTO(func_dealloc, PyFunctionObject) SEG_FUNCOBJECT_C;
GETATTRFUNC_PROTO(func_getattr, PyFunctionObject) SEG_FUNCOBJECT_C;
CMPFUNC_PROTO(func_compare, PyFunctionObject) SEG_FUNCOBJECT_C;
REPRFUNC_PROTO(func_repr, PyFunctionObject) SEG_FUNCOBJECT_C;
HASHFUNC_PROTO(func_hash, PyFunctionObject) SEG_FUNCOBJECT_C;


#endif /* FUNCOBJECT_C_H */
