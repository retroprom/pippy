#ifndef TYPEOBJECT_C_H
#define TYPEOBJECT_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from typeobject.c */

GETATTRFUNC_PROTO(type_getattr, PyTypeObject) SEG_TYPEOBJECT_C;
REPRFUNC_PROTO(type_repr, PyTypeObject) SEG_TYPEOBJECT_C SEG_TYPEOBJECT_C;

#endif /* TYPEOBJECT_C_H */
