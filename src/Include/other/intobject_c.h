#ifndef INTOBJECT_C_H
#define INTOBJECT_C_H

#include "proto_defs.h"
static PyObject *err_ovf Py_PROTO((char *)) SEG_INTOBJECT_C;
static PyIntObject *fill_free_list() SEG_INTOBJECT_C;

UNARYFUNC_PROTO(int_neg, PyIntObject) SEG_INTOBJECT_C;
UNARYFUNC_PROTO(int_pos, PyIntObject) SEG_INTOBJECT_C;
UNARYFUNC_PROTO(int_abs, PyIntObject) SEG_INTOBJECT_C;
UNARYFUNC_PROTO(int_invert, PyIntObject) SEG_INTOBJECT_C;
UNARYFUNC_PROTO(int_int, PyIntObject) SEG_INTOBJECT_C;
UNARYFUNC_PROTO(int_long, PyIntObject) SEG_INTOBJECT_C;
UNARYFUNC_PROTO(int_float, PyIntObject) SEG_INTOBJECT_C;
UNARYFUNC_PROTO(int_oct, PyIntObject) SEG_INTOBJECT_C;
UNARYFUNC_PROTO(int_hex, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_add, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_sub, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_mul, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_div, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_mod, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_divmod, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_lshift, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_rshift, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_and, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_xor, PyIntObject) SEG_INTOBJECT_C;
BINARYFUNC_PROTO(int_or, PyIntObject) SEG_INTOBJECT_C;
TERNARYFUNC_PROTO(int_pow, PyIntObject) SEG_INTOBJECT_C;
INQUIRY_PROTO(int_nonzero, PyIntObject) SEG_INTOBJECT_C;
DESTRUCTOR_PROTO(int_dealloc, PyIntObject) SEG_INTOBJECT_C;
PRINTFUNC_PROTO(int_print, PyIntObject) SEG_INTOBJECT_C;
CMPFUNC_PROTO(int_compare, PyIntObject) SEG_INTOBJECT_C;
REPRFUNC_PROTO(int_repr, PyIntObject) SEG_INTOBJECT_C;
HASHFUNC_PROTO(int_hash, PyIntObject) SEG_INTOBJECT_C;



#endif /* INTOBJECT_C_H */
