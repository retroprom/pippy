#ifndef MODULEOBJECT_C_H
#define MODULEOBJECT_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from moduleobject.c */


DESTRUCTOR_PROTO(module_dealloc, PyModuleObject) SEG_MODULEOBJECT_C;
GETATTRFUNC_PROTO(module_getattr, PyModuleObject) SEG_MODULEOBJECT_C;
SETATTRFUNC_PROTO(module_setattr, PyModuleObject) SEG_MODULEOBJECT_C;
REPRFUNC_PROTO(module_repr, PyModuleObject) SEG_MODULEOBJECT_C;

#endif /* MODULEOBJECT_C_H */
