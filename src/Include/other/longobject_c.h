#ifndef LONGOBJECT_C_H
#define LONGOBJECT_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from longobject.c */
static PyLongObject *long_normalize Py_PROTO((PyLongObject *)) SEG_LONGOBJECT_C;
static PyLongObject *mul1 Py_PROTO((PyLongObject *, wdigit)) SEG_LONGOBJECT_C;
static PyLongObject *muladd1 Py_PROTO((PyLongObject *, wdigit, wdigit)) SEG_LONGOBJECT_C;
static PyLongObject *divrem1 Py_PROTO((PyLongObject *, wdigit, digit *)) SEG_LONGOBJECT_C;
static PyObject *long_format Py_PROTO((PyObject *aa, int base, int addL)) SEG_LONGOBJECT_C;
static PyLongObject *x_divrem
	Py_PROTO((PyLongObject *, PyLongObject *, PyLongObject **)) SEG_LONGOBJECT_C;
static PyObject *long_pos Py_PROTO((PyLongObject *)) SEG_LONGOBJECT_C;
static int long_divrem Py_PROTO((PyLongObject *, PyLongObject *,
	PyLongObject **, PyLongObject **)) SEG_LONGOBJECT_C;

static PyLongObject *x_add Py_PROTO((PyLongObject *, PyLongObject *)) SEG_LONGOBJECT_C;
static PyLongObject *x_sub Py_PROTO((PyLongObject *, PyLongObject *)) SEG_LONGOBJECT_C;


static int l_divmod Py_PROTO((PyLongObject *, PyLongObject *,
	PyLongObject **, PyLongObject **)) SEG_LONGOBJECT_C;
static PyObject *long_bitwise Py_PROTO((PyLongObject *, int, PyLongObject *)) SEG_LONGOBJECT_C;
/* end previously forward decls */

UNARYFUNC_PROTO(long_neg, PyLongObject) SEG_LONGOBJECT_C;
UNARYFUNC_PROTO(long_pos, PyLongObject) SEG_LONGOBJECT_C;
UNARYFUNC_PROTO(long_abs, PyLongObject) SEG_LONGOBJECT_C;
UNARYFUNC_PROTO(long_int, PyObject) SEG_LONGOBJECT_C;
UNARYFUNC_PROTO(long_long, PyObject) SEG_LONGOBJECT_C;
UNARYFUNC_PROTO(long_float, PyObject) SEG_LONGOBJECT_C;
UNARYFUNC_PROTO(long_oct, PyObject) SEG_LONGOBJECT_C;
UNARYFUNC_PROTO(long_hex, PyObject) SEG_LONGOBJECT_C;
UNARYFUNC_PROTO(long_invert, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_add, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_sub, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_mul, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_div, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_mod, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_divmod, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_lshift, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_rshift, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_and, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_xor, PyLongObject) SEG_LONGOBJECT_C;
BINARYFUNC_PROTO( long_or, PyLongObject) SEG_LONGOBJECT_C;
TERNARYFUNC_PROTO(long_pow, PyLongObject) SEG_LONGOBJECT_C;
INQUIRY_PROTO(long_nonzero, PyLongObject) SEG_LONGOBJECT_C;
COERCION_PROTO(long_coerce) SEG_LONGOBJECT_C;
DESTRUCTOR_PROTO(long_dealloc, PyObject) SEG_LONGOBJECT_C;
CMPFUNC_PROTO(long_compare, PyLongObject) SEG_LONGOBJECT_C;
REPRFUNC_PROTO(long_repr, PyObject) SEG_LONGOBJECT_C;
HASHFUNC_PROTO(long_hash, PyLongObject) SEG_LONGOBJECT_C;


#endif /* LONGOBJECT_C_H */
