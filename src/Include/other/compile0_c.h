#ifndef COMPILE0_C_H
#define COMPILE0_C_H

#include "proto_defs.h"
DESTRUCTOR_PROTO(code_dealloc, PyCodeObject) SEG_COMPILE_C;
GETATTRFUNC_PROTO(code_getattr, PyCodeObject) SEG_COMPILE_C;
CMPFUNC_PROTO(code_compare, PyCodeObject) SEG_COMPILE_C;
REPRFUNC_PROTO(code_repr, PyCodeObject) SEG_COMPILE_C;
HASHFUNC_PROTO(code_hash, PyCodeObject) SEG_COMPILE_C;

#endif /* COMPILE0_C_H */
